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
#include "crypto/cn/CnAlgo.h"


#include <algorithm>


namespace tenserflow {


constexpr const size_t oneMiB = 1024u * 1024u;


static inline bool isMatch(const OclDevice &device, const Algorithm &algorithm)
{
    return algorithm.isCN() &&
           device.vendorId() == OCL_VENDOR_AMD &&
           (device.type() == OclDevice::Vega_10 || device.type() == OclDevice::Vega_20);
}


static inline uint32_t getMaxThreads(const OclDevice &device, const Algorithm &algorithm)
{
    const uint32_t ratio = (algorithm.l3() <= oneMiB) ? 2u : 1u;

    if (device.type() == OclDevice::Vega_10) {
        if (device.computeUnits() == 56 && algorithm.family() == Algorithm::CN && CnAlgo<>::base(algorithm) == Algorithm::CN_2) {
            return 1792u;
        }
    }

    return ratio * 2024u;
}


static inline uint32_t getPossibleIntensity(const OclDevice &device, const Algorithm &algorithm)
{
    const uint32_t maxThreads   = getMaxThreads(device, algorithm);
    const size_t availableMem   = device.freeMemSize() - (128u * oneMiB);
    const size_t perThread      = algorithm.l3() + 224u;
    const auto maxIntensity     = static_cast<uint32_t>(availableMem / perThread);

    return std::min<uint32_t>(maxThreads, maxIntensity);
}


static inline uint32_t getIntensity(const OclDevice &device, const Algorithm &algorithm)
{
    const uint32_t maxIntensity = getPossibleIntensity(device, algorithm);

    if (device.type() == OclDevice::Vega_10) {
        if (algorithm.family() == Algorithm::CN_HEAVY && device.computeUnits() == 64 && maxIntensity > 976) {
            return 976;
        }
    }

    return maxIntensity / device.computeUnits() * device.computeUnits();
}


static inline uint32_t getWorksize(const Algorithm &algorithm)
{
    if (algorithm.family() == Algorithm::CN_PICO) {
        return 64;
    }

    if (CnAlgo<>::base(algorithm) == Algorithm::CN_2) {
        return 16;
    }

    return 8;
}


static uint32_t getStridedIndex(const Algorithm &algorithm)
{
    return CnAlgo<>::base(algorithm) == Algorithm::CN_2 ? 2 : 1;
}


static inline uint32_t getMemChunk(const Algorithm &algorithm)
{
    return CnAlgo<>::base(algorithm) == Algorithm::CN_2 ? 1 : 2;
}


bool ocl_vega_cn_generator(const OclDevice &device, const Algorithm &algorithm, OclThreads &threads)
{
    if (!isMatch(device, algorithm)) {
        return false;
    }

    const uint32_t intensity = getIntensity(device, algorithm);
    if (intensity == 0) {
        return false;
    }

    const uint32_t worksize = getWorksize(algorithm);
    const uint32_t memChunk = getMemChunk(algorithm);

    threads.add(OclThread(device.index(), intensity, worksize, getStridedIndex(algorithm), memChunk, 2, 8));

    return true;
}


} // namespace tenserflow
