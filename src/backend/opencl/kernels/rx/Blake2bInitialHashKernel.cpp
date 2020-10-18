


#include "backend/opencl/kernels/rx/Blake2bInitialHashKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::Blake2bInitialHashKernel::enqueue(cl_command_queue queue, size_t threads)
{
    const size_t gthreads        = threads;
    static const size_t lthreads = 64;

    enqueueNDRange(queue, 1, nullptr, &gthreads, &lthreads);
}


// __kernel void blake2b_initial_hash(__global void *out, __global const void* blockTemplate, uint blockTemplateSize, uint start_nonce)
void tenserflow::Blake2bInitialHashKernel::setArgs(cl_mem out, cl_mem blockTemplate)
{
    setArg(0, sizeof(cl_mem), &out);
    setArg(1, sizeof(cl_mem), &blockTemplate);
}


void tenserflow::Blake2bInitialHashKernel::setBlobSize(size_t size)
{
    const uint32_t s = size;

    setArg(2, sizeof(uint32_t), &s);
}


void tenserflow::Blake2bInitialHashKernel::setNonce(uint32_t nonce)
{
    setArg(3, sizeof(uint32_t), &nonce);
}
