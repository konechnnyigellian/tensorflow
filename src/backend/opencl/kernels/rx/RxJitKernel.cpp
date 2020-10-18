


#include "backend/opencl/kernels/rx/RxJitKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::RxJitKernel::enqueue(cl_command_queue queue, size_t threads, uint32_t iteration)
{
    setArg(6, sizeof(uint32_t), &iteration);

    const size_t gthreads        = threads * 32;
    static const size_t lthreads = 64;

    enqueueNDRange(queue, 1, nullptr, &gthreads, &lthreads);
}


// __kernel void randomx_jit(__global ulong* entropy, __global ulong* registers, __global uint2* intermediate_programs, __global uint* programs, uint batch_size, __global uint32_t* rounding, uint32_t iteration)
void tenserflow::RxJitKernel::setArgs(cl_mem entropy, cl_mem registers, cl_mem intermediate_programs, cl_mem programs, uint32_t batch_size, cl_mem rounding)
{
    setArg(0, sizeof(cl_mem), &entropy);
    setArg(1, sizeof(cl_mem), &registers);
    setArg(2, sizeof(cl_mem), &intermediate_programs);
    setArg(3, sizeof(cl_mem), &programs);
    setArg(4, sizeof(uint32_t), &batch_size);
    setArg(5, sizeof(cl_mem), &rounding);
}
