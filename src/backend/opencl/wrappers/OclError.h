

#ifndef TENSERFLOW_OCLERROR_H
#define TENSERFLOW_OCLERROR_H


#include "3rdparty/cl.h"


#define OCL_ERR_SUCCESS    (0)
#define OCL_ERR_API        (2)
#define OCL_ERR_BAD_PARAMS (1)


namespace tenserflow {


class OclError
{
public:
    static const char *toString(cl_int ret);
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLERROR_H */
