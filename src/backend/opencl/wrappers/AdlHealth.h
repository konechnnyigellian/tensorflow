/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_ADLHEALTH_H
#define TENSERFLOW_ADLHEALTH_H


#include <cstdint>
#include <vector>


struct AdlHealth
{
    uint32_t clock          = 0;
    uint32_t memClock       = 0;
    uint32_t power          = 0;
    uint32_t rpm            = 0;
    uint32_t temperature    = 0;
};


#endif /* TENSERFLOW_ADLHEALTH_H */
