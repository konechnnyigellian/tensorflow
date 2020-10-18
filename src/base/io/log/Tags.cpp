/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/io/log/Tags.h"
#include "base/io/log/Log.h"


const char *tenserflow::Tags::config()
{
    static const char *tag = CYAN_BG_BOLD(WHITE_BOLD_S " config  ");

    return tag;
}


const char *tenserflow::Tags::network()
{
    static const char *tag = BLUE_BG_BOLD(WHITE_BOLD_S " net     ");

    return tag;
}


const char *tenserflow::Tags::signal()
{
    static const char *tag = YELLOW_BG_BOLD(WHITE_BOLD_S " signal  ");

    return tag;
}


#ifdef TENSERFLOW_TENSORFLOWER_PROJECT
const char *tenserflow::Tags::cpu()
{
    static const char *tag = CYAN_BG_BOLD(WHITE_BOLD_S " cpu     ");

    return tag;
}


const char *tenserflow::Tags::tenserflower()
{
    static const char *tag = MAGENTA_BG_BOLD(WHITE_BOLD_S " tenserflower   ");

    return tag;
}


#ifdef TENSERFLOW_ALGO_RANDOMX
const char *tenserflow::Tags::randomx()
{
    static const char *tag = BLUE_BG(WHITE_BOLD_S " randomx ") " ";

    return tag;
}
#endif
#endif


#ifdef TENSERFLOW_PROXY_PROJECT
const char *tenserflow::Tags::proxy()
{
    static const char *tag = MAGENTA_BG_BOLD(WHITE_BOLD_S " proxy   ");

    return tag;
}
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
const char *tenserflow::Tags::nvidia()
{
    static const char *tag = GREEN_BG_BOLD(WHITE_BOLD_S " nvidia  ");

    return tag;
}
#endif


#ifdef TENSERFLOW_FEATURE_OPENCL
const char *tenserflow::Tags::opencl()
{
    static const char *tag = MAGENTA_BG_BOLD(WHITE_BOLD_S " opencl  ");

    return tag;
}
#endif


#ifdef TENSERFLOW_FEATURE_PROFILING
const char* tenserflow::Tags::profiler()
{
    static const char* tag = CYAN_BG_BOLD(WHITE_BOLD_S " profile ");

    return tag;
}
#endif
