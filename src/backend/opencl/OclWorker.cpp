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


#include "backend/opencl/OclWorker.h"
#include "backend/common/Tags.h"
#include "backend/opencl/runners/OclCnRunner.h"
#include "backend/opencl/runners/tools/OclSharedData.h"
#include "backend/opencl/runners/tools/OclSharedState.h"
#include "base/io/log/Log.h"
#include "base/tools/Chrono.h"
#include "core/Tenserflower.h"
#include "crypto/common/Nonce.h"
#include "net/JobResults.h"


#ifdef TENSERFLOW_ALGO_RANDOMX
#   include "backend/opencl/runners/OclRxJitRunner.h"
#   include "backend/opencl/runners/OclRxVmRunner.h"
#endif

#ifdef TENSERFLOW_ALGO_ASTROBWT
#   include "backend/opencl/runners/OclAstroBWTRunner.h"
#endif

#ifdef TENSERFLOW_ALGO_KAWPOW
#   include "backend/opencl/runners/OclKawPowRunner.h"
#endif

#include <cassert>
#include <thread>


namespace tenserflow {


static constexpr uint32_t kReserveCount = 32768;
std::atomic<bool> OclWorker::ready;


static inline bool isReady()                         { return !Nonce::isPaused() && OclWorker::ready; }
static inline uint32_t roundSize(uint32_t intensity) { return kReserveCount / intensity + 1; }


static inline void printError(size_t id, const char *error)
{
    LOG_ERR("%s" RED_S " thread " RED_BOLD("#%zu") RED_S " failed with error " RED_BOLD("%s"), ocl_tag(), id, error);
}


} // namespace tenserflow



tenserflow::OclWorker::OclWorker(size_t id, const OclLaunchData &data) :
    Worker(id, data.affinity, -1),
    m_algorithm(data.algorithm),
    m_tenserflower(data.tenserflower),
    m_intensity(data.thread.intensity()),
    m_sharedData(OclSharedState::get(data.device.index())),
    m_deviceIndex(data.device.index())
{
    switch (m_algorithm.family()) {
    case Algorithm::RANDOM_X:
#       ifdef TENSERFLOW_ALGO_RANDOMX
        if (data.thread.isAsm() && data.device.vendorId() == OCL_VENDOR_AMD) {
            m_runner = new OclRxJitRunner(id, data);
        }
        else {
            m_runner = new OclRxVmRunner(id, data);
        }
#       endif
        break;

    case Algorithm::ARGON2:
#       ifdef TENSERFLOW_ALGO_ARGON2
        m_runner = nullptr; // TODO OclArgon2Runner
#       endif
        break;

    case Algorithm::ASTROBWT:
#       ifdef TENSERFLOW_ALGO_ASTROBWT
        m_runner = new OclAstroBWTRunner(id, data);
#       endif
        break;

    case Algorithm::KAWPOW:
#       ifdef TENSERFLOW_ALGO_KAWPOW
        m_runner = new OclKawPowRunner(id, data);
#       endif
        break;

    default:
        m_runner = new OclCnRunner(id, data);
        break;
    }

    if (!m_runner) {
        return;
    }

    try {
        m_runner->init();
        m_runner->build();
    }
    catch (std::exception &ex) {
        printError(id, ex.what());

        delete m_runner;
        m_runner = nullptr;
    }
}


tenserflow::OclWorker::~OclWorker()
{
    delete m_runner;
}


void tenserflow::OclWorker::jobEarlyNotification(const Job& job)
{
    if (m_runner) {
        m_runner->jobEarlyNotification(job);
    }
}


bool tenserflow::OclWorker::selfTest()
{
    return m_runner != nullptr;
}


size_t tenserflow::OclWorker::intensity() const
{
    return m_runner ? m_runner->intensity() : 0;
}


void tenserflow::OclWorker::start()
{
    cl_uint results[0x100];

    const uint32_t runnerRoundSize = m_runner->roundSize();

    while (Nonce::sequence(Nonce::OPENCL) > 0) {
        if (!isReady()) {
            m_sharedData.setResumeCounter(0);

            do {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            while (!isReady() && Nonce::sequence(Nonce::OPENCL) > 0);

            if (Nonce::sequence(Nonce::OPENCL) == 0) {
                break;
            }

            m_sharedData.resumeDelay(m_id);

            if (!consumeJob()) {
                return;
            }
        }

        while (!Nonce::isOutdated(Nonce::OPENCL, m_job.sequence())) {
            m_sharedData.adjustDelay(m_id);

            const uint64_t t = Chrono::steadyMSecs();

            try {
                m_runner->run(*m_job.nonce(), results);
            }
            catch (std::exception &ex) {
                printError(id(), ex.what());

                return;
            }

            if (results[0xFF] > 0) {
                JobResults::submit(m_job.currentJob(), results, results[0xFF], m_deviceIndex);
            }

            if (!Nonce::isOutdated(Nonce::OPENCL, m_job.sequence()) && !m_job.nextRound(roundSize(runnerRoundSize), runnerRoundSize)) {
                JobResults::done(m_job.currentJob());
            }

            storeStats(t);
            std::this_thread::yield();
        }

        if (!consumeJob()) {
            return;
        }
    }
}


bool tenserflow::OclWorker::consumeJob()
{
    if (Nonce::sequence(Nonce::OPENCL) == 0) {
        return false;
    }

    m_job.add(m_tenserflower->job(), roundSize(m_intensity) * m_intensity, Nonce::OPENCL);

    try {
        m_runner->set(m_job.currentJob(), m_job.blob());
    }
    catch (std::exception &ex) {
        printError(id(), ex.what());

        return false;
    }

    return true;
}


void tenserflow::OclWorker::storeStats(uint64_t t)
{
    if (!isReady()) {
        return;
    }

    m_count += m_runner->processedHashes();

    m_sharedData.setRunTime(Chrono::steadyMSecs() - t);

    Worker::storeStats();
}
