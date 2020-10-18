

#ifndef TENSERFLOW_ASTROBWT_SHA3KERNEL_H
#define TENSERFLOW_ASTROBWT_SHA3KERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class AstroBWT_SHA3Kernel : public OclKernel
{
public:
    inline AstroBWT_SHA3Kernel(cl_program program) : OclKernel(program, "sha3") {}

    void enqueue(cl_command_queue queue, size_t threads);
    void setArgs(cl_mem inputs, cl_mem input_sizes, uint32_t input_stride, cl_mem output_salsa20_keys);
};


} // namespace tenserflow


#endif /* TENSERFLOW_ASTROBWT_SHA3KERNEL_H */
