


#include "backend/opencl/kernels/astrobwt/AstroBWT_SHA3Kernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::AstroBWT_SHA3Kernel::enqueue(cl_command_queue queue, size_t threads)
{
    const size_t workgroup_size = 32;
    const size_t gthreads       = threads * workgroup_size;
    enqueueNDRange(queue, 1, nullptr, &gthreads, &workgroup_size);
}


void tenserflow::AstroBWT_SHA3Kernel::setArgs(cl_mem inputs, cl_mem input_sizes, uint32_t input_stride, cl_mem output_salsa20_keys)
{
    setArg(0, sizeof(cl_mem), &inputs);
    setArg(1, sizeof(cl_mem), &input_sizes);
    setArg(2, sizeof(uint32_t), &input_stride);
    setArg(3, sizeof(cl_mem), &output_salsa20_keys);
}
