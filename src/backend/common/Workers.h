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

#ifndef TENSERFLOW_WORKERS_H
#define TENSERFLOW_WORKERS_H


#include "backend/common/Thread.h"
#include "backend/cpu/CpuLaunchData.h"
#include "base/tools/Object.h"


#ifdef TENSERFLOW_FEATURE_OPENCL
#   include "backend/opencl/OclLaunchData.h"
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
#   include "backend/cuda/CudaLaunchData.h"
#endif


namespace tenserflow {


class Hashrate;
class WorkersPrivate;
class Job;


template<class T>
class Workers
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(Workers)

    Workers();
    ~Workers();

    const Hashrate *hashrate() const;
    void setBackend(IBackend *backend);
    void start(const std::vector<T> &data);
    void stop();
    void tick(uint64_t ticks);
    void jobEarlyNotification(const Job&);

private:
    static IWorker *create(Thread<T> *handle);
    static void onReady(void *arg);

    std::vector<Thread<T> *> m_workers;
    WorkersPrivate *d_ptr;
};


template<class T>
void tenserflow::Workers<T>::jobEarlyNotification(const Job& job)
{
    for (Thread<T>* t : m_workers) {
        if (t->worker()) {
            t->worker()->jobEarlyNotification(job);
        }
    }
}


template<>
IWorker *Workers<CpuLaunchData>::create(Thread<CpuLaunchData> *handle);
extern template class Workers<CpuLaunchData>;


#ifdef TENSERFLOW_FEATURE_OPENCL
template<>
IWorker *Workers<OclLaunchData>::create(Thread<OclLaunchData> *handle);
extern template class Workers<OclLaunchData>;
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
template<>
IWorker *Workers<CudaLaunchData>::create(Thread<CudaLaunchData> *handle);
extern template class Workers<CudaLaunchData>;
#endif


} // namespace tenserflow


#endif /* TENSERFLOW_WORKERS_H */
