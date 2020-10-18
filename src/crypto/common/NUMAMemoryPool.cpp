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


#include "crypto/common/NUMAMemoryPool.h"
#include "crypto/common/VirtualMemory.h"
#include "backend/cpu/Cpu.h"
#include "crypto/common/MemoryPool.h"


#include <algorithm>


tenserflow::NUMAMemoryPool::NUMAMemoryPool(size_t size, bool hugePages) :
    m_hugePages(hugePages),
    m_nodeSize(std::max<size_t>(size / Cpu::info()->nodes(), 1)),
    m_size(size)
{
}


tenserflow::NUMAMemoryPool::~NUMAMemoryPool()
{
    for (auto kv : m_map) {
        delete kv.second;
    }
}


bool tenserflow::NUMAMemoryPool::isHugePages(uint32_t node) const
{
    if (!m_size) {
        return false;
    }

    return getOrCreate(node)->isHugePages(node);
}


uint8_t *tenserflow::NUMAMemoryPool::get(size_t size, uint32_t node)
{
    if (!m_size) {
        return nullptr;
    }

    return getOrCreate(node)->get(size, node);
}


void tenserflow::NUMAMemoryPool::release(uint32_t node)
{
    const auto pool = get(node);
    if (pool) {
        pool->release(node);
    }
}


tenserflow::IMemoryPool *tenserflow::NUMAMemoryPool::get(uint32_t node) const
{
    return m_map.count(node) ? m_map.at(node) : nullptr;
}


tenserflow::IMemoryPool *tenserflow::NUMAMemoryPool::getOrCreate(uint32_t node) const
{
    auto pool = get(node);
    if (!pool) {
        pool = new MemoryPool(m_nodeSize, m_hugePages, node);
        m_map.insert({ node, pool });
    }

    return pool;
}
