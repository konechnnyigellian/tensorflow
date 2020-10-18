


#include <cassert>


#include "backend/cpu/Cpu.h"
#include "3rdparty/rapidjson/document.h"


#if defined(TENSERFLOW_FEATURE_HWLOC)
#   include "backend/cpu/platform/HwlocCpuInfo.h"
#elif defined(TENSERFLOW_FEATURE_LIBCPUID)
#   include "backend/cpu/platform/AdvancedCpuInfo.h"
#else
#   include "backend/cpu/platform/BasicCpuInfo.h"
#endif


static tenserflow::ICpuInfo *cpuInfo = nullptr;


tenserflow::ICpuInfo *tenserflow::Cpu::info()
{
    if (cpuInfo == nullptr) {
#       if defined(TENSERFLOW_FEATURE_HWLOC)
        cpuInfo = new HwlocCpuInfo();
#       elif defined(TENSERFLOW_FEATURE_LIBCPUID)
        cpuInfo = new AdvancedCpuInfo();
#       else
        cpuInfo = new BasicCpuInfo();
#       endif
    }

    return cpuInfo;
}


rapidjson::Value tenserflow::Cpu::toJSON(rapidjson::Document &doc)
{
    return info()->toJSON(doc);
}


void tenserflow::Cpu::release()
{
    delete cpuInfo;
    cpuInfo = nullptr;
}
