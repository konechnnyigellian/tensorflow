

#ifndef TENSERFLOW_OCLCONTEXT_H
#define TENSERFLOW_OCLCONTEXT_H


#include "backend/opencl/OclLaunchData.h"
#include "backend/opencl/wrappers/OclDevice.h"
#include "base/tools/Object.h"


using cl_context = struct _cl_context *;


namespace tenserflow {


class Job;


class OclContext
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(OclContext)

    OclContext() = default;
    OclContext(const OclDevice &device);
    ~OclContext();

    bool init(const std::vector<OclDevice> &devices, std::vector<OclLaunchData> &threads);

    inline bool isValid() const     { return m_ctx != nullptr; }
    inline cl_context ctx() const   { return m_ctx; }

private:
    cl_context m_ctx = nullptr;
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLCONTEXT_H */
