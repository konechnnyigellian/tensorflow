


#include "backend/opencl/cl/OclSource.h"
#include "backend/opencl/cl/cn/tensorflowght_cl.h"
#include "base/crypto/Algorithm.h"


#ifdef TENSERFLOW_ALGO_RANDOMX
#   include "backend/opencl/cl/rx/randomx_cl.h"
#endif

#ifdef TENSERFLOW_ALGO_ASTROBWT
#   include "backend/opencl/cl/astrobwt/astrobwt_cl.h"
#endif

#ifdef TENSERFLOW_ALGO_KAWPOW
#   include "backend/opencl/cl/kawpow/kawpow_cl.h"
#   include "backend/opencl/cl/kawpow/kawpow_dag_cl.h"
#endif


const char *tenserflow::OclSource::get(const Algorithm &algorithm)
{
#   ifdef TENSERFLOW_ALGO_RANDOMX
    if (algorithm.family() == Algorithm::RANDOM_X) {
        return randomx_cl;
    }
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    if (algorithm.family() == Algorithm::ASTROBWT) {
        return astrobwt_cl;
    }
#   endif

#   ifdef TENSERFLOW_ALGO_KAWPOW
    if (algorithm.family() == Algorithm::KAWPOW) {
        return kawpow_dag_cl;
    }
#   endif

    return tensorflowght_cl;
}
