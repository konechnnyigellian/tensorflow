

#ifndef TENSERFLOW_ASTROBWTFINDSHARESKERNEL_H
#define TENSERFLOW_ASTROBWTFINDSHARESKERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class AstroBWT_FindSharesKernel : public OclKernel
{
public:
    inline AstroBWT_FindSharesKernel(cl_program program) : OclKernel(program, "find_shares") {}

    void enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size);
    void setArgs(cl_mem hashes, cl_mem filtered_hashes, cl_mem shares);
    void setTarget(uint64_t target);
};


} // namespace tenserflow


#endif /* TENSERFLOW_ASTROBWTFINDSHARESKERNEL_H */
