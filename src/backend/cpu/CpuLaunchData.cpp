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


#include "backend/cpu/CpuLaunchData.h"
#include "backend/common/Tags.h"
#include "backend/cpu/CpuConfig.h"


#include <algorithm>


tenserflow::CpuLaunchData::CpuLaunchData(const Tenserflower *tenserflower, const Algorithm &algorithm, const CpuConfig &config, const CpuThread &thread) :
    algorithm(algorithm),
    assembly(config.assembly()),
    astrobwtAVX2(config.astrobwtAVX2()),
    hugePages(config.isHugePages()),
    hwAES(config.isHwAES()),
    yield(config.isYield()),
    astrobwtMaxSize(config.astrobwtMaxSize()),    
    priority(config.priority()),
    affinity(thread.affinity()),
    tenserflower(tenserflower),
    intensity(std::min<uint32_t>(thread.intensity(), algorithm.maxIntensity()))
{
}


bool tenserflow::CpuLaunchData::isEqual(const CpuLaunchData &other) const
{
    return (algorithm.l3()      == other.algorithm.l3()
            && assembly         == other.assembly
            && hugePages        == other.hugePages
            && hwAES            == other.hwAES
            && intensity        == other.intensity
            && priority         == other.priority
            && affinity         == other.affinity
            );
}


tenserflow::CnHash::AlgoVariant tenserflow::CpuLaunchData::av() const
{
    if (intensity <= 2) {
        return static_cast<CnHash::AlgoVariant>(!hwAES ? (intensity + 2) : intensity);
    }

    return static_cast<CnHash::AlgoVariant>(!hwAES ? (intensity + 5) : (intensity + 2));
}


const char *tenserflow::CpuLaunchData::tag()
{
    return cpu_tag();
}
