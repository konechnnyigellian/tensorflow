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

#ifndef TENSERFLOW_RX_QUEUE_H
#define TENSERFLOW_RX_QUEUE_H


#include "base/tools/Object.h"
#include "crypto/common/HugePagesInfo.h"
#include "crypto/rx/RxConfig.h"
#include "crypto/rx/RxSeed.h"


#include <condition_variable>
#include <mutex>
#include <thread>


using uv_async_t = struct uv_async_s;


namespace tenserflow
{


class IRxListener;
class IRxStorage;
class RxDataset;


class RxQueueItem
{
public:
    RxQueueItem(const RxSeed &seed, const std::vector<uint32_t> &nodeset, uint32_t threads, bool hugePages, bool oneGbPages, RxConfig::Mode mode, int priority) :
        hugePages(hugePages),
        oneGbPages(oneGbPages),
        priority(priority),
        mode(mode),
        seed(seed),
        nodeset(nodeset),
        threads(threads)
    {}

    const bool hugePages;
    const bool oneGbPages;
    const int priority;
    const RxConfig::Mode mode;
    const RxSeed seed;
    const std::vector<uint32_t> nodeset;
    const uint32_t threads;
};


class RxQueue
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(RxQueue);

    RxQueue(IRxListener *listener);
    ~RxQueue();

    HugePagesInfo hugePages();
    RxDataset *dataset(const Job &job, uint32_t nodeId);
    template<typename T> bool isReady(const T &seed);
    void enqueue(const RxSeed &seed, const std::vector<uint32_t> &nodeset, uint32_t threads, bool hugePages, bool oneGbPages, RxConfig::Mode mode, int priority);

private:
    enum State {
        STATE_IDLE,
        STATE_PENDING,
        STATE_SHUTDOWN
    };

    template<typename T> bool isReadyUnsafe(const T &seed) const;
    void backgroundInit();
    void onReady();

    IRxListener *m_listener = nullptr;
    IRxStorage *m_storage   = nullptr;
    RxSeed m_seed;
    State m_state = STATE_IDLE;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::thread m_thread;
    std::vector<RxQueueItem> m_queue;
    uv_async_t *m_async     = nullptr;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_RX_QUEUE_H */
