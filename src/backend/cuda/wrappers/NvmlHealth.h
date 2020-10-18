

#ifndef TENSERFLOW_NVMLHEALTH_H
#define TENSERFLOW_NVMLHEALTH_H


#include <cstdint>
#include <vector>


struct NvmlHealth
{
    std::vector<uint32_t> fanSpeed;
    uint32_t clock          = 0;
    uint32_t memClock       = 0;
    uint32_t power          = 0;
    uint32_t temperature    = 0;
};


#endif /* TENSERFLOW_NVMLHEALTH_H */
