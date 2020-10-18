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

#ifndef TENSERFLOW_BASICCPUINFO_H
#define TENSERFLOW_BASICCPUINFO_H


#include "backend/cpu/interfaces/ICpuInfo.h"


#include <bitset>


namespace tenserflow {


class BasicCpuInfo : public ICpuInfo
{
public:
    BasicCpuInfo();

protected:
    const char *backend() const override;
    CpuThreads threads(const Algorithm &algorithm, uint32_t limit) const override;
    rapidjson::Value toJSON(rapidjson::Document &doc) const override;

    inline Assembly::Id assembly() const override   { return m_assembly; }
    inline bool has(Flag flag) const override       { return m_flags.test(flag); }
    inline bool hasAES() const override             { return has(FLAG_AES); }
    inline bool hasAVX2() const override            { return has(FLAG_AVX2); }
    inline bool hasBMI2() const override            { return has(FLAG_BMI2); }
    inline bool hasOneGbPages() const override      { return has(FLAG_PDPE1GB); }
    inline bool hasCatL3() const override           { return has(FLAG_CAT_L3); }
    inline const char *brand() const override       { return m_brand; }
    inline MsrMod msrMod() const override           { return m_msrMod; }
    inline size_t cores() const override            { return 0; }
    inline size_t L2() const override               { return 0; }
    inline size_t L3() const override               { return 0; }
    inline size_t nodes() const override            { return 0; }
    inline size_t packages() const override         { return 1; }
    inline size_t threads() const override          { return m_threads; }
    inline Vendor vendor() const override           { return m_vendor; }
    inline bool jccErratum() const override         { return m_jccErratum; }

protected:
    char m_brand[64 + 6]{};
    size_t m_threads;
    Vendor m_vendor         = VENDOR_UNKNOWN;
    bool m_jccErratum       = false;

private:
    Assembly m_assembly     = Assembly::NONE;
    MsrMod m_msrMod         = MSR_MOD_NONE;
    std::bitset<FLAG_MAX> m_flags;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_BASICCPUINFO_H */
