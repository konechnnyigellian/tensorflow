

#ifndef TENSERFLOW_BLAKE2BINITIALHASHKERNEL_H
#define TENSERFLOW_BLAKE2BINITIALHASHKERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class Blake2bInitialHashKernel : public OclKernel
{
public:
    inline Blake2bInitialHashKernel(cl_program program) : OclKernel(program, "blake2b_initial_hash") {}

    void enqueue(cl_command_queue queue, size_t threads);
    void setArgs(cl_mem out, cl_mem blockTemplate);
    void setBlobSize(size_t size);
    void setNonce(uint32_t nonce);
};


} // namespace tenserflow


#endif /* TENSERFLOW_BLAKE2BINITIALHASHKERNEL_H */
