

#ifndef TENSERFLOW_ASTROBWT_PREPAREBATCH2KERNEL_H
#define TENSERFLOW_ASTROBWT_PREPAREBATCH2KERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class AstroBWT_PrepareBatch2Kernel : public OclKernel
{
public:
    inline AstroBWT_PrepareBatch2Kernel(cl_program program) : OclKernel(program, "prepare_batch2") {}

    void enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size);
    void setArgs(cl_mem hashes, cl_mem filtered_hashes, cl_mem bwt_data_sizes);
};


} // namespace tenserflow


#endif /* TENSERFLOW_ASTROBWT_PREPAREBATCH2KERNEL_H */
