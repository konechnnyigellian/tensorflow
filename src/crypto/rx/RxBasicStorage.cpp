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


#include "crypto/rx/RxBasicStorage.h"
#include "backend/common/Tags.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/tools/Chrono.h"
#include "base/tools/Object.h"
#include "crypto/rx/RxAlgo.h"
#include "crypto/rx/RxCache.h"
#include "crypto/rx/RxDataset.h"
#include "crypto/rx/RxSeed.h"


namespace tenserflow {


constexpr size_t oneMiB = 1024 * 1024;


class RxBasicStoragePrivate
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(RxBasicStoragePrivate)

    inline RxBasicStoragePrivate() = default;
    inline ~RxBasicStoragePrivate() { deleteDataset(); }

    inline bool isReady(const Job &job) const   { return m_ready && m_seed == job; }
    inline RxDataset *dataset() const           { return m_dataset; }
    inline void deleteDataset()                 { delete m_dataset; m_dataset = nullptr; }


    inline void setSeed(const RxSeed &seed)
    {
        m_ready = false;

        if (m_seed.algorithm() != seed.algorithm()) {
            RxAlgo::apply(seed.algorithm());
        }

        m_seed = seed;
    }


    inline bool createDataset(bool hugePages, bool oneGbPages, RxConfig::Mode mode)
    {
        const uint64_t ts = Chrono::steadyMSecs();

        m_dataset = new RxDataset(hugePages, oneGbPages, true, mode, 0);
        if (!m_dataset->cache()->get()) {
            deleteDataset();

            LOG_INFO("%s" RED_BOLD("failed to allocate RandomX memory") BLACK_BOLD(" (%" PRIu64 " ms)"), Tags::randomx(), Chrono::steadyMSecs() - ts);

            return false;
        }

        printAllocStatus(ts);

        return true;
    }


    inline void initDataset(uint32_t threads, int priority)
    {
        const uint64_t ts = Chrono::steadyMSecs();

        m_ready = m_dataset->init(m_seed.data(), threads, priority);

        if (m_ready) {
            LOG_INFO("%s" GREEN_BOLD("dataset ready") BLACK_BOLD(" (%" PRIu64 " ms)"), Tags::randomx(), Chrono::steadyMSecs() - ts);
        }
    }


private:
    void printAllocStatus(uint64_t ts)
    {
        if (m_dataset->get() != nullptr) {
            const auto pages = m_dataset->hugePages();

            LOG_INFO("%s" GREEN_BOLD("allocated") CYAN_BOLD(" %zu MB") BLACK_BOLD(" (%zu+%zu)") " huge pages %s%1.0f%% %u/%u" CLEAR " %sJIT" BLACK_BOLD(" (%" PRIu64 " ms)"),
                     Tags::randomx(),
                     pages.size / oneMiB,
                     RxDataset::maxSize() / oneMiB,
                     RxCache::maxSize() / oneMiB,
                     (pages.isFullyAllocated() ? GREEN_BOLD_S : (pages.allocated == 0 ? RED_BOLD_S : YELLOW_BOLD_S)),
                     pages.percent(),
                     pages.allocated,
                     pages.total,
                     m_dataset->cache()->isJIT() ? GREEN_BOLD_S "+" : RED_BOLD_S "-",
                     Chrono::steadyMSecs() - ts
                     );
        }
        else {
            LOG_WARN(CLEAR "%s" YELLOW_BOLD_S "failed to allocate RandomX dataset, switching to slow mode" BLACK_BOLD(" (%" PRIu64 " ms)"), Tags::randomx(), Chrono::steadyMSecs() - ts);
        }
    }


    bool m_ready         = false;
    RxDataset *m_dataset = nullptr;
    RxSeed m_seed;
};


} // namespace tenserflow


tenserflow::RxBasicStorage::RxBasicStorage() :
    d_ptr(new RxBasicStoragePrivate())
{
}


tenserflow::RxBasicStorage::~RxBasicStorage()
{
    delete d_ptr;
}


bool tenserflow::RxBasicStorage::isAllocated() const
{
    return d_ptr->dataset() && d_ptr->dataset()->cache() && d_ptr->dataset()->cache()->get();
}


tenserflow::HugePagesInfo tenserflow::RxBasicStorage::hugePages() const
{
    if (!d_ptr->dataset()) {
        return {};
    }

    return d_ptr->dataset()->hugePages();
}


tenserflow::RxDataset *tenserflow::RxBasicStorage::dataset(const Job &job, uint32_t) const
{
    if (!d_ptr->isReady(job)) {
        return nullptr;
    }

    return d_ptr->dataset();
}


void tenserflow::RxBasicStorage::init(const RxSeed &seed, uint32_t threads, bool hugePages, bool oneGbPages, RxConfig::Mode mode, int priority)
{
    d_ptr->setSeed(seed);

    if (!d_ptr->dataset() && !d_ptr->createDataset(hugePages, oneGbPages, mode)) {
        return;
    }

    d_ptr->initDataset(threads, priority);
}
