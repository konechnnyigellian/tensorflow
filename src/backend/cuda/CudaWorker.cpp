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


#include "backend/cuda/CudaWorker.h"
#include "backend/common/Tags.h"
#include "backend/cuda/runners/CudaCnRunner.h"
#include "backend/cuda/wrappers/CudaDevice.h"
#include "base/io/log/Log.h"
#include "base/tools/Chrono.h"
#include "core/Tenserflower.h"
#include "crypto/common/Nonce.h"
#include "net/JobResults.h"


#ifdef TENSERFLOW_ALGO_RANDOMX
#   include "backend/cuda/runners/CudaRxRunner.h"
#endif


#ifdef TENSERFLOW_ALGO_ASTROBWT
#   include "backend/cuda/runners/CudaAstroBWTRunner.h"
#endif


#ifdef TENSERFLOW_ALGO_KAWPOW
#   include "backend/cuda/runners/CudaKawPowRunner.h"
#endif


#include <cassert>
#include <thread>


namespace tenserflow {


static constexpr uint32_t kReserveCount = 32768;
std::atomic<bool> CudaWorker::ready;


static inline bool isReady()                         { return !Nonce::isPaused() && CudaWorker::ready; }
static inline uint32_t roundSize(uint32_t intensity) { return kReserveCount / intensity + 1; }


} // namespace tenserflow



tenserflow::CudaWorker::CudaWorker(size_t id, const CudaLaunchData &data) :
    Worker(id, data.thread.affinity(), -1),
    m_algorithm(data.algorithm),
    m_tenserflower(data.tenserflower),
    m_deviceIndex(data.device.index())
{
    switch (m_algorithm.family()) {
    case Algorithm::RANDOM_X:
#       ifdef TENSERFLOW_ALGO_RANDOMX
        m_runner = new CudaRxRunner(id, data);
#       endif
        break;

    case Algorithm::ARGON2:
        break;

    case Algorithm::ASTROBWT:
#       ifdef TENSERFLOW_ALGO_ASTROBWT
        m_runner = new CudaAstroBWTRunner(id, data);
#       endif
        break;

    case Algorithm::KAWPOW:
#       ifdef TENSERFLOW_ALGO_KAWPOW
        m_runner = new CudaKawPowRunner(id, data);
#       endif
        break;

    default:
        m_runner = new CudaCnRunner(id, data);
        break;
    }

    if (!m_runner) {
        return;
    }

    if (!m_runner->init()) {
        delete m_runner;

        m_runner = nullptr;
    }
}


tenserflow::CudaWorker::~CudaWorker()
{
    delete m_runner;
}


void tenserflow::CudaWorker::jobEarlyNotification(const Job& job)
{
    if (m_runner) {
        m_runner->jobEarlyNotification(job);
    }
}


bool tenserflow::CudaWorker::selfTest()
{
    return m_runner != nullptr;
}


size_t tenserflow::CudaWorker::intensity() const
{
    return m_runner ? m_runner->roundSize() : 0;
}


void tenserflow::CudaWorker::start()
{
    while (Nonce::sequence(Nonce::CUDA) > 0) {
        if (!isReady()) {
            do {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            while (!isReady() && Nonce::sequence(Nonce::CUDA) > 0);

            if (Nonce::sequence(Nonce::CUDA) == 0) {
                break;
            }

            if (!consumeJob()) {
                return;
            }
        }

        while (!Nonce::isOutdated(Nonce::CUDA, m_job.sequence())) {
            uint32_t foundNonce[16] = { 0 };
            uint32_t foundCount     = 0;

            if (!m_runner->run(*m_job.nonce(), &foundCount, foundNonce)) {
                return;
            }

            if (foundCount) {
                JobResults::submit(m_job.currentJob(), foundNonce, foundCount, m_deviceIndex);
            }

            const size_t batch_size = intensity();
            if (!Nonce::isOutdated(Nonce::CUDA, m_job.sequence()) && !m_job.nextRound(roundSize(batch_size), batch_size)) {
                JobResults::done(m_job.currentJob());
            }

            storeStats();
            std::this_thread::yield();
        }

        if (!consumeJob()) {
            return;
        }
    }
}


bool tenserflow::CudaWorker::consumeJob()
{
    if (Nonce::sequence(Nonce::CUDA) == 0) {
        return false;
    }

    const size_t batch_size = intensity();
    m_job.add(m_tenserflower->job(), roundSize(batch_size) * batch_size, Nonce::CUDA);

    return m_runner->set(m_job.currentJob(), m_job.blob());
}


void tenserflow::CudaWorker::storeStats()
{
    if (!isReady()) {
        return;
    }

    m_count += m_runner ? m_runner->processedHashes() : 0;

    Worker::storeStats();
}
