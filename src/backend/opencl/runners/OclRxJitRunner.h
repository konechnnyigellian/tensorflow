

#ifndef TENSERFLOW_OCLRXJITRUNNER_H
#define TENSERFLOW_OCLRXJITRUNNER_H


#include "backend/opencl/runners/OclRxBaseRunner.h"


namespace tenserflow {


class RxJitKernel;
class RxRunKernel;


class OclRxJitRunner : public OclRxBaseRunner
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(OclRxJitRunner)

    OclRxJitRunner(size_t index, const OclLaunchData &data);
    ~OclRxJitRunner() override;

protected:
    size_t bufferSize() const override;
    void build() override;
    void execute(uint32_t iteration) override;
    void init() override;

private:
    bool loadAsmProgram();

    cl_mem m_intermediate_programs  = nullptr;
    cl_mem m_programs               = nullptr;
    cl_mem m_registers              = nullptr;
    cl_program m_asmProgram         = nullptr;
    RxJitKernel *m_randomx_jit      = nullptr;
    RxRunKernel *m_randomx_run      = nullptr;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_OCLRXRUNNER_H
