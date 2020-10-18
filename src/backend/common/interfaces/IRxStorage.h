/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_IRXSTORAGE_H
#define TENSERFLOW_IRXSTORAGE_H


#include "crypto/rx/RxConfig.h"
#include "crypto/common/HugePagesInfo.h"


#include <cstdint>
#include <utility>


namespace tenserflow {


class Job;
class RxDataset;
class RxSeed;


class IRxStorage
{
public:
    virtual ~IRxStorage() = default;

    virtual bool isAllocated() const                                                                                            = 0;
    virtual HugePagesInfo hugePages() const                                                                                     = 0;
    virtual RxDataset *dataset(const Job &job, uint32_t nodeId) const                                                           = 0;
    virtual void init(const RxSeed &seed, uint32_t threads, bool hugePages, bool oneGbPages, RxConfig::Mode mode, int priority) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IRXSTORAGE_H
