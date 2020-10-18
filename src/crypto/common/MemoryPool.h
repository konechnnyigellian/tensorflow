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

#ifndef TENSERFLOW_MEMORYPOOL_H
#define TENSERFLOW_MEMORYPOOL_H


#include "backend/common/interfaces/IMemoryPool.h"
#include "base/tools/Object.h"


namespace tenserflow {


class VirtualMemory;


class MemoryPool : public IMemoryPool
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(MemoryPool)

    MemoryPool(size_t size, bool hugePages, uint32_t node = 0);
    ~MemoryPool() override;

protected:
    bool isHugePages(uint32_t node) const override;
    uint8_t *get(size_t size, uint32_t node) override;
    void release(uint32_t node) override;

private:
    size_t m_refs           = 0;
    size_t m_offset         = 0;
    VirtualMemory *m_memory = nullptr;
};


} /* namespace tenserflow */



#endif /* TENSERFLOW_MEMORYPOOL_H */
