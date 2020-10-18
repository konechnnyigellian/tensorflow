


#include "backend/opencl/kernels/astrobwt/AstroBWT_FilterKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::AstroBWT_FilterKernel::enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size)
{
    enqueueNDRange(queue, 1, nullptr, &threads, &workgroup_size);
}


void tenserflow::AstroBWT_FilterKernel::setArgs(uint32_t nonce, uint32_t bwt_max_size, cl_mem hashes, cl_mem filtered_hashes)
{
    setArg(0, sizeof(uint32_t), &nonce);
    setArg(1, sizeof(uint32_t), &bwt_max_size);
    setArg(2, sizeof(cl_mem), &hashes);
    setArg(3, sizeof(cl_mem), &filtered_hashes);
}
