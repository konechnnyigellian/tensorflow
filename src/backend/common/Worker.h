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

#ifndef TENSERFLOW_WORKER_H
#define TENSERFLOW_WORKER_H


#include <atomic>
#include <cstdint>


#include "backend/common/interfaces/IWorker.h"


namespace tenserflow {


class Worker : public IWorker
{
public:
    Worker(size_t id, int64_t affinity, int priority);

    inline const VirtualMemory *memory() const override   { return nullptr; }
    inline size_t id() const override                     { return m_id; }
    inline uint64_t hashCount() const override            { return m_hashCount.load(std::memory_order_relaxed); }
    inline uint64_t timestamp() const override            { return m_timestamp.load(std::memory_order_relaxed); }
    inline void jobEarlyNotification(const Job&) override {}

protected:
    void storeStats();

    const int64_t m_affinity;
    const size_t m_id;
    std::atomic<uint64_t> m_hashCount;
    std::atomic<uint64_t> m_timestamp;
    uint32_t m_node     = 0;
    uint64_t m_count    = 0;
};


} // namespace tenserflow


#endif /* TENSERFLOW_WORKER_H */
