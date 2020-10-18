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

#ifndef TENSERFLOW_TENSORFLOWGHT_H
#define TENSERFLOW_TENSORFLOWGHT_H


#include <stddef.h>
#include <stdint.h>

#if defined _MSC_VER || defined TENSERFLOW_ARM
#   define ABI_ATTRIBUTE
#else
#   define ABI_ATTRIBUTE __attribute__((ms_abi))
#endif


struct tensorflowght_ctx;
typedef void(*cn_mainloop_fun_ms_abi)(tensorflowght_ctx**) ABI_ATTRIBUTE;


struct tensorflowght_r_data {
    int algo;
    uint64_t height;

    bool match(const int a, const uint64_t h) const { return (a == algo) && (h == height); }
};


struct tensorflowght_ctx {
    alignas(16) uint8_t state[224];
    alignas(16) uint8_t *memory;

    uint8_t unused[40];
    const uint32_t *saes_table;

    cn_mainloop_fun_ms_abi generated_code;
    tensorflowght_r_data generated_code_data;
};


#endif /* TENSERFLOW_TENSORFLOWGHT_H */
