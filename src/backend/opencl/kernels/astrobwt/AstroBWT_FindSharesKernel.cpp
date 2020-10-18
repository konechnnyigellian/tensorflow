


#include "backend/opencl/kernels/astrobwt/AstroBWT_FindSharesKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::AstroBWT_FindSharesKernel::enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size)
{
    enqueueNDRange(queue, 1, nullptr, &threads, &workgroup_size);
}


void tenserflow::AstroBWT_FindSharesKernel::setArgs(cl_mem hashes, cl_mem filtered_hashes, cl_mem shares)
{
    setArg(0, sizeof(cl_mem), &hashes);
    setArg(1, sizeof(cl_mem), &filtered_hashes);
    setArg(3, sizeof(cl_mem), &shares);
}


void tenserflow::AstroBWT_FindSharesKernel::setTarget(uint64_t target)
{
    setArg(2, sizeof(uint64_t), &target);
}
