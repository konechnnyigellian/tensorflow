

#ifndef TENSERFLOW_OCLSHAREDSTATE_H
#define TENSERFLOW_OCLSHAREDSTATE_H


#include "backend/opencl/OclLaunchData.h"


namespace tenserflow {


class OclSharedState
{
public:
    static OclSharedData &get(uint32_t index);
    static void release();
    static void start(const std::vector<OclLaunchData> &threads, const Job &job);
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_OCLSHAREDSTATE_H */
