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
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_RX_NUMASTORAGE_H
#define TENSERFLOW_RX_NUMASTORAGE_H


#include "backend/common/interfaces/IRxStorage.h"
#include "base/tools/Object.h"


#include <vector>


namespace tenserflow
{


class RxNUMAStoragePrivate;


class RxNUMAStorage : public IRxStorage
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(RxNUMAStorage);

    RxNUMAStorage(const std::vector<uint32_t> &nodeset);
    ~RxNUMAStorage() override;

protected:
    bool isAllocated() const override;
    HugePagesInfo hugePages() const override;
    RxDataset *dataset(const Job &job, uint32_t nodeId) const override;
    void init(const RxSeed &seed, uint32_t threads, bool hugePages, bool oneGbPages, RxConfig::Mode mode, int priority) override;

private:
    RxNUMAStoragePrivate *d_ptr;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_RX_NUMASTORAGE_H */
