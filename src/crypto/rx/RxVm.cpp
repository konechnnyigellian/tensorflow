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


#include "crypto/randomx/randomx.h"
#include "crypto/rx/RxCache.h"
#include "crypto/rx/RxDataset.h"
#include "crypto/rx/RxVm.h"


#if defined(TENSERFLOW_FEATURE_SSE4_1)
extern "C" uint32_t rx_blake2b_use_sse41;
#endif


randomx_vm* tenserflow::RxVm::create(RxDataset *dataset, uint8_t *scratchpad, bool softAes, tenserflow::Assembly assembly, uint32_t node)
{
    int flags = 0;

    if (!softAes) {
       flags |= RANDOMX_FLAG_HARD_AES;
    }

    if (dataset->get()) {
        flags |= RANDOMX_FLAG_FULL_MEM;
    }

    if (!dataset->cache() || dataset->cache()->isJIT()) {
        flags |= RANDOMX_FLAG_JIT;
    }

    if (assembly == Assembly::AUTO) {
        assembly = Cpu::info()->assembly();
    }

    if ((assembly == Assembly::RYZEN) || (assembly == Assembly::BULLDOZER)) {
        flags |= RANDOMX_FLAG_AMD;
    }

#   if defined(TENSERFLOW_FEATURE_SSE4_1)
    rx_blake2b_use_sse41 = Cpu::info()->has(ICpuInfo::FLAG_SSE41) ? 1 : 0;
#   endif

    return randomx_create_vm(static_cast<randomx_flags>(flags), dataset->cache() ? dataset->cache()->get() : nullptr, dataset->get(), scratchpad, node);
}


void tenserflow::RxVm::destroy(randomx_vm* vm)
{
    if (vm) {
        randomx_destroy_vm(vm);
    }
}
