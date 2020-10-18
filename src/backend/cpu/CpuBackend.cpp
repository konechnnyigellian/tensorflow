


#include <mutex>


#include "backend/cpu/CpuBackend.h"
#include "3rdparty/rapidjson/document.h"
#include "backend/common/Hashrate.h"
#include "backend/common/interfaces/IWorker.h"
#include "backend/common/Tags.h"
#include "backend/common/Workers.h"
#include "backend/cpu/Cpu.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/net/stratum/Job.h"
#include "base/tools/Chrono.h"
#include "base/tools/String.h"
#include "core/config/Config.h"
#include "core/Controller.h"
#include "crypto/common/VirtualMemory.h"
#include "crypto/rx/Rx.h"
#include "crypto/rx/RxDataset.h"


#ifdef TENSERFLOW_FEATURE_API
#   include "base/api/interfaces/IApiRequest.h"
#endif


#ifdef TENSERFLOW_ALGO_ARGON2
#   include "crypto/argon2/Impl.h"
#endif


namespace tenserflow {


extern template class Threads<CpuThreads>;


static const String kType   = "cpu";
static std::mutex mutex;


struct CpuLaunchStatus
{
public:
    inline const HugePagesInfo &hugePages() const   { return m_hugePages; }
    inline size_t memory() const                    { return m_ways * m_memory; }
    inline size_t threads() const                   { return m_threads; }
    inline size_t ways() const                      { return m_ways; }

    inline void start(const std::vector<CpuLaunchData> &threads, size_t memory)
    {
        m_hugePages.reset();
        m_memory    = memory;
        m_started   = 0;
        m_errors    = 0;
        m_threads   = threads.size();
        m_ways      = 0;
        m_ts        = Chrono::steadyMSecs();
    }

    inline bool started(IWorker *worker, bool ready)
    {
        if (ready) {
            m_started++;

            m_hugePages += worker->memory()->hugePages();
            m_ways      += worker->intensity();
        }
        else {
            m_errors++;
        }

        return (m_started + m_errors) == m_threads;
    }

    inline void print() const
    {
        if (m_started == 0) {
            LOG_ERR("%s " RED_BOLD("disabled") YELLOW(" (failed to start threads)"), Tags::cpu());

            return;
        }

        LOG_INFO("%s" GREEN_BOLD(" READY") " threads %s%zu/%zu (%zu)" CLEAR " huge pages %s%1.0f%% %zu/%zu" CLEAR " memory " CYAN_BOLD("%zu KB") BLACK_BOLD(" (%" PRIu64 " ms)"),
                 Tags::cpu(),
                 m_errors == 0 ? CYAN_BOLD_S : YELLOW_BOLD_S,
                 m_started, m_threads, m_ways,
                 (m_hugePages.isFullyAllocated() ? GREEN_BOLD_S : (m_hugePages.allocated == 0 ? RED_BOLD_S : YELLOW_BOLD_S)),
                 m_hugePages.percent(),
                 m_hugePages.allocated, m_hugePages.total,
                 memory() / 1024,
                 Chrono::steadyMSecs() - m_ts
                 );
    }

private:
    HugePagesInfo m_hugePages;
    size_t m_errors       = 0;
    size_t m_memory       = 0;
    size_t m_started      = 0;
    size_t m_threads      = 0;
    size_t m_ways         = 0;
    uint64_t m_ts         = 0;
};


class CpuBackendPrivate
{
public:
    inline CpuBackendPrivate(Controller *controller) :
        controller(controller)
    {
    }


    inline void start()
    {
        LOG_INFO("%s use profile " BLUE_BG(WHITE_BOLD_S " %s ") WHITE_BOLD_S " (" CYAN_BOLD("%zu") WHITE_BOLD(" thread%s)") " scratchpad " CYAN_BOLD("%zu KB"),
                 Tags::cpu(),
                 profileName.data(),
                 threads.size(),
                 threads.size() > 1 ? "s" : "",
                 algo.l3() / 1024
                 );

        status.start(threads, algo.l3());
        workers.start(threads);
    }


    size_t ways()
    {
        std::lock_guard<std::mutex> lock(mutex);

        return status.ways();
    }


    rapidjson::Value hugePages(int version, rapidjson::Document &doc)
    {
        HugePagesInfo pages;

    #   ifdef TENSERFLOW_ALGO_RANDOMX
        if (algo.family() == Algorithm::RANDOM_X) {
            pages += Rx::hugePages();
        }
    #   endif

        mutex.lock();

        pages += status.hugePages();

        mutex.unlock();

        rapidjson::Value hugepages;

        if (version > 1) {
            hugepages.SetArray();
            hugepages.PushBack(static_cast<uint64_t>(pages.allocated), doc.GetAllocator());
            hugepages.PushBack(static_cast<uint64_t>(pages.total), doc.GetAllocator());
        }
        else {
            hugepages = pages.isFullyAllocated();
        }

        return hugepages;
    }


    Algorithm algo;
    Controller *controller;
    CpuLaunchStatus status;
    std::vector<CpuLaunchData> threads;
    String profileName;
    Workers<CpuLaunchData> workers;
};


} // namespace tenserflow


const char *tenserflow::backend_tag(uint32_t backend)
{
#   ifdef TENSERFLOW_FEATURE_OPENCL
    if (backend == Nonce::OPENCL) {
        return ocl_tag();
    }
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    if (backend == Nonce::CUDA) {
        return cuda_tag();
    }
#   endif

    return Tags::cpu();
}


const char *tenserflow::cpu_tag()
{
    return Tags::cpu();
}


tenserflow::CpuBackend::CpuBackend(Controller *controller) :
    d_ptr(new CpuBackendPrivate(controller))
{
    d_ptr->workers.setBackend(this);
}


tenserflow::CpuBackend::~CpuBackend()
{
    delete d_ptr;
}


bool tenserflow::CpuBackend::isEnabled() const
{
    return d_ptr->controller->config()->cpu().isEnabled();
}


bool tenserflow::CpuBackend::isEnabled(const Algorithm &algorithm) const
{
    return !d_ptr->controller->config()->cpu().threads().get(algorithm).isEmpty();
}


const tenserflow::Hashrate *tenserflow::CpuBackend::hashrate() const
{
    return d_ptr->workers.hashrate();
}


const tenserflow::String &tenserflow::CpuBackend::profileName() const
{
    return d_ptr->profileName;
}


const tenserflow::String &tenserflow::CpuBackend::type() const
{
    return kType;
}


void tenserflow::CpuBackend::prepare(const Job &nextJob)
{
#   ifdef TENSERFLOW_ALGO_ARGON2
    const tenserflow::Algorithm::Family f = nextJob.algorithm().family();
    if ((f == Algorithm::ARGON2) || (f == Algorithm::RANDOM_X)) {
        if (argon2::Impl::select(d_ptr->controller->config()->cpu().argon2Impl())) {
            LOG_INFO("%s use " WHITE_BOLD("argon2") " implementation " CSI "1;%dm" "%s",
                     Tags::cpu(),
                     argon2::Impl::name() == "default" ? 33 : 32,
                     argon2::Impl::name().data()
                     );
        }
    }
#   endif
}


void tenserflow::CpuBackend::printHashrate(bool details)
{
    if (!details || !hashrate()) {
        return;
    }

    char num[8 * 3] = { 0 };

    Log::print(WHITE_BOLD_S "|    CPU # | AFFINITY | 10s H/s | 60s H/s | 15m H/s |");

    size_t i = 0;
    for (const CpuLaunchData &data : d_ptr->threads) {
         Log::print("| %8zu | %8" PRId64 " | %7s | %7s | %7s |",
                    i,
                    data.affinity,
                    Hashrate::format(hashrate()->calc(i, Hashrate::ShortInterval),  num,         sizeof num / 3),
                    Hashrate::format(hashrate()->calc(i, Hashrate::MediumInterval), num + 8,     sizeof num / 3),
                    Hashrate::format(hashrate()->calc(i, Hashrate::LargeInterval),  num + 8 * 2, sizeof num / 3)
                    );

         i++;
    }

#   ifdef TENSERFLOW_FEATURE_OPENCL
    Log::print(WHITE_BOLD_S "|        - |        - | %7s | %7s | %7s |",
               Hashrate::format(hashrate()->calc(Hashrate::ShortInterval),  num,         sizeof num / 3),
               Hashrate::format(hashrate()->calc(Hashrate::MediumInterval), num + 8,     sizeof num / 3),
               Hashrate::format(hashrate()->calc(Hashrate::LargeInterval),  num + 8 * 2, sizeof num / 3)
               );
#   endif
}


void tenserflow::CpuBackend::printHealth()
{
}


void tenserflow::CpuBackend::setJob(const Job &job)
{
    if (!isEnabled()) {
        return stop();
    }

    const CpuConfig &cpu = d_ptr->controller->config()->cpu();

    std::vector<CpuLaunchData> threads = cpu.get(d_ptr->controller->tenserflower(), job.algorithm());
    if (!d_ptr->threads.empty() && d_ptr->threads.size() == threads.size() && std::equal(d_ptr->threads.begin(), d_ptr->threads.end(), threads.begin())) {
        return;
    }

    d_ptr->algo         = job.algorithm();
    d_ptr->profileName  = cpu.threads().profileName(job.algorithm());

    if (d_ptr->profileName.isNull() || threads.empty()) {
        LOG_WARN("%s " RED_BOLD("disabled") YELLOW(" (no suitable configuration found)"), Tags::cpu());

        return stop();
    }

    stop();

    d_ptr->threads = std::move(threads);
    d_ptr->start();
}


void tenserflow::CpuBackend::start(IWorker *worker, bool ready)
{
    mutex.lock();

    if (d_ptr->status.started(worker, ready)) {
        d_ptr->status.print();
    }

    mutex.unlock();

    if (ready) {
        worker->start();
    }
}


void tenserflow::CpuBackend::stop()
{
    if (d_ptr->threads.empty()) {
        return;
    }

    const uint64_t ts = Chrono::steadyMSecs();

    d_ptr->workers.stop();
    d_ptr->threads.clear();

    LOG_INFO("%s" YELLOW(" stopped") BLACK_BOLD(" (%" PRIu64 " ms)"), Tags::cpu(), Chrono::steadyMSecs() - ts);
}


void tenserflow::CpuBackend::tick(uint64_t ticks)
{
    d_ptr->workers.tick(ticks);
}


#ifdef TENSERFLOW_FEATURE_API
rapidjson::Value tenserflow::CpuBackend::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;
    auto &allocator         = doc.GetAllocator();
    const CpuConfig &cpu    = d_ptr->controller->config()->cpu();

    Value out(kObjectType);
    out.AddMember("type",       type().toJSON(), allocator);
    out.AddMember("enabled",    isEnabled(), allocator);
    out.AddMember("algo",       d_ptr->algo.toJSON(), allocator);
    out.AddMember("profile",    profileName().toJSON(), allocator);
    out.AddMember("hw-aes",     cpu.isHwAES(), allocator);
    out.AddMember("priority",   cpu.priority(), allocator);

#   ifdef TENSERFLOW_FEATURE_ASM
    const Assembly assembly = Cpu::assembly(cpu.assembly());
    out.AddMember("asm", assembly.toJSON(), allocator);
#   else
    out.AddMember("asm", false, allocator);
#   endif

#   ifdef TENSERFLOW_ALGO_ARGON2
    out.AddMember("argon2-impl", argon2::Impl::name().toJSON(), allocator);
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    out.AddMember("astrobwt-max-size", cpu.astrobwtMaxSize(), allocator);
#   endif

    out.AddMember("hugepages", d_ptr->hugePages(2, doc), allocator);
    out.AddMember("memory",    static_cast<uint64_t>(d_ptr->algo.isValid() ? (d_ptr->ways() * d_ptr->algo.l3()) : 0), allocator);

    if (d_ptr->threads.empty() || !hashrate()) {
        return out;
    }

    out.AddMember("hashrate", hashrate()->toJSON(doc), allocator);

    Value threads(kArrayType);

    size_t i = 0;
    for (const CpuLaunchData &data : d_ptr->threads) {
        Value thread(kObjectType);
        thread.AddMember("intensity",   data.intensity, allocator);
        thread.AddMember("affinity",    data.affinity, allocator);
        thread.AddMember("av",          data.av(), allocator);
        thread.AddMember("hashrate",    hashrate()->toJSON(i, doc), allocator);

        i++;
        threads.PushBack(thread, allocator);
    }

    out.AddMember("threads", threads, allocator);

    return out;
}


void tenserflow::CpuBackend::handleRequest(IApiRequest &request)
{
    if (request.type() == IApiRequest::REQ_SUMMARY) {
        request.reply().AddMember("hugepages", d_ptr->hugePages(request.version(), request.doc()), request.doc().GetAllocator());
    }
}
#endif
