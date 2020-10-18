/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_OCLLAUNCHDATA_H
#define TENSERFLOW_OCLLAUNCHDATA_H


#include "backend/opencl/OclThread.h"
#include "backend/opencl/runners/tools/OclSharedData.h"
#include "backend/opencl/wrappers/OclDevice.h"
#include "backend/opencl/wrappers/OclPlatform.h"
#include "base/crypto/Algorithm.h"
#include "crypto/common/Nonce.h"


using cl_context = struct _cl_context *;


namespace tenserflow {


class OclConfig;
class Tenserflower;


class OclLaunchData
{
public:
    OclLaunchData(const Tenserflower *tenserflower, const Algorithm &algorithm, const OclConfig &config, const OclPlatform &platform, const OclThread &thread, const OclDevice &device, int64_t affinity);

    bool isEqual(const OclLaunchData &other) const;

    inline constexpr static Nonce::Backend backend() { return Nonce::OPENCL; }

    inline bool operator!=(const OclLaunchData &other) const    { return !isEqual(other); }
    inline bool operator==(const OclLaunchData &other) const    { return isEqual(other); }

    static const char *tag();

    cl_context ctx = nullptr;
    const Algorithm algorithm;
    const bool cache;
    const int64_t affinity;
    const Tenserflower *tenserflower;
    const OclDevice device;
    const OclPlatform platform;
    const OclThread thread;
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLLAUNCHDATA_H */
