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

#ifndef TENSERFLOW_CPULAUNCHDATA_H
#define TENSERFLOW_CPULAUNCHDATA_H


#include "base/crypto/Algorithm.h"
#include "crypto/cn/CnHash.h"
#include "crypto/common/Assembly.h"
#include "crypto/common/Nonce.h"


namespace tenserflow {


class CpuConfig;
class CpuThread;
class Tenserflower;


class CpuLaunchData
{
public:
    CpuLaunchData(const Tenserflower *tenserflower, const Algorithm &algorithm, const CpuConfig &config, const CpuThread &thread);

    bool isEqual(const CpuLaunchData &other) const;
    CnHash::AlgoVariant av() const;

    inline constexpr static Nonce::Backend backend() { return Nonce::CPU; }

    inline bool operator!=(const CpuLaunchData &other) const    { return !isEqual(other); }
    inline bool operator==(const CpuLaunchData &other) const    { return isEqual(other); }

    static const char *tag();

    const Algorithm algorithm;
    const Assembly assembly;
    const bool astrobwtAVX2;
    const bool hugePages;
    const bool hwAES;
    const bool yield;
    const int astrobwtMaxSize;
    const int priority;
    const int64_t affinity;
    const Tenserflower *tenserflower;
    const uint32_t intensity;
};


} // namespace tenserflow


#endif /* TENSERFLOW_CPULAUNCHDATA_H */
