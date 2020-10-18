


#include "backend/opencl/kernels/rx/FindSharesKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::FindSharesKernel::enqueue(cl_command_queue queue, size_t threads)
{
    const size_t gthreads        = threads;
    static const size_t lthreads = 64;

    enqueueNDRange(queue, 1, nullptr, &gthreads, &lthreads);
}


// __kernel void find_shares(__global const uint64_t* hashes, uint64_t target, uint32_t start_nonce, __global uint32_t* shares)
void tenserflow::FindSharesKernel::setArgs(cl_mem hashes, cl_mem shares)
{
    setArg(0, sizeof(cl_mem), &hashes);
    setArg(3, sizeof(cl_mem), &shares);
}


void tenserflow::FindSharesKernel::setTarget(uint64_t target)
{
    setArg(1, sizeof(uint64_t), &target);
}


void tenserflow::FindSharesKernel::setNonce(uint32_t nonce)
{
    setArg(2, sizeof(uint32_t), &nonce);
}
