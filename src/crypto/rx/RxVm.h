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

#ifndef TENSERFLOW_RX_VM_H
#define TENSERFLOW_RX_VM_H


#include "base/tools/Object.h"
#include "backend/cpu/Cpu.h"


#include <cstdint>


class randomx_vm;


namespace tenserflow
{


class RxDataset;


class RxVm
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(RxVm);

    static randomx_vm* create(RxDataset *dataset, uint8_t *scratchpad, bool softAes, tenserflow::Assembly assembly, uint32_t node);
    static void destroy(randomx_vm* vm);
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_RX_VM_H */
