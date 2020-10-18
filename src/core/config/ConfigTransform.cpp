


#include "base/kernel/interfaces/IConfig.h"
#include "core/config/ConfigTransform.h"
#include "crypto/cn/CnHash.h"


namespace tenserflow
{


static const char *kAffinity    = "affinity";
static const char *kAsterisk    = "*";
static const char *kCpu         = "cpu";
static const char *kEnabled     = "enabled";
static const char *kIntensity   = "intensity";
static const char *kThreads     = "threads";

#ifdef TENSERFLOW_ALGO_RANDOMX
static const char *kRandomX     = "randomx";
#endif

#ifdef TENSERFLOW_FEATURE_OPENCL
static const char *kOcl         = "opencl";
#endif

#ifdef TENSERFLOW_FEATURE_CUDA
static const char *kCuda        = "cuda";
#endif


static inline uint64_t intensity(uint64_t av)
{
    switch (av) {
    case CnHash::AV_SINGLE:
    case CnHash::AV_SINGLE_SOFT:
        return 1;

    case CnHash::AV_DOUBLE_SOFT:
    case CnHash::AV_DOUBLE:
        return 2;

    case CnHash::AV_TRIPLE_SOFT:
    case CnHash::AV_TRIPLE:
        return 3;

    case CnHash::AV_QUAD_SOFT:
    case CnHash::AV_QUAD:
        return 4;

    case CnHash::AV_PENTA_SOFT:
    case CnHash::AV_PENTA:
        return 5;

    default:
        break;
    }

    return 1;
}


static inline bool isHwAes(uint64_t av)
{
    return av == CnHash::AV_SINGLE || av == CnHash::AV_DOUBLE || (av > CnHash::AV_DOUBLE_SOFT && av < CnHash::AV_TRIPLE_SOFT);
}


} // namespace tenserflow


void tenserflow::ConfigTransform::finalize(rapidjson::Document &doc)
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    BaseTransform::finalize(doc);

    if (m_threads) {
        doc.AddMember("version", 1, allocator);

        if (!doc.HasMember(kCpu)) {
            doc.AddMember(StringRef(kCpu), Value(kObjectType), allocator);
        }

        Value profile(kObjectType);
        profile.AddMember(StringRef(kIntensity), m_intensity, allocator);
        profile.AddMember(StringRef(kThreads),   m_threads, allocator);
        profile.AddMember(StringRef(kAffinity),  m_affinity, allocator);

        doc[kCpu].AddMember(StringRef(kAsterisk), profile, doc.GetAllocator());
    }

#   ifdef TENSERFLOW_FEATURE_OPENCL
    if (m_opencl) {
        set(doc, kOcl, kEnabled, true);
    }
#   endif
}


void tenserflow::ConfigTransform::transform(rapidjson::Document &doc, int key, const char *arg)
{
    BaseTransform::transform(doc, key, arg);

    switch (key) {
    case IConfig::AVKey:          /* --av */
    case IConfig::CPUPriorityKey: /* --cpu-priority */
    case IConfig::ThreadsKey:     /* --threads */
        return transformUint64(doc, key, static_cast<uint64_t>(strtol(arg, nullptr, 10)));

    case IConfig::HugePagesKey: /* --no-huge-pages */
    case IConfig::CPUKey:       /* --no-cpu */
        return transformBoolean(doc, key, false);

    case IConfig::CPUAffinityKey: /* --cpu-affinity */
        {
            const char *p  = strstr(arg, "0x");
            return transformUint64(doc, key, p ? strtoull(p, nullptr, 16) : strtoull(arg, nullptr, 10));
        }

    case IConfig::CPUMaxThreadsKey: /* --cpu-max-threads-hint */
        return set(doc, kCpu, "max-threads-hint", static_cast<uint64_t>(strtol(arg, nullptr, 10)));

    case IConfig::MemoryPoolKey: /* --cpu-memory-pool */
        return set(doc, kCpu, "memory-pool", static_cast<int64_t>(strtol(arg, nullptr, 10)));

    case IConfig::YieldKey: /* --cpu-no-yield */
        return set(doc, kCpu, "yield", false);

    case IConfig::Argon2ImplKey: /* --argon2-impl */
        return set(doc, kCpu, "argon2-impl", arg);

#   ifdef TENSERFLOW_FEATURE_ASM
    case IConfig::AssemblyKey: /* --asm */
        return set(doc, kCpu, "asm", arg);
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    case IConfig::AstroBWTMaxSizeKey: /* --astrobwt-max-size */
        return set(doc, kCpu, "astrobwt-max-size", static_cast<uint64_t>(strtol(arg, nullptr, 10)));

    case IConfig::AstroBWTAVX2Key: /* --astrobwt-avx2 */
        return set(doc, kCpu, "astrobwt-avx2", true);
#   endif

#   ifdef TENSERFLOW_ALGO_RANDOMX
    case IConfig::RandomXInitKey: /* --randomx-init */
        return set(doc, kRandomX, "init", static_cast<int64_t>(strtol(arg, nullptr, 10)));

    case IConfig::RandomXNumaKey: /* --randomx-no-numa */
        return set(doc, kRandomX, "numa", false);

    case IConfig::RandomXModeKey: /* --randomx-mode */
        return set(doc, kRandomX, "mode", arg);

    case IConfig::RandomX1GbPagesKey: /* --randomx-1gb-pages */
        return set(doc, kRandomX, "1gb-pages", true);

    case IConfig::RandomXWrmsrKey: /* --randomx-wrmsr */
        if (arg == nullptr) {
            return set(doc, kRandomX, "wrmsr", true);
        }

        return set(doc, kRandomX, "wrmsr", static_cast<int64_t>(strtol(arg, nullptr, 10)));

    case IConfig::RandomXRdmsrKey: /* --randomx-no-rdmsr */
        return set(doc, kRandomX, "rdmsr", false);

    case IConfig::RandomXCacheQoSKey: /* --cache-qos */
        return set(doc, kRandomX, "cache_qos", true);
#   endif

#   ifdef TENSERFLOW_FEATURE_OPENCL
    case IConfig::OclKey: /* --opencl */
        m_opencl = true;
        break;

    case IConfig::OclCacheKey: /* --opencl-no-cache */
        return set(doc, kOcl, "cache", false);

    case IConfig::OclLoaderKey: /* --opencl-loader */
        return set(doc, kOcl, "loader", arg);

    case IConfig::OclDevicesKey: /* --opencl-devices */
        m_opencl = true;
        return set(doc, kOcl, "devices-hint", arg);

    case IConfig::OclPlatformKey: /* --opencl-platform */
        if (strlen(arg) < 3) {
            return set(doc, kOcl, "platform", static_cast<uint64_t>(strtol(arg, nullptr, 10)));
        }

        return set(doc, kOcl, "platform", arg);
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    case IConfig::CudaKey: /* --cuda */
        return set(doc, kCuda, kEnabled, true);

    case IConfig::CudaLoaderKey: /* --cuda-loader */
        return set(doc, kCuda, "loader", arg);

    case IConfig::CudaDevicesKey: /* --cuda-devices */
        set(doc, kCuda, kEnabled, true);
        return set(doc, kCuda, "devices-hint", arg);

    case IConfig::CudaBFactorKey: /* --cuda-bfactor-hint */
        return set(doc, kCuda, "bfactor-hint", static_cast<uint64_t>(strtol(arg, nullptr, 10)));

    case IConfig::CudaBSleepKey: /* --cuda-bsleep-hint */
        return set(doc, kCuda, "bsleep-hint", static_cast<uint64_t>(strtol(arg, nullptr, 10)));
#   endif

#   ifdef TENSERFLOW_FEATURE_NVML
    case IConfig::NvmlKey: /* --no-nvml */
        return set(doc, kCuda, "nvml", false);

    case IConfig::HealthPrintTimeKey: /* --health-print-time */
        return set(doc, "health-print-time", static_cast<uint64_t>(strtol(arg, nullptr, 10)));
#   endif

    default:
        break;
    }
}


void tenserflow::ConfigTransform::transformBoolean(rapidjson::Document &doc, int key, bool enable)
{
    switch (key) {
    case IConfig::HugePagesKey: /* --no-huge-pages */
        return set(doc, kCpu, "huge-pages", enable);

    case IConfig::CPUKey:       /* --no-cpu */
        return set(doc, kCpu, kEnabled, enable);

    default:
        break;
    }
}


void tenserflow::ConfigTransform::transformUint64(rapidjson::Document &doc, int key, uint64_t arg)
{
    using namespace rapidjson;

    switch (key) {
    case IConfig::CPUAffinityKey: /* --cpu-affinity */
        m_affinity = static_cast<int64_t>(arg);
        break;

    case IConfig::ThreadsKey: /* --threads */
        m_threads = arg;
        break;

    case IConfig::AVKey: /* --av */
        m_intensity = intensity(arg);
        set(doc, kCpu, "hw-aes", isHwAes(arg));
        break;

    case IConfig::CPUPriorityKey: /* --cpu-priority */
        return set(doc, kCpu, "priority", arg);

    default:
        break;
    }
}

