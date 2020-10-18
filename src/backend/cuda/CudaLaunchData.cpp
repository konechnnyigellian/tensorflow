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


#include "backend/cuda/CudaLaunchData.h"
#include "backend/common/Tags.h"


tenserflow::CudaLaunchData::CudaLaunchData(const Tenserflower *tenserflower, const Algorithm &algorithm, const CudaThread &thread, const CudaDevice &device) :
    algorithm(algorithm),
    tenserflower(tenserflower),
    device(device),
    thread(thread)
{
}


bool tenserflow::CudaLaunchData::isEqual(const CudaLaunchData &other) const
{
    return (other.algorithm.family() == algorithm.family() &&
            other.algorithm.l3()     == algorithm.l3() &&
            other.thread             == thread);
}


const char *tenserflow::CudaLaunchData::tag()
{
    return cuda_tag();
}
