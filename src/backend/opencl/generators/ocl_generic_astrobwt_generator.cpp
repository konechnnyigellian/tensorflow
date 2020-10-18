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


#include "backend/opencl/OclThreads.h"
#include "backend/opencl/wrappers/OclDevice.h"
#include "base/crypto/Algorithm.h"
#include "crypto/randomx/randomx.h"
#include "crypto/rx/RxAlgo.h"


namespace tenserflow {


bool ocl_generic_astrobwt_generator(const OclDevice &device, const Algorithm &algorithm, OclThreads &threads)
{
    if (algorithm.family() != Algorithm::ASTROBWT) {
        return false;
    }

    const size_t mem = device.globalMemSize();

    uint32_t per_thread_mem = 10 << 20;
    uint32_t intensity = static_cast<uint32_t>((mem - (128 << 20)) / per_thread_mem / 2);

    intensity &= ~63U;

    if (!intensity) {
        return false;
    }

    if (intensity > 256) {
        intensity = 256;
    }

    threads.add(OclThread(device.index(), intensity, 2));

    return true;
}


} // namespace tenserflow
