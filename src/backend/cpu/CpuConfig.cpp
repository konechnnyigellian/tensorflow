


#include "backend/cpu/CpuConfig.h"
#include "3rdparty/rapidjson/document.h"
#include "backend/cpu/CpuConfig_gen.h"
#include "backend/cpu/Cpu.h"
#include "base/io/json/Json.h"

#include <algorithm>


namespace tenserflow {

static const char *kEnabled             = "enabled";
static const char *kHugePages           = "huge-pages";
static const char *kHwAes               = "hw-aes";
static const char *kMaxThreadsHint      = "max-threads-hint";
static const char *kMemoryPool          = "memory-pool";
static const char *kPriority            = "priority";
static const char *kYield               = "yield";

#ifdef TENSERFLOW_FEATURE_ASM
static const char *kAsm = "asm";
#endif

#ifdef TENSERFLOW_ALGO_ARGON2
static const char *kArgon2Impl = "argon2-impl";
#endif

#ifdef TENSERFLOW_ALGO_ASTROBWT
static const char* kAstroBWTMaxSize = "astrobwt-max-size";
static const char* kAstroBWTAVX2    = "astrobwt-avx2";
#endif


extern template class Threads<CpuThreads>;

}


bool tenserflow::CpuConfig::isHwAES() const
{
    return (m_aes == AES_AUTO ? (Cpu::info()->hasAES() ? AES_HW : AES_SOFT) : m_aes) == AES_HW;
}


rapidjson::Value tenserflow::CpuConfig::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    Value obj(kObjectType);

    obj.AddMember(StringRef(kEnabled),      m_enabled, allocator);
    obj.AddMember(StringRef(kHugePages),    m_hugePages, allocator);
    obj.AddMember(StringRef(kHwAes),        m_aes == AES_AUTO ? Value(kNullType) : Value(m_aes == AES_HW), allocator);
    obj.AddMember(StringRef(kPriority),     priority() != -1 ? Value(priority()) : Value(kNullType), allocator);
    obj.AddMember(StringRef(kMemoryPool),   m_memoryPool < 1 ? Value(m_memoryPool < 0) : Value(m_memoryPool), allocator);
    obj.AddMember(StringRef(kYield),        m_yield, allocator);

    if (m_threads.isEmpty()) {
        obj.AddMember(StringRef(kMaxThreadsHint), m_limit, allocator);
    }

#   ifdef TENSERFLOW_FEATURE_ASM
    obj.AddMember(StringRef(kAsm), m_assembly.toJSON(), allocator);
#   endif

#   ifdef TENSERFLOW_ALGO_ARGON2
    obj.AddMember(StringRef(kArgon2Impl), m_argon2Impl.toJSON(), allocator);
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    obj.AddMember(StringRef(kAstroBWTMaxSize),  m_astrobwtMaxSize, allocator);
    obj.AddMember(StringRef(kAstroBWTAVX2),     m_astrobwtAVX2, allocator);
#   endif

    m_threads.toJSON(obj, doc);

    return obj;
}


size_t tenserflow::CpuConfig::memPoolSize() const
{
    return m_memoryPool < 0 ? Cpu::info()->threads() : m_memoryPool;
}


std::vector<tenserflow::CpuLaunchData> tenserflow::CpuConfig::get(const Tenserflower *tenserflower, const Algorithm &algorithm) const
{
    std::vector<CpuLaunchData> out;
    const CpuThreads &threads = m_threads.get(algorithm);

    if (threads.isEmpty()) {
        return out;
    }

    out.reserve(threads.count());

    for (const CpuThread &thread : threads.data()) {
        out.emplace_back(tenserflower, algorithm, *this, thread);
    }

    return out;
}


void tenserflow::CpuConfig::read(const rapidjson::Value &value)
{
    if (value.IsObject()) {
        m_enabled    = Json::getBool(value, kEnabled, m_enabled);
        m_hugePages  = Json::getBool(value, kHugePages, m_hugePages);
        m_limit      = Json::getUint(value, kMaxThreadsHint, m_limit);
        m_yield      = Json::getBool(value, kYield, m_yield);

        setAesMode(Json::getValue(value, kHwAes));
        setPriority(Json::getInt(value,  kPriority, -1));
        setMemoryPool(Json::getValue(value, kMemoryPool));

#       ifdef TENSERFLOW_FEATURE_ASM
        m_assembly = Json::getValue(value, kAsm);
#       endif

#       ifdef TENSERFLOW_ALGO_ARGON2
        m_argon2Impl = Json::getString(value, kArgon2Impl);
#       endif

#       ifdef TENSERFLOW_ALGO_ASTROBWT
        const auto& astroBWTMaxSize = Json::getValue(value, kAstroBWTMaxSize);
        if (astroBWTMaxSize.IsNull() || !astroBWTMaxSize.IsInt()) {
            m_shouldSave = true;
        }
        else {
            m_astrobwtMaxSize = std::min(std::max(astroBWTMaxSize.GetInt(), 400), 1200);
        }

        const auto& astroBWTAVX2 = Json::getValue(value, kAstroBWTAVX2);
        if (astroBWTAVX2.IsNull() || !astroBWTAVX2.IsBool()) {
            m_shouldSave = true;
        }
        else {
            m_astrobwtAVX2 = astroBWTAVX2.GetBool();
        }
#       endif

        m_threads.read(value);

        generate();
    }
    else if (value.IsBool()) {
        m_enabled = value.GetBool();

        generate();
    }
    else {
        generate();
    }
}


void tenserflow::CpuConfig::generate()
{
    if (!isEnabled() || m_threads.has("*")) {
        return;
    }

    size_t count = 0;

    count += tenserflow::generate<Algorithm::CN>(m_threads, m_limit);
    count += tenserflow::generate<Algorithm::CN_LITE>(m_threads, m_limit);
    count += tenserflow::generate<Algorithm::CN_HEAVY>(m_threads, m_limit);
    count += tenserflow::generate<Algorithm::CN_PICO>(m_threads, m_limit);
    count += tenserflow::generate<Algorithm::RANDOM_X>(m_threads, m_limit);
    count += tenserflow::generate<Algorithm::ARGON2>(m_threads, m_limit);
    count += tenserflow::generate<Algorithm::ASTROBWT>(m_threads, m_limit);

    m_shouldSave |= count > 0;
}


void tenserflow::CpuConfig::setAesMode(const rapidjson::Value &value)
{
    if (value.IsBool()) {
        m_aes = value.GetBool() ? AES_HW : AES_SOFT;
    }
    else {
        m_aes = AES_AUTO;
    }
}


void tenserflow::CpuConfig::setMemoryPool(const rapidjson::Value &value)
{
    if (value.IsBool()) {
        m_memoryPool = value.GetBool() ? -1 : 0;
    }
    else if (value.IsInt()) {
        m_memoryPool = value.GetInt();
    }
}
