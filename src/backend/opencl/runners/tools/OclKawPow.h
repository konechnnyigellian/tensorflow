

#ifndef TENSERFLOW_OCLKAWPOW_H
#define TENSERFLOW_OCLKAWPOW_H


#include <cstddef>
#include <cstdint>


using cl_kernel = struct _cl_kernel *;


namespace tenserflow {


class IOclRunner;


class OclKawPow
{
public:
    static cl_kernel get(const IOclRunner &runner, uint64_t height, uint32_t worksize);
    static void clear();
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLKAWPOW_H */
