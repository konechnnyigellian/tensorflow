


#include "backend/cuda/runners/CudaBaseRunner.h"
#include "backend/cuda/wrappers/CudaLib.h"
#include "backend/cuda/CudaLaunchData.h"
#include "backend/common/Tags.h"
#include "base/io/log/Log.h"
#include "base/net/stratum/Job.h"


tenserflow::CudaBaseRunner::CudaBaseRunner(size_t id, const CudaLaunchData &data) :
    m_data(data),
    m_threadId(id)
{
}


tenserflow::CudaBaseRunner::~CudaBaseRunner()
{
    CudaLib::release(m_ctx);
}


bool tenserflow::CudaBaseRunner::init()
{
    m_ctx = CudaLib::alloc(m_data.thread.index(), m_data.thread.bfactor(), m_data.thread.bsleep());
    if (!callWrapper(CudaLib::deviceInfo(m_ctx, m_data.thread.blocks(), m_data.thread.threads(), m_data.algorithm, m_data.thread.datasetHost()))) {
        return false;
    }

    return callWrapper(CudaLib::deviceInit(m_ctx));
}


bool tenserflow::CudaBaseRunner::set(const Job &job, uint8_t *blob)
{
    m_height = job.height();
    m_target = job.target();

    return callWrapper(CudaLib::setJob(m_ctx, blob, job.size(), job.algorithm()));
}


size_t tenserflow::CudaBaseRunner::intensity() const
{
    return m_data.thread.threads() * m_data.thread.blocks();
}


bool tenserflow::CudaBaseRunner::callWrapper(bool result) const
{
    if (!result) {
        const char *error = CudaLib::lastError(m_ctx);
        if (error) {
            LOG_ERR("%s" RED_S " thread " RED_BOLD("#%zu") RED_S " failed with error " RED_BOLD("%s"), cuda_tag(), m_threadId, error);
        }
    }

    return result;
}
