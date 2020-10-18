


#include <algorithm>
#include <mutex>
#include <thread>


#include "core/Tenserflower.h"
#include "3rdparty/rapidjson/document.h"
#include "backend/common/Hashrate.h"
#include "backend/cpu/Cpu.h"
#include "backend/cpu/CpuBackend.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/kernel/Platform.h"
#include "base/net/stratum/Job.h"
#include "base/tools/Object.h"
#include "base/tools/Profiler.h"
#include "base/tools/Timer.h"
#include "core/config/Config.h"
#include "core/Controller.h"
#include "crypto/common/Nonce.h"
#include "crypto/rx/Rx.h"
#include "version.h"


#ifdef TENSERFLOW_FEATURE_API
#   include "base/api/Api.h"
#   include "base/api/interfaces/IApiRequest.h"
#endif


#ifdef TENSERFLOW_FEATURE_OPENCL
#   include "backend/opencl/OclBackend.h"
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
#   include "backend/cuda/CudaBackend.h"
#endif


#ifdef TENSERFLOW_ALGO_RANDOMX
#   include "crypto/rx/RxConfig.h"
#endif


#ifdef TENSERFLOW_ALGO_ASTROBWT
#   include "crypto/astrobwt/AstroBWT.h"
#endif


namespace tenserflow {


static std::mutex mutex;


class TenserflowerPrivate
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(TenserflowerPrivate)


    inline TenserflowerPrivate(Controller *controller) : controller(controller) {}


    inline ~TenserflowerPrivate()
    {
        delete timer;

        for (IBackend *backend : backends) {
            delete backend;
        }

#       ifdef TENSERFLOW_ALGO_RANDOMX
        Rx::destroy();
#       endif
    }


    bool isEnabled(const Algorithm &algorithm) const
    {
        for (IBackend *backend : backends) {
            if (backend->isEnabled() && backend->isEnabled(algorithm)) {
                return true;
            }
        }

        return false;
    }


    inline void rebuild()
    {
        algorithms.clear();

        for (int i = 0; i < Algorithm::MAX; ++i) {
            const Algorithm algo(static_cast<Algorithm::Id>(i));

            if (algo.isValid() && isEnabled(algo)) {
                algorithms.push_back(algo);
            }
        }
    }


    inline void handleJobChange()
    {
        if (!enabled) {
            Nonce::pause(true);
        }

        active = true;

        if (reset) {
            Nonce::reset(job.index());
        }

        for (IBackend *backend : backends) {
            backend->setJob(job);
        }

        Nonce::touch();

        if (enabled) {
            Nonce::pause(false);
        }

        if (ticks == 0) {
            ticks++;
            timer->start(500, 500);
        }
    }


#   ifdef TENSERFLOW_FEATURE_API
    void getTenserflower(rapidjson::Value &reply, rapidjson::Document &doc, int) const
    {
        using namespace rapidjson;
        auto &allocator = doc.GetAllocator();

        reply.AddMember("version",      APP_VERSION, allocator);
        reply.AddMember("kind",         APP_KIND, allocator);
        reply.AddMember("ua",           StringRef(Platform::userAgent()), allocator);
        reply.AddMember("cpu",          Cpu::toJSON(doc), allocator);
        reply.AddMember("donate_level", controller->config()->pools().donateLevel(), allocator);
        reply.AddMember("paused",       !enabled, allocator);

        Value algo(kArrayType);

        for (const Algorithm &a : algorithms) {
            algo.PushBack(StringRef(a.shortName()), allocator);
        }

        reply.AddMember("algorithms", algo, allocator);
    }


    void getHashrate(rapidjson::Value &reply, rapidjson::Document &doc, int version) const
    {
        using namespace rapidjson;
        auto &allocator = doc.GetAllocator();

        Value hashrate(kObjectType);
        Value total(kArrayType);
        Value threads(kArrayType);

        double t[3] = { 0.0 };

        for (IBackend *backend : backends) {
            const Hashrate *hr = backend->hashrate();
            if (!hr) {
                continue;
            }

            t[0] += hr->calc(Hashrate::ShortInterval);
            t[1] += hr->calc(Hashrate::MediumInterval);
            t[2] += hr->calc(Hashrate::LargeInterval);

            if (version > 1) {
                continue;
            }

            for (size_t i = 0; i < hr->threads(); i++) {
                Value thread(kArrayType);
                thread.PushBack(Hashrate::normalize(hr->calc(i, Hashrate::ShortInterval)),  allocator);
                thread.PushBack(Hashrate::normalize(hr->calc(i, Hashrate::MediumInterval)), allocator);
                thread.PushBack(Hashrate::normalize(hr->calc(i, Hashrate::LargeInterval)),  allocator);

                threads.PushBack(thread, allocator);
            }
        }

        total.PushBack(Hashrate::normalize(t[0]),  allocator);
        total.PushBack(Hashrate::normalize(t[1]), allocator);
        total.PushBack(Hashrate::normalize(t[2]),  allocator);

        hashrate.AddMember("total",   total, allocator);
        hashrate.AddMember("highest", Hashrate::normalize(maxHashrate[algorithm]), allocator);

        if (version == 1) {
            hashrate.AddMember("threads", threads, allocator);
        }

        reply.AddMember("hashrate", hashrate, allocator);
    }


    void getBackends(rapidjson::Value &reply, rapidjson::Document &doc) const
    {
        using namespace rapidjson;
        auto &allocator = doc.GetAllocator();

        reply.SetArray();

        for (IBackend *backend : backends) {
            reply.PushBack(backend->toJSON(doc), allocator);
        }
    }
#   endif


    void printHashrate(bool details)
    {
        char num[16 * 4] = { 0 };
        double speed[3] = { 0.0 };

        for (auto backend : backends) {
            const auto hashrate = backend->hashrate();
            if (hashrate) {
                speed[0] += hashrate->calc(Hashrate::ShortInterval);
                speed[1] += hashrate->calc(Hashrate::MediumInterval);
                speed[2] += hashrate->calc(Hashrate::LargeInterval);
            }

            backend->printHashrate(details);
        }

        double scale = 1.0;
        const char* h = "H/s";

        if ((speed[0] >= 1e6) || (speed[1] >= 1e6) || (speed[2] >= 1e6) || (maxHashrate[algorithm] >= 1e6)) {
            scale = 1e-6;
            h = "MH/s";
        }

#       ifdef TENSERFLOW_FEATURE_PROFILING
        ProfileScopeData* data[ProfileScopeData::MAX_DATA_COUNT];

        const uint32_t n = std::min<uint32_t>(ProfileScopeData::s_dataCount, ProfileScopeData::MAX_DATA_COUNT);
        memcpy(data, ProfileScopeData::s_data, n * sizeof(ProfileScopeData*));

        std::sort(data, data + n, [](ProfileScopeData* a, ProfileScopeData* b) {
            return strcmp(a->m_threadId, b->m_threadId) < 0;
        });

        for (uint32_t i = 0; i < n;)
        {
            uint32_t n1 = i;
            while ((n1 < n) && (strcmp(data[i]->m_threadId, data[n1]->m_threadId) == 0)) {
                ++n1;
            }

            std::sort(data + i, data + n1, [](ProfileScopeData* a, ProfileScopeData* b) {
                return a->m_totalCycles > b->m_totalCycles;
            });

            for (uint32_t j = i; j < n1; ++j) {
                ProfileScopeData* p = data[j];
                LOG_INFO("%s Thread %6s | %-30s | %7.3f%% | %9.0f ns",
                    Tags::profiler(),
                    p->m_threadId,
                    p->m_name,
                    p->m_totalCycles * 100.0 / data[i]->m_totalCycles,
                    p->m_totalCycles / p->m_totalSamples * 1e9 / ProfileScopeData::s_tscSpeed
                );
            }

            LOG_INFO("%s --------------|--------------------------------|----------|-------------", Tags::profiler());

            i = n1;
        }
#       endif

        LOG_INFO("%s " WHITE_BOLD("speed") " 10s/60s/15m " CYAN_BOLD("%s") CYAN(" %s %s ") CYAN_BOLD("%s") " max " CYAN_BOLD("%s %s"),
                 Tags::tenserflower(),
                 Hashrate::format(speed[0] * scale,                 num,          sizeof(num) / 4),
                 Hashrate::format(speed[1] * scale,                 num + 16,     sizeof(num) / 4),
                 Hashrate::format(speed[2] * scale,                 num + 16 * 2, sizeof(num) / 4), h,
                 Hashrate::format(maxHashrate[algorithm] * scale,   num + 16 * 3, sizeof(num) / 4), h
                 );
    }


#   ifdef TENSERFLOW_ALGO_RANDOMX
    inline bool initRX() { return Rx::init(job, controller->config()->rx(), controller->config()->cpu()); }
#   endif


    Algorithm algorithm;
    Algorithms algorithms;
    bool active         = false;
    bool enabled        = true;
    bool reset          = true;
    bool battery_power  = false;
    Controller *controller;
    Job job;
    mutable std::map<Algorithm::Id, double> maxHashrate;
    std::vector<IBackend *> backends;
    String userJobId;
    Timer *timer        = nullptr;
    uint64_t ticks      = 0;
};


} // namespace tenserflow



tenserflow::Tenserflower::Tenserflower(Controller *controller)
    : d_ptr(new TenserflowerPrivate(controller))
{
    const int priority = controller->config()->cpu().priority();
    if (priority >= 0) {
        Platform::setProcessPriority(priority);
        Platform::setThreadPriority(std::min(priority + 1, 5));
    }

#   ifdef TENSERFLOW_FEATURE_PROFILING
    ProfileScopeData::Init();
#   endif

#   ifdef TENSERFLOW_ALGO_RANDOMX
    Rx::init(this);
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    astrobwt::init();
#   endif

    controller->addListener(this);

#   ifdef TENSERFLOW_FEATURE_API
    controller->api()->addListener(this);
#   endif

    d_ptr->timer = new Timer(this);

    d_ptr->backends.reserve(3);
    d_ptr->backends.push_back(new CpuBackend(controller));

#   ifdef TENSERFLOW_FEATURE_OPENCL
    d_ptr->backends.push_back(new OclBackend(controller));
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    d_ptr->backends.push_back(new CudaBackend(controller));
#   endif

    d_ptr->rebuild();
}


tenserflow::Tenserflower::~Tenserflower()
{
    delete d_ptr;
}


bool tenserflow::Tenserflower::isEnabled() const
{
    return d_ptr->enabled;
}


bool tenserflow::Tenserflower::isEnabled(const Algorithm &algorithm) const
{
    return std::find(d_ptr->algorithms.begin(), d_ptr->algorithms.end(), algorithm) != d_ptr->algorithms.end();
}


const tenserflow::Algorithms &tenserflow::Tenserflower::algorithms() const
{
    return d_ptr->algorithms;
}


const std::vector<tenserflow::IBackend *> &tenserflow::Tenserflower::backends() const
{
    return d_ptr->backends;
}


tenserflow::Job tenserflow::Tenserflower::job() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return d_ptr->job;
}


void tenserflow::Tenserflower::execCommand(char command)
{
    switch (command) {
    case 'h':
    case 'H':
        d_ptr->printHashrate(true);
        break;

    case 'p':
    case 'P':
        setEnabled(false);
        break;

    case 'r':
    case 'R':
        setEnabled(true);
        break;

    case 'e':
    case 'E':
        for (auto backend : d_ptr->backends) {
            backend->printHealth();
        }
        break;

    default:
        break;
    }

    for (auto backend : d_ptr->backends) {
        backend->execCommand(command);
    }
}


void tenserflow::Tenserflower::pause()
{
    d_ptr->active = false;

    Nonce::pause(true);
    Nonce::touch();
}


void tenserflow::Tenserflower::setEnabled(bool enabled)
{
    if (d_ptr->enabled == enabled) {
        return;
    }

    if (d_ptr->battery_power && enabled) {
        LOG_INFO("%s " YELLOW_BOLD("can't resume while on battery power"), Tags::tenserflower());

        return;
    }

    d_ptr->enabled = enabled;

    if (enabled) {
        LOG_INFO("%s " GREEN_BOLD("resumed"), Tags::tenserflower());
    }
    else {
        if (d_ptr->battery_power) {
            LOG_INFO("%s " YELLOW_BOLD("paused"), Tags::tenserflower());
        }
        else {
            LOG_INFO("%s " YELLOW_BOLD("paused") ", press " MAGENTA_BG_BOLD(" r ") " to resume", Tags::tenserflower());
        }
    }

    if (!d_ptr->active) {
        return;
    }

    Nonce::pause(!enabled);
    Nonce::touch();
}


void tenserflow::Tenserflower::setJob(const Job &job, bool donate)
{
    for (IBackend *backend : d_ptr->backends) {
        backend->prepare(job);
    }

#   ifdef TENSERFLOW_ALGO_RANDOMX
    if (job.algorithm().family() == Algorithm::RANDOM_X && !Rx::isReady(job)) {
        stop();
    }
#   endif

    d_ptr->algorithm = job.algorithm();

    mutex.lock();

    const uint8_t index = donate ? 1 : 0;

    d_ptr->reset = !(d_ptr->job.index() == 1 && index == 0 && d_ptr->userJobId == job.id());
    d_ptr->job   = job;
    d_ptr->job.setIndex(index);

    if (index == 0) {
        d_ptr->userJobId = job.id();
    }

#   ifdef TENSERFLOW_ALGO_RANDOMX
    const bool ready = d_ptr->initRX();
#   else
    constexpr const bool ready = true;
#   endif

    mutex.unlock();

    if (ready) {
        d_ptr->handleJobChange();
    }
}


void tenserflow::Tenserflower::stop()
{
    Nonce::stop();

    for (IBackend *backend : d_ptr->backends) {
        backend->stop();
    }
}


void tenserflow::Tenserflower::onConfigChanged(Config *config, Config *previousConfig)
{
    d_ptr->rebuild();

    if (config->pools() != previousConfig->pools() && config->pools().active() > 0) {
        return;
    }

    const Job job = this->job();

    for (IBackend *backend : d_ptr->backends) {
        backend->setJob(job);
    }
}


void tenserflow::Tenserflower::onTimer(const Timer *)
{
    double maxHashrate          = 0.0;
    const auto healthPrintTime  = d_ptr->controller->config()->healthPrintTime();

    for (IBackend *backend : d_ptr->backends) {
        backend->tick(d_ptr->ticks);

        if (healthPrintTime && d_ptr->ticks && (d_ptr->ticks % (healthPrintTime * 2)) == 0 && backend->isEnabled()) {
            backend->printHealth();
        }

        if (backend->hashrate()) {
            maxHashrate += backend->hashrate()->calc(Hashrate::ShortInterval);
        }
    }

    d_ptr->maxHashrate[d_ptr->algorithm] = std::max(d_ptr->maxHashrate[d_ptr->algorithm], maxHashrate);

    const auto printTime = d_ptr->controller->config()->printTime();
    if (printTime && d_ptr->ticks && (d_ptr->ticks % (printTime * 2)) == 0) {
        d_ptr->printHashrate(false);
    }

    d_ptr->ticks++;

    if (d_ptr->controller->config()->isPauseOnBattery()) {
        const bool battery_power = Platform::isOnBatteryPower();
        if (battery_power && d_ptr->enabled) {
            LOG_INFO("%s " YELLOW_BOLD("on battery power"), Tags::tenserflower());
            d_ptr->battery_power = true;
            setEnabled(false);
        }
        else if (!battery_power && !d_ptr->enabled && d_ptr->battery_power) {
            LOG_INFO("%s " GREEN_BOLD("on AC power"), Tags::tenserflower());
            d_ptr->battery_power = false;
            setEnabled(true);
        }
    }
}


#ifdef TENSERFLOW_FEATURE_API
void tenserflow::Tenserflower::onRequest(IApiRequest &request)
{
    if (request.method() == IApiRequest::METHOD_GET) {
        if (request.type() == IApiRequest::REQ_SUMMARY) {
            request.accept();

            d_ptr->getTenserflower(request.reply(), request.doc(), request.version());
            d_ptr->getHashrate(request.reply(), request.doc(), request.version());
        }
        else if (request.url() == "/2/backends") {
            request.accept();

            d_ptr->getBackends(request.reply(), request.doc());
        }
    }
    else if (request.type() == IApiRequest::REQ_JSON_RPC) {
        if (request.rpcMethod() == "pause") {
            request.accept();

            setEnabled(false);
        }
        else if (request.rpcMethod() == "resume") {
            request.accept();

            setEnabled(true);
        }
        else if (request.rpcMethod() == "stop") {
            request.accept();

            stop();
        }
    }

    for (IBackend *backend : d_ptr->backends) {
        backend->handleRequest(request);
    }
}
#endif


#ifdef TENSERFLOW_ALGO_RANDOMX
void tenserflow::Tenserflower::onDatasetReady()
{
    if (!Rx::isReady(job())) {
        return;
    }

    d_ptr->handleJobChange();
}
#endif
