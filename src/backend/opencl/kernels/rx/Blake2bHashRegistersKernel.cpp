


#include "backend/opencl/kernels/rx/Blake2bHashRegistersKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::Blake2bHashRegistersKernel::enqueue(cl_command_queue queue, size_t threads)
{
    const size_t gthreads        = threads;
    static const size_t lthreads = 64;

    enqueueNDRange(queue, 1, nullptr, &gthreads, &lthreads);
}


// __kernel void blake2b_hash_registers_32(__global void *out, __global const void* in, uint inStrideBytes)
// __kernel void blake2b_hash_registers_64(__global void *out, __global const void* in, uint inStrideBytes)
void tenserflow::Blake2bHashRegistersKernel::setArgs(cl_mem out, cl_mem in, uint32_t inStrideBytes)
{
    setArg(0, sizeof(cl_mem), &out);
    setArg(1, sizeof(cl_mem), &in);
    setArg(2, sizeof(uint32_t), &inStrideBytes);
}
