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


#include "backend/opencl/OclLaunchData.h"
#include "backend/common/Tags.h"
#include "backend/opencl/OclConfig.h"


tenserflow::OclLaunchData::OclLaunchData(const Tenserflower *tenserflower, const Algorithm &algorithm, const OclConfig &config, const OclPlatform &platform, const OclThread &thread, const OclDevice &device, int64_t affinity) :
    algorithm(algorithm),
    cache(config.isCacheEnabled()),
    affinity(affinity),
    tenserflower(tenserflower),
    device(device),
    platform(platform),
    thread(thread)
{
}


bool tenserflow::OclLaunchData::isEqual(const OclLaunchData &other) const
{
    return (other.algorithm == algorithm &&
            other.thread    == thread);
}


const char *tenserflow::OclLaunchData::tag()
{
    return ocl_tag();
}
