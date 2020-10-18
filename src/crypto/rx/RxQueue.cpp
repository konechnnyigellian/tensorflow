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


#include "crypto/rx/RxQueue.h"
#include "backend/common/interfaces/IRxListener.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/tools/Handle.h"
#include "crypto/rx/RxBasicStorage.h"


#ifdef TENSERFLOW_FEATURE_HWLOC
#   include "crypto/rx/RxNUMAStorage.h"
#endif


tenserflow::RxQueue::RxQueue(IRxListener *listener) :
    m_listener(listener)
{
    m_async = new uv_async_t;
    m_async->data = this;

    uv_async_init(uv_default_loop(), m_async, [](uv_async_t *handle) { static_cast<RxQueue *>(handle->data)->onReady(); });

    m_thread = std::thread(&RxQueue::backgroundInit, this);
}


tenserflow::RxQueue::~RxQueue()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_state = STATE_SHUTDOWN;
    lock.unlock();

    m_cv.notify_one();

    m_thread.join();

    delete m_storage;

    Handle::close(m_async);
}


tenserflow::RxDataset *tenserflow::RxQueue::dataset(const Job &job, uint32_t nodeId)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (isReadyUnsafe(job)) {
        return m_storage->dataset(job, nodeId);
    }

    return nullptr;
}


tenserflow::HugePagesInfo tenserflow::RxQueue::hugePages()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_storage && m_state == STATE_IDLE ? m_storage->hugePages() : HugePagesInfo();
}


template<typename T>
bool tenserflow::RxQueue::isReady(const T &seed)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return isReadyUnsafe(seed);
}


void tenserflow::RxQueue::enqueue(const RxSeed &seed, const std::vector<uint32_t> &nodeset, uint32_t threads, bool hugePages, bool oneGbPages, RxConfig::Mode mode, int priority)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    if (!m_storage) {
#       ifdef TENSERFLOW_FEATURE_HWLOC
        if (!nodeset.empty()) {
            m_storage = new RxNUMAStorage(nodeset);
        }
        else
#       endif
        {
            m_storage = new RxBasicStorage();
        }
    }

    if (m_state == STATE_PENDING && m_seed == seed) {
        return;
    }

    m_queue.emplace_back(seed, nodeset, threads, hugePages, oneGbPages, mode, priority);
    m_seed  = seed;
    m_state = STATE_PENDING;

    lock.unlock();

    m_cv.notify_one();
}


template<typename T>
bool tenserflow::RxQueue::isReadyUnsafe(const T &seed) const
{
    return m_storage != nullptr && m_storage->isAllocated() && m_state == STATE_IDLE && m_seed == seed;
}


void tenserflow::RxQueue::backgroundInit()
{
    while (m_state != STATE_SHUTDOWN) {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_state == STATE_IDLE) {
            m_cv.wait(lock, [this]{ return m_state != STATE_IDLE; });
        }

        if (m_state != STATE_PENDING) {
            continue;
        }

        const auto item = m_queue.back();
        m_queue.clear();

        lock.unlock();

        LOG_INFO("%s" MAGENTA_BOLD("init dataset%s") " algo " WHITE_BOLD("%s (") CYAN_BOLD("%u") WHITE_BOLD(" threads)") BLACK_BOLD(" seed %s..."),
                 Tags::randomx(),
                 item.nodeset.size() > 1 ? "s" : "",
                 item.seed.algorithm().shortName(),
                 item.threads,
                 Buffer::toHex(item.seed.data().data(), 8).data()
                 );

        m_storage->init(item.seed, item.threads, item.hugePages, item.oneGbPages, item.mode, item.priority);

        lock = std::unique_lock<std::mutex>(m_mutex);

        if (m_state == STATE_SHUTDOWN || !m_queue.empty()) {
            continue;
        }

        m_state = STATE_IDLE;
        uv_async_send(m_async);
    }
}


void tenserflow::RxQueue::onReady()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    const bool ready = m_listener && m_state == STATE_IDLE;
    lock.unlock();

    if (ready) {
        m_listener->onDatasetReady();
    }
}


namespace tenserflow {


template bool RxQueue::isReady(const Job &);
template bool RxQueue::isReady(const RxSeed &);


} // namespace tenserflow
