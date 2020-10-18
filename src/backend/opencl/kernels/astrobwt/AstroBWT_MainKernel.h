

#ifndef TENSERFLOW_ASTROBWT_MAINKERNEL_H
#define TENSERFLOW_ASTROBWT_MAINKERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class AstroBWT_MainKernel : public OclKernel
{
public:
    inline AstroBWT_MainKernel(cl_program program) : OclKernel(program, "BWT") {}

    void enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size);
    void setArgs(cl_mem inputs, cl_mem input_sizes, uint32_t input_stride, cl_mem indices, cl_mem tmp_indices);
};


} // namespace tenserflow


#endif /* TENSERFLOW_ASTROBWT_MAINKERNEL_H */
