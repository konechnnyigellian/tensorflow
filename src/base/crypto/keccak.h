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


#ifndef TENSERFLOW_KECCAK_H
#define TENSERFLOW_KECCAK_H

#include <cstdint>
#include <cstring>


namespace tenserflow {

// compute a keccak hash (md) of given byte length from "in"
void keccak(const uint8_t *in, int inlen, uint8_t *md, int mdlen);


inline void keccak(const uint8_t *in, size_t inlen, uint8_t *md)
{
    keccak(in, static_cast<int>(inlen), md, 200);
}


inline void keccak(const char *in, size_t inlen, uint8_t *md)
{
    keccak(reinterpret_cast<const uint8_t *>(in), static_cast<int>(inlen), md, 200);
}

// update the state
void keccakf(uint64_t st[25], int norounds);

} /* namespace tenserflow */

#endif /* TENSERFLOW_KECCAK_H */
