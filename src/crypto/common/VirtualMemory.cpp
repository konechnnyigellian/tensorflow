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
#include "base/io/log/Log.h"
#include "crypto/common/MemoryPool.h"
#include "crypto/common/portable/mm_malloc.h"


#ifdef TENSERFLOW_FEATURE_HWLOC
#   include "crypto/common/NUMAMemoryPool.h"
#endif


#include <cinttypes>
#include <mutex>


namespace tenserflow {

static IMemoryPool *pool = nullptr;
static std::mutex mutex;

} // namespace tenserflow


tenserflow::VirtualMemory::VirtualMemory(size_t size, bool hugePages, bool oneGbPages, bool usePool, uint32_t node, size_t alignSize) :
    m_size(align(size)),
    m_capacity(m_size),
    m_node(node)
{
    if (usePool) {
        std::lock_guard<std::mutex> lock(mutex);
        if (hugePages && !pool->isHugePages(node) && allocateLargePagesMemory()) {
            return;
        }

        m_scratchpad = pool->get(m_size, node);
        if (m_scratchpad) {
            m_flags.set(FLAG_HUGEPAGES, pool->isHugePages(node));
            m_flags.set(FLAG_EXTERNAL,  true);

            return;
        }
    }

    if (oneGbPages && allocateOneGbPagesMemory()) {
        m_capacity = align(size, 1ULL << 30);
        return;
    }

    if (hugePages && allocateLargePagesMemory()) {
        return;
    }

    m_scratchpad = static_cast<uint8_t*>(_mm_malloc(m_size, alignSize));
}


tenserflow::VirtualMemory::~VirtualMemory()
{
    if (!m_scratchpad) {
        return;
    }

    if (m_flags.test(FLAG_EXTERNAL)) {
        std::lock_guard<std::mutex> lock(mutex);
        pool->release(m_node);
    }
    else if (isHugePages() || isOneGbPages()) {
        freeLargePagesMemory();
    }
    else {
        _mm_free(m_scratchpad);
    }
}


tenserflow::HugePagesInfo tenserflow::VirtualMemory::hugePages() const
{
    return { this };
}


#ifndef TENSERFLOW_FEATURE_HWLOC
uint32_t tenserflow::VirtualMemory::bindToNUMANode(int64_t)
{
    return 0;
}
#endif


void tenserflow::VirtualMemory::destroy()
{
    delete pool;
}


void tenserflow::VirtualMemory::init(size_t poolSize, bool hugePages)
{
    if (!pool) {
        osInit(hugePages);
    }

#   ifdef TENSERFLOW_FEATURE_HWLOC
    if (Cpu::info()->nodes() > 1) {
        pool = new NUMAMemoryPool(align(poolSize, Cpu::info()->nodes()), hugePages);
    } else
#   endif
    {
        pool = new MemoryPool(poolSize, hugePages);
    }
}
