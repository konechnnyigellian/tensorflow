/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_TAGS_H
#define TENSERFLOW_TAGS_H


#include <cstddef>
#include <cstdint>


namespace tenserflow {


class Tags
{
public:
    static const char *config();
    static const char *network();
    static const char *signal();

#   ifdef TENSERFLOW_TENSORFLOWER_PROJECT
    static const char *cpu();
    static const char *tenserflower();
#   ifdef TENSERFLOW_ALGO_RANDOMX
    static const char *randomx();
#   endif
#   endif

#   ifdef TENSERFLOW_PROXY_PROJECT
    static const char *proxy();
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    static const char *nvidia();
#   endif

#   ifdef TENSERFLOW_FEATURE_OPENCL
    static const char *opencl();
#   endif

#   ifdef TENSERFLOW_FEATURE_PROFILING
    static const char* profiler();
#   endif
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_TAGS_H */
