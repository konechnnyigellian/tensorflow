

#ifndef TENSERFLOW_OCLCONFIG_GEN_H
#define TENSERFLOW_OCLCONFIG_GEN_H


#include "backend/common/Threads.h"
#include "backend/opencl/OclThreads.h"


#include <algorithm>


namespace tenserflow {


static inline size_t generate(const char *key, Threads<OclThreads> &threads, const Algorithm &algorithm, const std::vector<OclDevice> &devices)
{
    if (threads.isExist(algorithm) || threads.has(key)) {
        return 0;
    }

    return threads.move(key, OclThreads(devices, algorithm));
}


template<Algorithm::Family FAMILY>
static inline size_t generate(Threads<OclThreads> &, const std::vector<OclDevice> &) { return 0; }


template<>
size_t inline generate<Algorithm::CN>(Threads<OclThreads> &threads, const std::vector<OclDevice> &devices)
{
    size_t count = 0;

    count += generate("cn", threads, Algorithm::CN_1, devices);
    count += generate("cn/2", threads, Algorithm::CN_2, devices);

    if (!threads.isExist(Algorithm::CN_0)) {
        threads.disable(Algorithm::CN_0);
        count++;
    }

    return count;
}


#ifdef TENSERFLOW_ALGO_CN_LITE
template<>
size_t inline generate<Algorithm::CN_LITE>(Threads<OclThreads> &threads, const std::vector<OclDevice> &devices)
{
    size_t count = generate("cn-lite", threads, Algorithm::CN_LITE_1, devices);

    if (!threads.isExist(Algorithm::CN_LITE_0)) {
        threads.disable(Algorithm::CN_LITE_0);
        ++count;
    }

    return count;
}
#endif


#ifdef TENSERFLOW_ALGO_CN_HEAVY
template<>
size_t inline generate<Algorithm::CN_HEAVY>(Threads<OclThreads> &threads, const std::vector<OclDevice> &devices)
{
    return generate("cn-heavy", threads, Algorithm::CN_HEAVY_0, devices);
}
#endif


#ifdef TENSERFLOW_ALGO_CN_PICO
template<>
size_t inline generate<Algorithm::CN_PICO>(Threads<OclThreads> &threads, const std::vector<OclDevice> &devices)
{
    return generate("cn-pico", threads, Algorithm::CN_PICO_0, devices);
}
#endif


#ifdef TENSERFLOW_ALGO_RANDOMX
template<>
size_t inline generate<Algorithm::RANDOM_X>(Threads<OclThreads> &threads, const std::vector<OclDevice> &devices)
{
    size_t count = 0;

    auto rx  = OclThreads(devices, Algorithm::RX_0);
    auto wow = OclThreads(devices, Algorithm::RX_WOW);
    auto arq = OclThreads(devices, Algorithm::RX_ARQ);

    if (!threads.isExist(Algorithm::RX_WOW) && wow != rx) {
        count += threads.move("rx/wow", std::move(wow));
    }

    if (!threads.isExist(Algorithm::RX_ARQ) && arq != rx) {
        count += threads.move("rx/arq", std::move(arq));
    }

    count += threads.move("rx", std::move(rx));

    return count;
}
#endif


#ifdef TENSERFLOW_ALGO_ASTROBWT
template<>
size_t inline generate<Algorithm::ASTROBWT>(Threads<OclThreads>& threads, const std::vector<OclDevice>& devices)
{
    return generate("astrobwt", threads, Algorithm::ASTROBWT_DERO, devices);
}
#endif


#ifdef TENSERFLOW_ALGO_KAWPOW
template<>
size_t inline generate<Algorithm::KAWPOW>(Threads<OclThreads>& threads, const std::vector<OclDevice>& devices)
{
    return generate("kawpow", threads, Algorithm::KAWPOW_RVN, devices);
}
#endif


static inline std::vector<OclDevice> filterDevices(const std::vector<OclDevice> &devices, const std::vector<uint32_t> &hints)
{
    std::vector<OclDevice> out;
    out.reserve(std::min(devices.size(), hints.size()));

    for (const auto &device  : devices) {
        auto it = std::find(hints.begin(), hints.end(), device.index());
        if (it != hints.end()) {
            out.emplace_back(device);
        }
    }

    return out;
}


} /* namespace tenserflow */


#endif /* TENSERFLOW_OCLCONFIG_GEN_H */
