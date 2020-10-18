

#ifndef TENSERFLOW_ASTROBWT_SHA3INITIALKERNEL_H
#define TENSERFLOW_ASTROBWT_SHA3INITIALKERNEL_H


#include "backend/opencl/wrappers/OclKernel.h"


namespace tenserflow {


class AstroBWT_SHA3InitialKernel : public OclKernel
{
public:
    inline AstroBWT_SHA3InitialKernel(cl_program program) : OclKernel(program, "sha3_initial") {}

    void enqueue(cl_command_queue queue, size_t threads);
    void setArgs(cl_mem input, uint32_t input_size, uint32_t nonce, cl_mem output_salsa20_keys);
};


} // namespace tenserflow


#endif /* TENSERFLOW_ASTROBWT_SHA3INITIALKERNEL_H */
