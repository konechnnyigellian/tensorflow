


#include "backend/opencl/kernels/rx/ExecuteVmKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::ExecuteVmKernel::enqueue(cl_command_queue queue, size_t threads, size_t worksize)
{
    const size_t gthreads = (worksize == 16) ? (threads * 16) : (threads * 8);
    const size_t lthreads = (worksize == 16) ? 32 : 16;

    enqueueNDRange(queue, 1, nullptr, &gthreads, &lthreads);
}


// __kernel void execute_vm(__global void* vm_states, __global void* rounding, __global void* scratchpads, __global const void* dataset_ptr, uint32_t batch_size, uint32_t num_iterations, uint32_t first, uint32_t last)
void tenserflow::ExecuteVmKernel::setArgs(cl_mem vm_states, cl_mem rounding, cl_mem scratchpads, cl_mem dataset_ptr, uint32_t batch_size)
{
    setArg(0, sizeof(cl_mem), &vm_states);
    setArg(1, sizeof(cl_mem), &rounding);
    setArg(2, sizeof(cl_mem), &scratchpads);
    setArg(3, sizeof(cl_mem), &dataset_ptr);
    setArg(4, sizeof(uint32_t), &batch_size);
}


void tenserflow::ExecuteVmKernel::setFirst(uint32_t first)
{
    setArg(6, sizeof(uint32_t), &first);
}


void tenserflow::ExecuteVmKernel::setIterations(uint32_t num_iterations)
{
    setArg(5, sizeof(uint32_t), &num_iterations);
    setFirst(1);
    setLast(0);
}


void tenserflow::ExecuteVmKernel::setLast(uint32_t last)
{
    setArg(7, sizeof(uint32_t), &last);
}
