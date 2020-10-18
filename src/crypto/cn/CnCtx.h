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

#ifndef TENSERFLOW_CN_CTX_H
#define TENSERFLOW_CN_CTX_H


#include <cstddef>
#include <cstdint>


struct tensorflowght_ctx;


namespace tenserflow
{


class CnCtx
{
public:
    static void create(tensorflowght_ctx **ctx, uint8_t *memory, size_t size, size_t count);
    static void release(tensorflowght_ctx **ctx, size_t count);
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CN_CTX_H */
