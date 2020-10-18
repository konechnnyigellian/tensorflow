

#ifndef TENSERFLOW_OCLCNR_H
#define TENSERFLOW_OCLCNR_H


#include <cstddef>
#include <cstdint>


using cl_program = struct _cl_program *;


namespace tenserflow {


class Algorithm;
class IOclRunner;


class OclCnR
{
public:
    constexpr static size_t kHeightChunkSize = 10;

    static cl_program get(const IOclRunner &runner, uint64_t height);
    static void clear();
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLCNR_H */
