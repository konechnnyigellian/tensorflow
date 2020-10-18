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

#ifndef TENSERFLOW_OCLWORKER_H
#define TENSERFLOW_OCLWORKER_H


#include "backend/common/Worker.h"
#include "backend/common/WorkerJob.h"
#include "backend/opencl/OclLaunchData.h"
#include "base/tools/Object.h"
#include "net/JobResult.h"


namespace tenserflow {


class IOclRunner;
class Job;


class OclWorker : public Worker
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(OclWorker)

    OclWorker(size_t id, const OclLaunchData &data);

    ~OclWorker() override;

    void jobEarlyNotification(const Job&) override;

    static std::atomic<bool> ready;

protected:
    bool selfTest() override;
    size_t intensity() const override;
    void start() override;

private:
    bool consumeJob();
    void storeStats(uint64_t ts);

    const Algorithm m_algorithm;
    const Tenserflower *m_tenserflower;
    const uint32_t m_intensity;
    IOclRunner *m_runner = nullptr;
    OclSharedData &m_sharedData;
    WorkerJob<1> m_job;
    uint32_t m_deviceIndex;
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLWORKER_H */
