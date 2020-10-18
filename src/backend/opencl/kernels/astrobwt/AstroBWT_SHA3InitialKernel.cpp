


#include "backend/opencl/kernels/astrobwt/AstroBWT_SHA3InitialKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::AstroBWT_SHA3InitialKernel::enqueue(cl_command_queue queue, size_t threads)
{
    const size_t workgroup_size = 32;
    const size_t gthreads       = threads * workgroup_size;
    enqueueNDRange(queue, 1, nullptr, &gthreads, &workgroup_size);
}


void tenserflow::AstroBWT_SHA3InitialKernel::setArgs(cl_mem input, uint32_t input_size, uint32_t nonce, cl_mem output_salsa20_keys)
{
    setArg(0, sizeof(cl_mem), &input);
    setArg(1, sizeof(uint32_t), &input_size);
    setArg(2, sizeof(uint32_t), &nonce);
    setArg(3, sizeof(cl_mem), &output_salsa20_keys);
}
