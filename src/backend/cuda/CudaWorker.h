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

#ifndef TENSERFLOW_CUDAWORKER_H
#define TENSERFLOW_CUDAWORKER_H


#include "backend/common/Worker.h"
#include "backend/common/WorkerJob.h"
#include "backend/cuda/CudaLaunchData.h"
#include "base/tools/Object.h"
#include "net/JobResult.h"


namespace tenserflow {


class ICudaRunner;


class CudaWorker : public Worker
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(CudaWorker)

    CudaWorker(size_t id, const CudaLaunchData &data);

    ~CudaWorker() override;

    void jobEarlyNotification(const Job&) override;

    static std::atomic<bool> ready;

protected:
    bool selfTest() override;
    size_t intensity() const override;
    void start() override;

private:
    bool consumeJob();
    void storeStats();

    const Algorithm m_algorithm;
    const Tenserflower *m_tenserflower;
    ICudaRunner *m_runner = nullptr;
    WorkerJob<1> m_job;
    uint32_t m_deviceIndex;
};


} // namespace tenserflow


#endif /* TENSERFLOW_CUDAWORKER_H */
