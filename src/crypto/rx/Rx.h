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
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_RX_H
#define TENSERFLOW_RX_H


#include <cstdint>
#include <utility>
#include <vector>


#include "crypto/common/HugePagesInfo.h"


namespace tenserflow
{


class Algorithm;
class CpuConfig;
class CpuThread;
class IRxListener;
class Job;
class RxConfig;
class RxDataset;


class Rx
{
public:
    static HugePagesInfo hugePages();
    static RxDataset *dataset(const Job &job, uint32_t nodeId);
    static void destroy();
    static void init(IRxListener *listener);
    template<typename T> static bool init(const T &seed, const RxConfig &config, const CpuConfig &cpu);
    template<typename T> static bool isReady(const T &seed);

#   ifdef TENSERFLOW_FIX_RYZEN
    static void setMainLoopBounds(const std::pair<const void*, const void*>& bounds);
#   endif

private:
    static void msrInit(const RxConfig &config, const std::vector<CpuThread>& threads);
    static void msrDestroy();
    static void setupMainLoopExceptionFrame();
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_RX_H */
