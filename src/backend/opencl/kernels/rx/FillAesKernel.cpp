


#include "backend/opencl/kernels/rx/FillAesKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::FillAesKernel::enqueue(cl_command_queue queue, size_t threads)
{
    const size_t gthreads        = threads * 4;
    static const size_t lthreads = 64;

    enqueueNDRange(queue, 1, nullptr, &gthreads, &lthreads);
}


// __kernel void fillAes1Rx4_scratchpad(__global void* state, __global void* out, uint batch_size, uint rx_version)
// __kernel void fillAes4Rx4_entropy(__global void* state, __global void* out, uint batch_size, uint rx_version)
void tenserflow::FillAesKernel::setArgs(cl_mem state, cl_mem out, uint32_t batch_size, uint32_t rx_version)
{
    setArg(0, sizeof(cl_mem), &state);
    setArg(1, sizeof(cl_mem), &out);
    setArg(2, sizeof(uint32_t), &batch_size);
    setArg(3, sizeof(uint32_t), &rx_version);
}
