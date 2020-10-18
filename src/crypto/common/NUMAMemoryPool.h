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

#ifndef TENSERFLOW_NUMAMEMORYPOOL_H
#define TENSERFLOW_NUMAMEMORYPOOL_H


#include "backend/common/interfaces/IMemoryPool.h"
#include "base/tools/Object.h"


#include <map>


namespace tenserflow {


class IMemoryPool;


class NUMAMemoryPool : public IMemoryPool
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(NUMAMemoryPool)

    NUMAMemoryPool(size_t size, bool hugePages);
    ~NUMAMemoryPool() override;

protected:
    bool isHugePages(uint32_t node) const override;
    uint8_t *get(size_t size, uint32_t node) override;
    void release(uint32_t node) override;

private:
    IMemoryPool *get(uint32_t node) const;
    IMemoryPool *getOrCreate(uint32_t node) const;

    bool m_hugePages        = true;
    size_t m_nodeSize       = 0;
    size_t m_size           = 0;
    mutable std::map<uint32_t, IMemoryPool *> m_map;
};


} /* namespace tenserflow */



#endif /* TENSERFLOW_NUMAMEMORYPOOL_H */
