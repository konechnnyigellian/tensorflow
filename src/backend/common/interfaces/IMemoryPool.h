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

#ifndef TENSERFLOW_IMEMORYPOOL_H
#define TENSERFLOW_IMEMORYPOOL_H


#include <cstddef>
#include <cstdint>


namespace tenserflow {


class IMemoryPool
{
public:
    virtual ~IMemoryPool() = default;

    virtual bool isHugePages(uint32_t node) const       = 0;
    virtual uint8_t *get(size_t size, uint32_t node)    = 0;
    virtual void release(uint32_t node)                 = 0;
};


} /* namespace tenserflow */



#endif /* TENSERFLOW_IMEMORYPOOL_H */
