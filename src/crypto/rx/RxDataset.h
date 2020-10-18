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

#ifndef TENSERFLOW_RX_DATASET_H
#define TENSERFLOW_RX_DATASET_H


#include "base/crypto/Algorithm.h"
#include "base/tools/Buffer.h"
#include "base/tools/Object.h"
#include "crypto/common/HugePagesInfo.h"
#include "crypto/randomx/configuration.h"
#include "crypto/randomx/randomx.h"
#include "crypto/rx/RxConfig.h"

#include <atomic>


struct randomx_dataset;


namespace tenserflow
{


class RxCache;
class VirtualMemory;


class RxDataset
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(RxDataset)

    RxDataset(bool hugePages, bool oneGbPages, bool cache, RxConfig::Mode mode, uint32_t node);
    RxDataset(RxCache *cache);
    ~RxDataset();

    inline randomx_dataset *get() const     { return m_dataset; }
    inline RxCache *cache() const           { return m_cache; }
    inline void setCache(RxCache *cache)    { m_cache = cache; }

    bool init(const Buffer &seed, uint32_t numThreads, int priority);
    bool isHugePages() const;
    bool isOneGbPages() const;
    HugePagesInfo hugePages(bool cache = true) const;
    size_t size(bool cache = true) const;
    void *raw() const;
    void setRaw(const void *raw);

    uint8_t *tryAllocateScrathpad();

    static inline constexpr size_t maxSize() { return RANDOMX_DATASET_MAX_SIZE; }

private:
    void allocate(bool hugePages, bool oneGbPages);

    const RxConfig::Mode m_mode = RxConfig::FastMode;
    const uint32_t m_node;
    randomx_dataset *m_dataset  = nullptr;
    RxCache *m_cache            = nullptr;
    VirtualMemory *m_memory     = nullptr;

    std::atomic<size_t> m_scratchpadOffset;
    size_t m_scratchpadLimit    = 0;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_RX_DATASET_H */
