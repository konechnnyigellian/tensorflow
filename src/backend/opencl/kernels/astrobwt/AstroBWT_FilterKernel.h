

#ifndef TENSERFLOW_ASTROBWT_FILTERKERNEL_H
#define TENSERFLOW_ASTROBWT_FILTERKERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class AstroBWT_FilterKernel : public OclKernel
{
public:
    inline AstroBWT_FilterKernel(cl_program program) : OclKernel(program, "filter") {}

    void enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size);
    void setArgs(uint32_t nonce, uint32_t bwt_max_size, cl_mem hashes, cl_mem filtered_hashes);
};


} // namespace tenserflow


#endif /* TENSERFLOW_ASTROBWT_FILTERKERNEL_H */
