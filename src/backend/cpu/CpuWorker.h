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

#ifndef TENSERFLOW_CPUWORKER_H
#define TENSERFLOW_CPUWORKER_H


#include "backend/common/Worker.h"
#include "backend/common/WorkerJob.h"
#include "backend/cpu/CpuLaunchData.h"
#include "base/tools/Object.h"
#include "net/JobResult.h"


#ifdef TENSERFLOW_ALGO_RANDOMX
class randomx_vm;
#endif


namespace tenserflow {


class RxVm;


template<size_t N>
class CpuWorker : public Worker
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(CpuWorker)

    CpuWorker(size_t id, const CpuLaunchData &data);
    ~CpuWorker() override;

protected:
    bool selfTest() override;
    void start() override;

    inline const VirtualMemory *memory() const override { return m_memory; }
    inline size_t intensity() const override            { return N; }

private:
    inline cn_hash_fun fn(const Algorithm &algorithm) const { return CnHash::fn(algorithm, m_av, m_assembly); }

#   ifdef TENSERFLOW_ALGO_RANDOMX
    void allocateRandomX_VM();
#   endif

    bool verify(const Algorithm &algorithm, const uint8_t *referenceValue);
    bool verify2(const Algorithm &algorithm, const uint8_t *referenceValue);
    void allocateCnCtx();
    void consumeJob();

    const Algorithm m_algorithm;
    const Assembly m_assembly;
    const bool m_astrobwtAVX2;
    const bool m_hwAES;
    const bool m_yield;
    const CnHash::AlgoVariant m_av;
    const int m_astrobwtMaxSize;
    const Tenserflower *m_tenserflower;
    tensorflowght_ctx *m_ctx[N];
    uint8_t m_hash[N * 32]{ 0 };
    VirtualMemory *m_memory = nullptr;
    WorkerJob<N> m_job;

#   ifdef TENSERFLOW_ALGO_RANDOMX
    randomx_vm *m_vm = nullptr;
#   endif
};


template<>
bool CpuWorker<1>::verify2(const Algorithm &algorithm, const uint8_t *referenceValue);


extern template class CpuWorker<1>;
extern template class CpuWorker<2>;
extern template class CpuWorker<3>;
extern template class CpuWorker<4>;
extern template class CpuWorker<5>;


} // namespace tenserflow


#endif /* TENSERFLOW_CPUWORKER_H */
