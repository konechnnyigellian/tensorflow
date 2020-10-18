/* TENSERflow
  Copyright 2020
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


#include "base/kernel/Platform.h"
#include "backend/cpu/platform/HwlocCpuInfo.h"
#include "backend/cpu/Cpu.h"


#include <hwloc.h>
#include <thread>


bool tenserflow::Platform::setThreadAffinity(uint64_t cpu_id)
{
    auto cpu       = static_cast<HwlocCpuInfo *>(Cpu::info());
    hwloc_obj_t pu = hwloc_get_pu_obj_by_os_index(cpu->topology(), static_cast<unsigned>(cpu_id));

    if (pu == nullptr) {
        return false;
    }

    if (hwloc_set_cpubind(cpu->topology(), pu->cpuset, HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT) >= 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return true;
    }

    const bool result = (hwloc_set_cpubind(cpu->topology(), pu->cpuset, HWLOC_CPUBIND_THREAD) >= 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return result;
}
