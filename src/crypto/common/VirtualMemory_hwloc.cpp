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
  Copyright 2020
 *
 Copyright 2020
 */


#include "crypto/common/VirtualMemory.h"
#include "backend/cpu/Cpu.h"
#include "backend/cpu/platform/HwlocCpuInfo.h"
#include "base/io/log/Log.h"


#include <hwloc.h>


uint32_t tenserflow::VirtualMemory::bindToNUMANode(int64_t affinity)
{
    if (affinity < 0 || Cpu::info()->nodes() < 2) {
        return 0;
    }

    auto cpu       = static_cast<HwlocCpuInfo *>(Cpu::info());
    hwloc_obj_t pu = hwloc_get_pu_obj_by_os_index(cpu->topology(), static_cast<unsigned>(affinity));

    if (pu == nullptr || !cpu->membind(pu->nodeset)) {
        LOG_WARN("CPU #%02" PRId64 " warning: \"can't bind memory\"", affinity);

        return 0;
    }

    return hwloc_bitmap_first(pu->nodeset);
}
