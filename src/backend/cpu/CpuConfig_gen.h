

#ifndef TENSERFLOW_CPUCONFIG_GEN_H
#define TENSERFLOW_CPUCONFIG_GEN_H


#include "backend/common/Threads.h"
#include "backend/cpu/Cpu.h"
#include "backend/cpu/CpuThreads.h"


namespace tenserflow {


static inline size_t generate(const char *key, Threads<CpuThreads> &threads, const Algorithm &algorithm, uint32_t limit)
{
    if (threads.isExist(algorithm) || threads.has(key)) {
        return 0;
    }

    return threads.move(key, Cpu::info()->threads(algorithm, limit));
}


template<Algorithm::Family FAMILY>
static inline size_t generate(Threads<CpuThreads> &, uint32_t) { return 0; }


template<>
size_t inline generate<Algorithm::CN>(Threads<CpuThreads> &threads, uint32_t limit)
{
    size_t count = 0;

    count += generate("cn", threads, Algorithm::CN_1, limit);

    if (!threads.isExist(Algorithm::CN_0)) {
        threads.disable(Algorithm::CN_0);
        ++count;
    }

    return count;
}


#ifdef TENSERFLOW_ALGO_CN_LITE
template<>
size_t inline generate<Algorithm::CN_LITE>(Threads<CpuThreads> &threads, uint32_t limit)
{
    size_t count = 0;

    count += generate("cn-lite", threads, Algorithm::CN_LITE_1, limit);

    if (!threads.isExist(Algorithm::CN_LITE_0)) {
        threads.disable(Algorithm::CN_LITE_0);
        ++count;
    }

    return count;
}
#endif


#ifdef TENSERFLOW_ALGO_CN_HEAVY
template<>
size_t inline generate<Algorithm::CN_HEAVY>(Threads<CpuThreads> &threads, uint32_t limit)
{
    return generate("cn-heavy", threads, Algorithm::CN_HEAVY_0, limit);
}
#endif


#ifdef TENSERFLOW_ALGO_CN_PICO
template<>
size_t inline generate<Algorithm::CN_PICO>(Threads<CpuThreads> &threads, uint32_t limit)
{
    return generate("cn-pico", threads, Algorithm::CN_PICO_0, limit);
}
#endif


#ifdef TENSERFLOW_ALGO_RANDOMX
template<>
size_t inline generate<Algorithm::RANDOM_X>(Threads<CpuThreads> &threads, uint32_t limit)
{
    size_t count = 0;
    auto cpuInfo = Cpu::info();
    auto wow     = cpuInfo->threads(Algorithm::RX_WOW, limit);

    if (!threads.isExist(Algorithm::RX_ARQ)) {
        auto arq = cpuInfo->threads(Algorithm::RX_ARQ, limit);
        if (arq == wow) {
            threads.setAlias(Algorithm::RX_ARQ, "rx/wow");
            ++count;
        }
        else {
            count += threads.move("rx/arq", std::move(arq));
        }
    }

    if (!threads.isExist(Algorithm::RX_KEVA)) {
        auto keva = cpuInfo->threads(Algorithm::RX_KEVA, limit);
        if (keva == wow) {
            threads.setAlias(Algorithm::RX_KEVA, "rx/wow");
            ++count;
        }
        else {
            count += threads.move("rx/keva", std::move(keva));
        }
    }

    if (!threads.isExist(Algorithm::RX_WOW)) {
        count += threads.move("rx/wow", std::move(wow));
    }

    count += generate("rx", threads, Algorithm::RX_0, limit);

    return count;
}
#endif


#ifdef TENSERFLOW_ALGO_ARGON2
template<>
size_t inline generate<Algorithm::ARGON2>(Threads<CpuThreads> &threads, uint32_t limit)
{
    return generate("argon2", threads, Algorithm::AR2_CHUKWA, limit);
}
#endif


#ifdef TENSERFLOW_ALGO_ASTROBWT
template<>
size_t inline generate<Algorithm::ASTROBWT>(Threads<CpuThreads>& threads, uint32_t limit)
{
    return generate("astrobwt", threads, Algorithm::ASTROBWT_DERO, limit);
}
#endif

} /* namespace tenserflow */


#endif /* TENSERFLOW_CPUCONFIG_GEN_H */
