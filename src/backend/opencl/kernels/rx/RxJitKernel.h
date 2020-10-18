

#ifndef TENSERFLOW_RXJITKERNEL_H
#define TENSERFLOW_RXJITKERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class RxJitKernel : public OclKernel
{
public:
    inline RxJitKernel(cl_program program) : OclKernel(program, "randomx_jit") {}

    void enqueue(cl_command_queue queue, size_t threads, uint32_t iteration);
    void setArgs(cl_mem entropy, cl_mem registers, cl_mem intermediate_programs, cl_mem programs, uint32_t batch_size, cl_mem rounding);
};


} // namespace tenserflow


#endif /* TENSERFLOW_RXJITKERNEL_H */
