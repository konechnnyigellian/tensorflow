/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_CPUINFO_H
#define TENSERFLOW_CPUINFO_H


#include "backend/cpu/CpuThreads.h"
#include "base/crypto/Algorithm.h"
#include "base/tools/Object.h"
#include "crypto/common/Assembly.h"


namespace tenserflow {


class ICpuInfo
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(ICpuInfo)

    enum Vendor : uint32_t {
        VENDOR_UNKNOWN,
        VENDOR_INTEL,
        VENDOR_AMD
    };

    enum MsrMod : uint32_t {
        MSR_MOD_NONE,
        MSR_MOD_RYZEN,
        MSR_MOD_INTEL,
        MSR_MOD_CUSTOM,
        MSR_MOD_MAX
    };

    enum Flag : uint32_t {
        FLAG_AES,
        FLAG_AVX2,
        FLAG_AVX512F,
        FLAG_BMI2,
        FLAG_OSXSAVE,
        FLAG_PDPE1GB,
        FLAG_SSE2,
        FLAG_SSSE3,
        FLAG_SSE41,
        FLAG_XOP,
        FLAG_POPCNT,
        FLAG_CAT_L3,
        FLAG_MAX
    };

    ICpuInfo()          = default;
    virtual ~ICpuInfo() = default;

#   if defined(__x86_64__) || defined(_M_AMD64) || defined (__arm64__) || defined (__aarch64__)
    inline constexpr static bool isX64() { return true; }
#   else
    inline constexpr static bool isX64() { return false; }
#   endif

    virtual Assembly::Id assembly() const                                           = 0;
    virtual bool has(Flag feature) const                                            = 0;
    virtual bool hasAES() const                                                     = 0;
    virtual bool hasAVX2() const                                                    = 0;
    virtual bool hasBMI2() const                                                    = 0;
    virtual bool hasOneGbPages() const                                              = 0;
    virtual bool hasCatL3() const                                                   = 0;
    virtual const char *backend() const                                             = 0;
    virtual const char *brand() const                                               = 0;
    virtual CpuThreads threads(const Algorithm &algorithm, uint32_t limit) const    = 0;
    virtual MsrMod msrMod() const                                                   = 0;
    virtual rapidjson::Value toJSON(rapidjson::Document &doc) const                 = 0;
    virtual size_t cores() const                                                    = 0;
    virtual size_t L2() const                                                       = 0;
    virtual size_t L3() const                                                       = 0;
    virtual size_t nodes() const                                                    = 0;
    virtual size_t packages() const                                                 = 0;
    virtual size_t threads() const                                                  = 0;
    virtual Vendor vendor() const                                                   = 0;
    virtual bool jccErratum() const                                                 = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_CPUINFO_H
