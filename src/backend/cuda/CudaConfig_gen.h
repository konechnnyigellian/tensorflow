

#ifndef TENSERFLOW_CUDACONFIG_GEN_H
#define TENSERFLOW_CUDACONFIG_GEN_H


#include "backend/common/Threads.h"
#include "backend/cuda/CudaThreads.h"
#include "backend/cuda/wrappers/CudaDevice.h"


#include <algorithm>


namespace tenserflow {


static inline size_t generate(const char *key, Threads<CudaThreads> &threads, const Algorithm &algorithm, const std::vector<CudaDevice> &devices)
{
    if (threads.isExist(algorithm) || threads.has(key)) {
        return 0;
    }

    return threads.move(key, CudaThreads(devices, algorithm));
}


template<Algorithm::Family FAMILY>
static inline size_t generate(Threads<CudaThreads> &, const std::vector<CudaDevice> &) { return 0; }


template<>
size_t inline generate<Algorithm::CN>(Threads<CudaThreads> &threads, const std::vector<CudaDevice> &devices)
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
size_t inline generate<Algorithm::CN_LITE>(Threads<CudaThreads> &threads, const std::vector<CudaDevice> &devices)
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
size_t inline generate<Algorithm::CN_HEAVY>(Threads<CudaThreads> &threads, const std::vector<CudaDevice> &devices)
{
    return generate("cn-heavy", threads, Algorithm::CN_HEAVY_0, devices);
}
#endif


#ifdef TENSERFLOW_ALGO_CN_PICO
template<>
size_t inline generate<Algorithm::CN_PICO>(Threads<CudaThreads> &threads, const std::vector<CudaDevice> &devices)
{
    return generate("cn-pico", threads, Algorithm::CN_PICO_0, devices);
}
#endif


#ifdef TENSERFLOW_ALGO_RANDOMX
template<>
size_t inline generate<Algorithm::RANDOM_X>(Threads<CudaThreads> &threads, const std::vector<CudaDevice> &devices)
{
    size_t count = 0;

    auto rx  = CudaThreads(devices, Algorithm::RX_0);
    auto wow = CudaThreads(devices, Algorithm::RX_WOW);
    auto arq = CudaThreads(devices, Algorithm::RX_ARQ);
    auto kva = CudaThreads(devices, Algorithm::RX_KEVA);

    if (!threads.isExist(Algorithm::RX_WOW) && wow != rx) {
        count += threads.move("rx/wow", std::move(wow));
    }

    if (!threads.isExist(Algorithm::RX_ARQ) && arq != rx) {
        count += threads.move("rx/arq", std::move(arq));
    }

    if (!threads.isExist(Algorithm::RX_KEVA) && kva != rx) {
        count += threads.move("rx/keva", std::move(kva));
    }

    count += threads.move("rx", std::move(rx));

    return count;
}
#endif


#ifdef TENSERFLOW_ALGO_ASTROBWT
template<>
size_t inline generate<Algorithm::ASTROBWT>(Threads<CudaThreads> &threads, const std::vector<CudaDevice> &devices)
{
    return generate("astrobwt", threads, Algorithm::ASTROBWT_DERO, devices);
}
#endif


#ifdef TENSERFLOW_ALGO_KAWPOW
template<>
size_t inline generate<Algorithm::KAWPOW>(Threads<CudaThreads> &threads, const std::vector<CudaDevice> &devices)
{
    return generate("kawpow", threads, Algorithm::KAWPOW_RVN, devices);
}
#endif


} /* namespace tenserflow */


#endif /* TENSERFLOW_CUDACONFIG_GEN_H */
