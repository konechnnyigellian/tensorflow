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


#include "crypto/common/MemoryPool.h"
#include "crypto/common/VirtualMemory.h"


#include <cassert>


namespace tenserflow {


constexpr size_t pageSize = 2 * 1024 * 1024;


} // namespace tenserflow


tenserflow::MemoryPool::MemoryPool(size_t size, bool hugePages, uint32_t node)
{
    if (!size) {
        return;
    }

    m_memory = new VirtualMemory(size * pageSize, hugePages, false, false, node);
}


tenserflow::MemoryPool::~MemoryPool()
{
    delete m_memory;
}


bool tenserflow::MemoryPool::isHugePages(uint32_t) const
{
    return m_memory && m_memory->isHugePages();
}


uint8_t *tenserflow::MemoryPool::get(size_t size, uint32_t)
{
    assert(!(size % pageSize));

    if (!m_memory || (m_memory->size() - m_offset) < size) {
        return nullptr;
    }

    uint8_t *out = m_memory->scratchpad() + m_offset;

    m_offset += size;
    ++m_refs;

    return out;
}


void tenserflow::MemoryPool::release(uint32_t)
{
    assert(m_refs > 0);

    if (m_refs > 0) {
        --m_refs;
    }

    if (m_refs == 0) {
        m_offset = 0;
    }
}
