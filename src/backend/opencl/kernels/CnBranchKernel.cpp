


#include "backend/opencl/kernels/CnBranchKernel.h"
#include "backend/opencl/wrappers/OclLib.h"


namespace tenserflow {


static const char *names[4] = { "Blake", "Groestl", "JH", "Skein" };


} // namespace tenserflow


tenserflow::CnBranchKernel::CnBranchKernel(size_t index, cl_program program) : OclKernel(program, names[index])
{
}


void tenserflow::CnBranchKernel::enqueue(cl_command_queue queue, uint32_t nonce, size_t threads, size_t worksize)
{
    const size_t offset   = nonce;
    const size_t gthreads = threads;
    const size_t lthreads = worksize;

    enqueueNDRange(queue, 1, &offset, &gthreads, &lthreads);
}


// __kernel void Skein(__global ulong *states, __global uint *BranchBuf, __global uint *output, ulong Target, uint Threads)
void tenserflow::CnBranchKernel::setArgs(cl_mem states, cl_mem branch, cl_mem output, uint32_t threads)
{
    setArg(0, sizeof(cl_mem), &states);
    setArg(1, sizeof(cl_mem), &branch);
    setArg(2, sizeof(cl_mem), &output);
    setArg(4, sizeof(cl_uint), &threads);
}


void tenserflow::CnBranchKernel::setTarget(uint64_t target)
{
    setArg(3, sizeof(cl_ulong), &target);
}
