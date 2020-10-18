

#ifndef TENSERFLOW_ASTROBWT_SALSA20KERNEL_H
#define TENSERFLOW_ASTROBWT_SALSA20KERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class AstroBWT_Salsa20Kernel : public OclKernel
{
public:
    inline AstroBWT_Salsa20Kernel(cl_program program) : OclKernel(program, "Salsa20_XORKeyStream") {}

    void enqueue(cl_command_queue queue, size_t threads, size_t workgroup_size);
    void setArgs(cl_mem salsa20_keys, cl_mem outputs, cl_mem output_sizes, uint32_t output_stride);
};


} // namespace tenserflow


#endif /* TENSERFLOW_ASTROBWT_SALSA20KERNEL_H */
