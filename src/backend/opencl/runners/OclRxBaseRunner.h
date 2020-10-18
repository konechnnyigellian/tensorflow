

#ifndef TENSERFLOW_OCLRXBASERUNNER_H
#define TENSERFLOW_OCLRXBASERUNNER_H


#include "backend/opencl/runners/OclBaseRunner.h"
#include "base/tools/Buffer.h"


namespace tenserflow {


class Blake2bHashRegistersKernel;
class Blake2bInitialHashKernel;
class FillAesKernel;
class FindSharesKernel;
class HashAesKernel;


class OclRxBaseRunner : public OclBaseRunner
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(OclRxBaseRunner)

    OclRxBaseRunner(size_t index, const OclLaunchData &data);
    ~OclRxBaseRunner() override;

protected:
    size_t bufferSize() const override;
    void build() override;
    void init() override;
    void run(uint32_t nonce, uint32_t *hashOutput) override;
    void set(const Job &job, uint8_t *blob) override;

protected:
    virtual void execute(uint32_t iteration) = 0;

    Blake2bHashRegistersKernel *m_blake2b_hash_registers_32 = nullptr;
    Blake2bHashRegistersKernel *m_blake2b_hash_registers_64 = nullptr;
    Blake2bInitialHashKernel *m_blake2b_initial_hash        = nullptr;
    Buffer m_seed;
    cl_mem m_dataset                                        = nullptr;
    cl_mem m_entropy                                        = nullptr;
    cl_mem m_hashes                                         = nullptr;
    cl_mem m_rounding                                       = nullptr;
    cl_mem m_scratchpads                                    = nullptr;
    FillAesKernel *m_fillAes1Rx4_scratchpad                 = nullptr;
    FillAesKernel *m_fillAes4Rx4_entropy                    = nullptr;
    FindSharesKernel *m_find_shares                         = nullptr;
    HashAesKernel *m_hashAes1Rx4                            = nullptr;
    uint32_t m_gcn_version                                  = 12;
    uint32_t m_worksize                                     = 8;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_OCLRXBASERUNNER_H
