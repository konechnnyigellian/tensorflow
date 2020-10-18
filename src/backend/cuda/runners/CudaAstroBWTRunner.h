

#ifndef TENSERFLOW_CUDAASTROBWTRUNNER_H
#define TENSERFLOW_CUDAASTROBWTRUNNER_H


#include "backend/cuda/runners/CudaBaseRunner.h"


namespace tenserflow {


class CudaAstroBWTRunner : public CudaBaseRunner
{
public:
    static constexpr uint32_t BWT_DATA_MAX_SIZE = 560 * 1024 - 256;
    static constexpr uint32_t BWT_DATA_STRIDE = (BWT_DATA_MAX_SIZE + 256 + 255) & ~255U;

    CudaAstroBWTRunner(size_t index, const CudaLaunchData &data);

protected:
    inline size_t intensity() const override { return m_intensity; }
    inline size_t roundSize() const override;
    inline size_t processedHashes() const override;

    bool run(uint32_t startNonce, uint32_t *rescount, uint32_t *resnonce) override;
    bool set(const Job &job, uint8_t *blob) override;

private:
    size_t m_intensity  = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_CUDAASTROBWTRUNNER_H
