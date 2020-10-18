


#include "backend/opencl/kernels/Cn2Kernel.h"
#include "backend/opencl/wrappers/OclLib.h"


void tenserflow::Cn2Kernel::enqueue(cl_command_queue queue, uint32_t nonce, size_t threads)
{
    const size_t offset[2]          = { nonce, 1 };
    const size_t gthreads[2]        = { threads, 8 };
    static const size_t lthreads[2] = { 8, 8 };

    enqueueNDRange(queue, 2, offset, gthreads, lthreads);
}


// __kernel void cn2(__global uint4 *Scratchpad, __global ulong *states, __global uint *Branch0, __global uint *Branch1, __global uint *Branch2, __global uint *Branch3, uint Threads)
void tenserflow::Cn2Kernel::setArgs(cl_mem scratchpads, cl_mem states, const std::vector<cl_mem> &branches, uint32_t threads)
{
    setArg(0, sizeof(cl_mem), &scratchpads);
    setArg(1, sizeof(cl_mem), &states);
    setArg(6, sizeof(uint32_t), &threads);

    for (uint32_t i = 0; i < branches.size(); ++i) {
        setArg(i + 2, sizeof(cl_mem), &branches[i]);
    }
}
