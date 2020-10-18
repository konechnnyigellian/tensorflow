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

#ifndef TENSERFLOW_BACKEND_TAGS_H
#define TENSERFLOW_BACKEND_TAGS_H


#include <cstdint>


namespace tenserflow {


const char *backend_tag(uint32_t backend);
const char *cpu_tag();


#ifdef TENSERFLOW_FEATURE_OPENCL
const char *ocl_tag();
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
const char *cuda_tag();
#endif


} // namespace tenserflow


#endif /* TENSERFLOW_BACKEND_TAGS_H */
