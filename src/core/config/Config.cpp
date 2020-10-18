

#include <algorithm>
#include <cstring>
#include <uv.h>
#include <cinttypes>


#include "core/config/Config.h"
#include "3rdparty/rapidjson/document.h"
#include "backend/cpu/Cpu.h"
#include "base/io/log/Log.h"
#include "base/kernel/interfaces/IJsonReader.h"
#include "crypto/common/Assembly.h"


#ifdef TENSERFLOW_ALGO_RANDOMX
#   include "crypto/rx/RxConfig.h"
#endif


#ifdef TENSERFLOW_FEATURE_OPENCL
#   include "backend/opencl/OclConfig.h"
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
#   include "backend/cuda/CudaConfig.h"
#endif


namespace tenserflow {

static const char *kCPU     = "cpu";

#ifdef TENSERFLOW_ALGO_RANDOMX
static const char *kRandomX = "randomx";
#endif

#ifdef TENSERFLOW_FEATURE_OPENCL
static const char *kOcl     = "opencl";
#endif

#ifdef TENSERFLOW_FEATURE_CUDA
static const char *kCuda    = "cuda";
#endif


#if defined(TENSERFLOW_FEATURE_NVML) || defined (TENSERFLOW_FEATURE_ADL)
static const char *kHealthPrintTime = "health-print-time";
#endif


class ConfigPrivate
{
public:
    CpuConfig cpu;

#   ifdef TENSERFLOW_ALGO_RANDOMX
    RxConfig rx;
#   endif

#   ifdef TENSERFLOW_FEATURE_OPENCL
    OclConfig cl;
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    CudaConfig cuda;
#   endif

#   if defined(TENSERFLOW_FEATURE_NVML) || defined (TENSERFLOW_FEATURE_ADL)
    uint32_t healthPrintTime = 60;
#   endif
};

}


tenserflow::Config::Config() :
    d_ptr(new ConfigPrivate())
{
}


tenserflow::Config::~Config()
{
    delete d_ptr;
}


const tenserflow::CpuConfig &tenserflow::Config::cpu() const
{
    return d_ptr->cpu;
}


#ifdef TENSERFLOW_FEATURE_OPENCL
const tenserflow::OclConfig &tenserflow::Config::cl() const
{
    return d_ptr->cl;
}
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
const tenserflow::CudaConfig &tenserflow::Config::cuda() const
{
    return d_ptr->cuda;
}
#endif


#ifdef TENSERFLOW_ALGO_RANDOMX
const tenserflow::RxConfig &tenserflow::Config::rx() const
{
    return d_ptr->rx;
}
#endif


#if defined(TENSERFLOW_FEATURE_NVML) || defined (TENSERFLOW_FEATURE_ADL)
uint32_t tenserflow::Config::healthPrintTime() const
{
    return d_ptr->healthPrintTime;
}
#endif


bool tenserflow::Config::isShouldSave() const
{
    if (!isAutoSave()) {
        return false;
    }

#   ifdef TENSERFLOW_FEATURE_OPENCL
    if (cl().isShouldSave()) {
        return true;
    }
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    if (cuda().isShouldSave()) {
        return true;
    }
#   endif

    return (m_upgrade || cpu().isShouldSave());
}


bool tenserflow::Config::read(const IJsonReader &reader, const char *fileName)
{
    if (!BaseConfig::read(reader, fileName)) {
        return false;
    }

    d_ptr->cpu.read(reader.getValue(kCPU));

#   ifdef TENSERFLOW_ALGO_RANDOMX
    if (!d_ptr->rx.read(reader.getValue(kRandomX))) {
        m_upgrade = true;
    }
#   endif

#   ifdef TENSERFLOW_FEATURE_OPENCL
    d_ptr->cl.read(reader.getValue(kOcl));
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    d_ptr->cuda.read(reader.getValue(kCuda));
#   endif

#   if defined(TENSERFLOW_FEATURE_NVML) || defined (TENSERFLOW_FEATURE_ADL)
    d_ptr->healthPrintTime = reader.getUint(kHealthPrintTime, d_ptr->healthPrintTime);
#   endif

    return true;
}


void tenserflow::Config::getJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;

    doc.SetObject();

    auto &allocator = doc.GetAllocator();

    Value api(kObjectType);
    api.AddMember(StringRef(kApiId),                    m_apiId.toJSON(), allocator);
    api.AddMember(StringRef(kApiWorkerId),              m_apiWorkerId.toJSON(), allocator);

    doc.AddMember(StringRef(kApi),                      api, allocator);
    doc.AddMember(StringRef(kHttp),                     m_http.toJSON(doc), allocator);
    doc.AddMember(StringRef(kAutosave),                 isAutoSave(), allocator);
    doc.AddMember(StringRef(kBackground),               isBackground(), allocator);
    doc.AddMember(StringRef(kColors),                   Log::isColors(), allocator);
    doc.AddMember(StringRef(kTitle),                    title().toJSON(), allocator);

#   ifdef TENSERFLOW_ALGO_RANDOMX
    doc.AddMember(StringRef(kRandomX),                  rx().toJSON(doc), allocator);
#   endif

    doc.AddMember(StringRef(kCPU),                      cpu().toJSON(doc), allocator);

#   ifdef TENSERFLOW_FEATURE_OPENCL
    doc.AddMember(StringRef(kOcl),                      cl().toJSON(doc), allocator);
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    doc.AddMember(StringRef(kCuda),                     cuda().toJSON(doc), allocator);
#   endif

    doc.AddMember(StringRef(Pools::kDonateLevel),       m_pools.donateLevel(), allocator);
    doc.AddMember(StringRef(Pools::kDonateOverProxy),   m_pools.proxyDonate(), allocator);
    doc.AddMember(StringRef(kLogFile),                  m_logFile.toJSON(), allocator);
    doc.AddMember(StringRef(Pools::kPools),             m_pools.toJSON(doc), allocator);
    doc.AddMember(StringRef(kPrintTime),                printTime(), allocator);
#   if defined(TENSERFLOW_FEATURE_NVML) || defined (TENSERFLOW_FEATURE_ADL)
    doc.AddMember(StringRef(kHealthPrintTime),          healthPrintTime(), allocator);
#   endif
    doc.AddMember(StringRef(Pools::kRetries),           m_pools.retries(), allocator);
    doc.AddMember(StringRef(Pools::kRetryPause),        m_pools.retryPause(), allocator);
    doc.AddMember(StringRef(kSyslog),                   isSyslog(), allocator);

#   ifdef TENSERFLOW_FEATURE_TLS
    doc.AddMember(StringRef(kTls),                      m_tls.toJSON(doc), allocator);
#   endif

    doc.AddMember(StringRef(kUserAgent),                m_userAgent.toJSON(), allocator);
    doc.AddMember(StringRef(kVerbose),                  Log::verbose(), allocator);
    doc.AddMember(StringRef(kWatch),                    m_watch, allocator);
    doc.AddMember(StringRef(kPauseOnBattery),           isPauseOnBattery(), allocator);
}
