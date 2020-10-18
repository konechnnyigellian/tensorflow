


#include "backend/opencl/runners/tools/OclSharedState.h"
#include "backend/opencl/runners/tools/OclSharedData.h"


#include <cassert>
#include <map>


namespace tenserflow {


static std::map<uint32_t, OclSharedData> map;


} // namespace tenserflow


tenserflow::OclSharedData &tenserflow::OclSharedState::get(uint32_t index)
{
    return map[index];
}


void tenserflow::OclSharedState::release()
{
    for (auto &kv : map) {
        kv.second.release();
    }

    map.clear();
}


void tenserflow::OclSharedState::start(const std::vector<OclLaunchData> &threads, const Job &job)
{
    assert(map.empty());

    for (const auto &data : threads) {
        auto &sharedData = map[data.device.index()];

        ++sharedData;

#       ifdef TENSERFLOW_ALGO_RANDOMX
        if (data.algorithm.family() == Algorithm::RANDOM_X) {
            sharedData.createDataset(data.ctx, job, data.thread.isDatasetHost());
        }
#       endif
    }
}
