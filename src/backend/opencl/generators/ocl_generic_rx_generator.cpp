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


bool ocl_generic_rx_generator(const OclDevice &device, const Algorithm &algorithm, OclThreads &threads)
{
    if (algorithm.family() != Algorithm::RANDOM_X) {
        return false;
    }

    // Mobile Ryzen APUs
    if (device.type() == OclDevice::Raven) {
        threads.add(OclThread(device.index(), (device.computeUnits() > 4) ? 256 : 128, 8, 1, true, true, 6));
        return true;
    }

    const size_t mem = device.globalMemSize();
    auto config      = RxAlgo::base(algorithm);
    bool gcnAsm      = false;
    bool isNavi      = false;

    switch (device.type()) {
    case OclDevice::Baffin:
    case OclDevice::Polaris:
    case OclDevice::Lexa:
    case OclDevice::Vega_10:
    case OclDevice::Vega_20:
        gcnAsm = true;
        break;

    case OclDevice::Navi_10:
    case OclDevice::Navi_12:
    case OclDevice::Navi_14:
        gcnAsm = true;
        isNavi = true;
        break;

    default:
        break;
    }

    // Must have space for dataset, scratchpads and 128 MB of free memory
    const uint32_t dataset_mem = config->DatasetBaseSize + config->DatasetExtraSize + (128U << 20);

    // Use dataset on host if not enough memory
    bool datasetHost = (mem < dataset_mem);

    // Each thread uses 1 scratchpad plus a few small buffers on GPU
    const uint32_t per_thread_mem = config->ScratchpadL3_Size + 32768;

    uint32_t intensity = static_cast<uint32_t>((mem - (datasetHost ? 0 : dataset_mem)) / per_thread_mem / 2);

    // Too high intensity makes hashrate worse
    const uint32_t intensityCoeff = isNavi ? 64 : 16;
    if (intensity > device.computeUnits() * intensityCoeff) {
        intensity = device.computeUnits() * intensityCoeff;
    }

    intensity -= intensity % 64;

    // If there are too few threads, use dataset on host to get more threads
    if (intensity < device.computeUnits() * 4) {
        datasetHost = true;
        intensity = static_cast<uint32_t>(mem / per_thread_mem / 2);
        intensity -= intensity % 64;
    }

    if (!intensity) {
        return false;
    }

    threads.add(OclThread(device.index(), intensity, 8, device.vendorId() == OCL_VENDOR_AMD ? 2 : 1, gcnAsm, datasetHost, 6));

    return true;
}


} // namespace tenserflow
