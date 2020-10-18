

#ifndef TENSERFLOW_OCLCNRUNNER_H
#define TENSERFLOW_OCLCNRUNNER_H


#include "backend/opencl/runners/OclBaseRunner.h"


namespace tenserflow {


class Cn0Kernel;
class Cn1Kernel;
class Cn2Kernel;
class CnBranchKernel;


class OclCnRunner : public OclBaseRunner
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(OclCnRunner)

    OclCnRunner(size_t index, const OclLaunchData &data);
    ~OclCnRunner() override;

protected:
    size_t bufferSize() const override;
    void run(uint32_t nonce, uint32_t *hashOutput) override;
    void set(const Job &job, uint8_t *blob) override;
    void build() override;
    void init() override;

private:
    enum Branches : size_t {
        BRANCH_BLAKE_256,
        BRANCH_GROESTL_256,
        BRANCH_JH_256,
        BRANCH_SKEIN_512,
        BRANCH_MAX
    };


    cl_mem m_scratchpads    = nullptr;
    cl_mem m_states         = nullptr;
    cl_program m_cnr        = nullptr;
    Cn0Kernel *m_cn0        = nullptr;
    Cn1Kernel *m_cn1        = nullptr;
    Cn2Kernel *m_cn2        = nullptr;
    uint64_t m_height       = 0;

    std::vector<cl_mem> m_branches                = { nullptr, nullptr, nullptr, nullptr };
    std::vector<CnBranchKernel *> m_branchKernels = { nullptr, nullptr, nullptr, nullptr };
};


} /* namespace tenserflow */


#endif // TENSERFLOW_OCLCNRUNNER_H
