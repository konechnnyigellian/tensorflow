/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/tools/String.h"


#include <array>
#include <cstring>
#include <thread>


#if __ARM_FEATURE_CRYPTO && !defined(__APPLE__)
#   include <sys/auxv.h>
#   include <asm/hwcap.h>
#endif


#include "backend/cpu/platform/BasicCpuInfo.h"
#include "3rdparty/rapidjson/document.h"


#ifdef TENSERFLOW_OS_UNIX
namespace tenserflow {

extern String cpu_name_arm();

} // namespace tenserflow
#endif


tenserflow::BasicCpuInfo::BasicCpuInfo() :
    m_threads(std::thread::hardware_concurrency())
{
#   ifdef TENSERFLOW_ARMv8
    memcpy(m_brand, "ARMv8", 5);
#   else
    memcpy(m_brand, "ARMv7", 5);
#   endif

#   if __ARM_FEATURE_CRYPTO
#   if !defined(__APPLE__)
    m_flags.set(FLAG_AES, getauxval(AT_HWCAP) & HWCAP_AES);
#   else
    m_flags.set(FLAG_AES, true);
#   endif
#   endif

#   ifdef TENSERFLOW_OS_UNIX
    auto name = cpu_name_arm();
    if (!name.isNull()) {
        strncpy(m_brand, name, sizeof(m_brand) - 1);
    }
#   endif
}


const char *tenserflow::BasicCpuInfo::backend() const
{
    return "basic/1";
}


tenserflow::CpuThreads tenserflow::BasicCpuInfo::threads(const Algorithm &, uint32_t) const
{
    return CpuThreads(threads());
}


rapidjson::Value tenserflow::BasicCpuInfo::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    Value out(kObjectType);

    out.AddMember("brand",      StringRef(brand()), allocator);
    out.AddMember("aes",        hasAES(), allocator);
    out.AddMember("avx2",       false, allocator);
    out.AddMember("x64",        isX64(), allocator);
    out.AddMember("l2",         static_cast<uint64_t>(L2()), allocator);
    out.AddMember("l3",         static_cast<uint64_t>(L3()), allocator);
    out.AddMember("cores",      static_cast<uint64_t>(cores()), allocator);
    out.AddMember("threads",    static_cast<uint64_t>(threads()), allocator);
    out.AddMember("packages",   static_cast<uint64_t>(packages()), allocator);
    out.AddMember("nodes",      static_cast<uint64_t>(nodes()), allocator);
    out.AddMember("backend",    StringRef(backend()), allocator);
    out.AddMember("msr",        "none", allocator);
    out.AddMember("assembly",   "none", allocator);

    Value flags(kArrayType);

    if (hasAES()) {
        flags.PushBack("aes", allocator);
    }

    out.AddMember("flags", flags, allocator);

    return out;
}
