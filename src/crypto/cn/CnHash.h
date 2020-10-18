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

#ifndef TENSERFLOW_CN_HASH_H
#define TENSERFLOW_CN_HASH_H


#include <cstddef>
#include <cstdint>


#include "crypto/cn/CnAlgo.h"
#include "crypto/common/Assembly.h"


struct tensorflowght_ctx;


namespace tenserflow
{

using cn_hash_fun     = void (*)(const uint8_t *, size_t, uint8_t *, tensorflowght_ctx **, uint64_t);
using cn_mainloop_fun = void (*)(tensorflowght_ctx **);


class CnHash
{
public:
    enum AlgoVariant {
        AV_AUTO,        // --av=0 Automatic mode.
        AV_SINGLE,      // --av=1  Single hash mode
        AV_DOUBLE,      // --av=2  Double hash mode
        AV_SINGLE_SOFT, // --av=3  Single hash mode (Software AES)
        AV_DOUBLE_SOFT, // --av=4  Double hash mode (Software AES)
        AV_TRIPLE,      // --av=5  Triple hash mode
        AV_QUAD,        // --av=6  Quard hash mode
        AV_PENTA,       // --av=7  Penta hash mode
        AV_TRIPLE_SOFT, // --av=8  Triple hash mode (Software AES)
        AV_QUAD_SOFT,   // --av=9  Quard hash mode  (Software AES)
        AV_PENTA_SOFT,  // --av=10 Penta hash mode  (Software AES)
        AV_MAX
    };

    CnHash();

    static cn_hash_fun fn(const Algorithm &algorithm, AlgoVariant av, Assembly::Id assembly);

private:
    cn_hash_fun m_map[Algorithm::MAX][AV_MAX][Assembly::MAX] = {};
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CN_HASH_H */
