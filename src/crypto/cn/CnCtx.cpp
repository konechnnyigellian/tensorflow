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

#include <limits>


#include "crypto/cn/CnCtx.h"
#include "base/crypto/Algorithm.h"
#include "crypto/cn/TensorFlowght.h"
#include "crypto/common/portable/mm_malloc.h"
#include "crypto/common/VirtualMemory.h"


void tenserflow::CnCtx::create(tensorflowght_ctx **ctx, uint8_t *memory, size_t size, size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        tensorflowght_ctx *c = static_cast<tensorflowght_ctx *>(_mm_malloc(sizeof(tensorflowght_ctx), 4096));
        c->memory          = memory + (i * size);

        c->generated_code              = reinterpret_cast<cn_mainloop_fun_ms_abi>(VirtualMemory::allocateExecutableMemory(0x4000));
        c->generated_code_data.algo    = Algorithm::INVALID;
        c->generated_code_data.height  = std::numeric_limits<uint64_t>::max();

        ctx[i] = c;
    }
}


void tenserflow::CnCtx::release(tensorflowght_ctx **ctx, size_t count)
{
    if (ctx[0] == nullptr) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        _mm_free(ctx[i]);
    }
}
