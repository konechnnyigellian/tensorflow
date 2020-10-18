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

#ifndef TENSERFLOW_ADVANCEDCPUINFO_H
#define TENSERFLOW_ADVANCEDCPUINFO_H


#include "backend/cpu/platform/BasicCpuInfo.h"


namespace tenserflow {


class AdvancedCpuInfo : public BasicCpuInfo
{
public:
    AdvancedCpuInfo();

protected:
    CpuThreads threads(const Algorithm &algorithm, uint32_t limit) const override;

    inline const char *backend() const override     { return m_backend; }
    inline size_t cores() const override            { return m_cores; }
    inline size_t L2() const override               { return m_L2; }
    inline size_t L3() const override               { return m_L3; }
    inline size_t packages() const override         { return m_packages; }
    inline size_t threads() const override          { return m_threads; }

private:
    bool m_L2_exclusive   = false;
    char m_backend[32]{};
    size_t m_cores        = 0;
    size_t m_L2           = 0;
    size_t m_L3           = 0;
    size_t m_packages     = 1;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_ADVANCEDCPUINFO_H */
