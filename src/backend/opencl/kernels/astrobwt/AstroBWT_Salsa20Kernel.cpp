


#include "backend/opencl/kernels/astrobwt/AstroBWT_Salsa20Kernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::AstroBWT_Salsa20Kernel::enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size)
{
    const size_t gthreads       = threads * workgroup_size;
    enqueueNDRange(queue, 1, nullptr, &gthreads, &workgroup_size);
}


void tenserflow::AstroBWT_Salsa20Kernel::setArgs(cl_mem salsa20_keys, cl_mem outputs, cl_mem output_sizes, uint32_t output_stride)
{
    setArg(0, sizeof(cl_mem), &salsa20_keys);
    setArg(1, sizeof(cl_mem), &outputs);
    setArg(2, sizeof(cl_mem), &output_sizes);
    setArg(3, sizeof(uint32_t), &output_stride);
}
