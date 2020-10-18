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
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/crypto/Algorithm.h"


struct tensorflowght_ctx;


namespace tenserflow {


namespace astrobwt {

bool astrobwt_dero(const void* input_data, uint32_t input_size, void* scratchpad, uint8_t* output_hash, int stage2_max_size, bool avx2);
void init();

template<Algorithm::Id ALGO>
void single_hash(const uint8_t* input, size_t size, uint8_t* output, tensorflowght_ctx** ctx, uint64_t);

template<>
void single_hash<Algorithm::ASTROBWT_DERO>(const uint8_t* input, size_t size, uint8_t* output, tensorflowght_ctx** ctx, uint64_t);


}} // namespace tenserflow::astrobwt
