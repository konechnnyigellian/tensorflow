


#include "backend/cuda/runners/CudaAstroBWTRunner.h"
#include "backend/cuda/CudaLaunchData.h"
#include "backend/cuda/wrappers/CudaLib.h"
#include "base/net/stratum/Job.h"


constexpr uint32_t tenserflow::CudaAstroBWTRunner::BWT_DATA_STRIDE;


tenserflow::CudaAstroBWTRunner::CudaAstroBWTRunner(size_t index, const CudaLaunchData &data) :
    CudaBaseRunner(index, data)
{
    m_intensity = m_data.thread.threads() * m_data.thread.blocks();
    m_intensity -= m_intensity % 32;
}


bool tenserflow::CudaAstroBWTRunner::run(uint32_t startNonce, uint32_t *rescount, uint32_t *resnonce)
{
    return callWrapper(CudaLib::astroBWTHash(m_ctx, startNonce, m_target, rescount, resnonce));
}


bool tenserflow::CudaAstroBWTRunner::set(const Job &job, uint8_t *blob)
{
    if (!CudaBaseRunner::set(job, blob)) {
        return false;
    }

    return callWrapper(CudaLib::astroBWTPrepare(m_ctx, static_cast<uint32_t>(m_intensity)));
}


size_t tenserflow::CudaAstroBWTRunner::roundSize() const
{
    constexpr uint32_t STAGE1_SIZE = 147253;
    constexpr uint32_t STAGE1_DATA_STRIDE = (STAGE1_SIZE + 256 + 255) & ~255U;

    const uint32_t BATCH2_SIZE = m_intensity;
    const uint32_t BWT_ALLOCATION_SIZE = BATCH2_SIZE * BWT_DATA_STRIDE;
    const uint32_t BATCH1_SIZE = (BWT_ALLOCATION_SIZE / STAGE1_DATA_STRIDE) & ~255U;

    return BATCH1_SIZE;
}


size_t tenserflow::CudaAstroBWTRunner::processedHashes() const
{
    return CudaLib::deviceInt(m_ctx, CudaLib::DeviceAstroBWTProcessedHashes);
}
