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

#ifndef TENSERFLOW_CUDALAUNCHDATA_H
#define TENSERFLOW_CUDALAUNCHDATA_H


#include "backend/cuda/CudaThread.h"
#include "base/crypto/Algorithm.h"
#include "crypto/common/Nonce.h"


namespace tenserflow {


class CudaDevice;
class Tenserflower;


class CudaLaunchData
{
public:
    CudaLaunchData(const Tenserflower *tenserflower, const Algorithm &algorithm, const CudaThread &thread, const CudaDevice &device);

    bool isEqual(const CudaLaunchData &other) const;

    inline constexpr static Nonce::Backend backend() { return Nonce::CUDA; }

    inline bool operator!=(const CudaLaunchData &other) const    { return !isEqual(other); }
    inline bool operator==(const CudaLaunchData &other) const    { return isEqual(other); }

    static const char *tag();

    const Algorithm algorithm;
    const Tenserflower *tenserflower;
    const CudaDevice &device;
    const CudaThread thread;
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLLAUNCHDATA_H */
