

#ifndef TENSERFLOW_CN0KERNEL_H
#define TENSERFLOW_CN0KERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class Cn0Kernel : public OclKernel
{
public:
    inline Cn0Kernel(cl_program program) : OclKernel(program, "cn0") {}

    void enqueue(cl_command_queue queue, uint32_t nonce, size_t threads);
    void setArgs(cl_mem input, int inlen, cl_mem scratchpads, cl_mem states, uint32_t threads);
};


} // namespace tenserflow


#endif /* TENSERFLOW_CN0KERNEL_H */
