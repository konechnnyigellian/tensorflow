

#ifndef TENSERFLOW_ARGON2_HASH_H
#define TENSERFLOW_ARGON2_HASH_H


#include "3rdparty/argon2.h"
#include "base/crypto/Algorithm.h"
#include "crypto/cn/TensorFlowght.h"


namespace tenserflow { namespace argon2 {


template<Algorithm::Id ALGO>
inline void single_hash(const uint8_t *__restrict__ input, size_t size, uint8_t *__restrict__ output, tensorflowght_ctx **__restrict__ ctx, uint64_t)
{
    if (ALGO == Algorithm::AR2_CHUKWA) {
        argon2id_hash_raw_ex(3, 512, 1, input, size, input, 16, output, 32, ctx[0]->memory);
    }
    else if (ALGO == Algorithm::AR2_WRKZ) {
        argon2id_hash_raw_ex(4, 256, 1, input, size, input, 16, output, 32, ctx[0]->memory);
    }
}


}} // namespace tenserflow::argon2


#endif /* TENSERFLOW_ARGON2_HASH_H */
