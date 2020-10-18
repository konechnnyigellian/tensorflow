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


#include "backend/common/Worker.h"
#include "base/kernel/Platform.h"
#include "base/tools/Chrono.h"
#include "crypto/common/VirtualMemory.h"


tenserflow::Worker::Worker(size_t id, int64_t affinity, int priority) :
    m_affinity(affinity),
    m_id(id),
    m_hashCount(0),
    m_timestamp(0)
{
    m_node = VirtualMemory::bindToNUMANode(affinity);

    Platform::trySetThreadAffinity(affinity);
    Platform::setThreadPriority(priority);
}


void tenserflow::Worker::storeStats()
{
    m_hashCount.store(m_count, std::memory_order_relaxed);
    m_timestamp.store(Chrono::highResolutionMSecs(), std::memory_order_relaxed);
}
