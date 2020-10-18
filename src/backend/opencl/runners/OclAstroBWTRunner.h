

#ifndef TENSERFLOW_OCLASTROBWTRUNNER_H
#define TENSERFLOW_OCLASTROBWTRUNNER_H


#include "backend/opencl/runners/OclBaseRunner.h"

namespace tenserflow {


class AstroBWT_FilterKernel;
class AstroBWT_MainKernel;
class AstroBWT_PrepareBatch2Kernel;
class AstroBWT_Salsa20Kernel;
class AstroBWT_SHA3InitialKernel;
class AstroBWT_SHA3Kernel;
class AstroBWT_FindSharesKernel;


class OclAstroBWTRunner : public OclBaseRunner
{
public:
    static constexpr uint32_t BWT_DATA_MAX_SIZE = 600 * 1024 - 256;
    static constexpr uint32_t BWT_DATA_STRIDE = (BWT_DATA_MAX_SIZE + 256 + 255) & ~255U;

    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(OclAstroBWTRunner)

    OclAstroBWTRunner(size_t index, const OclLaunchData &data);
    ~OclAstroBWTRunner() override;

    inline uint32_t roundSize() const override { return static_cast<uint32_t>(m_batch_size1); }

    // ~0.5% of all hashes are invalid
    inline uint32_t processedHashes() const override { return static_cast<uint32_t>(m_processedHashes * 0.995); }

protected:
    size_t bufferSize() const override;
    void run(uint32_t nonce, uint32_t *hashOutput) override;
    void set(const Job &job, uint8_t *blob) override;
    void build() override;
    void init() override;

private:
    AstroBWT_SHA3InitialKernel*   m_sha3_initial_kernel   = nullptr;
    AstroBWT_SHA3Kernel*          m_sha3_kernel           = nullptr;
    AstroBWT_Salsa20Kernel*       m_salsa20_kernel        = nullptr;
    AstroBWT_MainKernel*          m_bwt_kernel            = nullptr;
    AstroBWT_FilterKernel*        m_filter_kernel         = nullptr;
    AstroBWT_PrepareBatch2Kernel* m_prepare_batch2_kernel = nullptr;
    AstroBWT_FindSharesKernel*    m_find_shares_kernel    = nullptr;


    cl_mem m_salsa20_keys                                 = nullptr;
    cl_mem m_bwt_data                                     = nullptr;
    cl_mem m_bwt_data_sizes                               = nullptr;
    cl_mem m_indices                                      = nullptr;
    cl_mem m_tmp_indices                                  = nullptr;
    cl_mem m_filtered_hashes                              = nullptr;

    uint32_t m_workgroup_size                             = 0;
    uint64_t m_bwt_allocation_size                        = 0;
    uint64_t m_batch_size1                                = 0;
    uint32_t m_processedHashes                            = 0;

    uint32_t* m_bwt_data_sizes_host                       = nullptr;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_OCLASTROBWTRUNNER_H
