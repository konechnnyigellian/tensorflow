


#include "backend/opencl/kernels/astrobwt/AstroBWT_PrepareBatch2Kernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::AstroBWT_PrepareBatch2Kernel::enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size)
{
    enqueueNDRange(queue, 1, nullptr, &threads, &workgroup_size);
}


void tenserflow::AstroBWT_PrepareBatch2Kernel::setArgs(cl_mem hashes, cl_mem filtered_hashes, cl_mem bwt_data_sizes)
{
    setArg(0, sizeof(cl_mem), &hashes);
    setArg(1, sizeof(cl_mem), &filtered_hashes);
    setArg(2, sizeof(cl_mem), &bwt_data_sizes);
}
