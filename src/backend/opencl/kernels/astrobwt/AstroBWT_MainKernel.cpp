


#include "backend/opencl/kernels/astrobwt/AstroBWT_MainKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::AstroBWT_MainKernel::enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size)
{
    const size_t gthreads       = threads * workgroup_size;
    enqueueNDRange(queue, 1, nullptr, &gthreads, &workgroup_size);
}


void tenserflow::AstroBWT_MainKernel::setArgs(cl_mem inputs, cl_mem input_sizes, uint32_t input_stride, cl_mem indices, cl_mem tmp_indices)
{
    setArg(0, sizeof(cl_mem), &inputs);
    setArg(1, sizeof(cl_mem), &input_sizes);
    setArg(2, sizeof(uint32_t), &input_stride);
    setArg(3, sizeof(cl_mem), &indices);
    setArg(4, sizeof(cl_mem), &tmp_indices);
}
