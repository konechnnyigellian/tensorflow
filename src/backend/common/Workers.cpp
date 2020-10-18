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


#include "backend/common/Hashrate.h"
#include "backend/common/interfaces/IBackend.h"
#include "backend/common/Workers.h"
#include "backend/cpu/CpuWorker.h"
#include "base/io/log/Log.h"
#include "base/tools/Object.h"


#ifdef TENSERFLOW_FEATURE_OPENCL
#   include "backend/opencl/OclWorker.h"
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
#   include "backend/cuda/CudaWorker.h"
#endif


namespace tenserflow {


class WorkersPrivate
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(WorkersPrivate)


    WorkersPrivate() = default;


    inline ~WorkersPrivate()
    {
        delete hashrate;
    }


    Hashrate *hashrate = nullptr;
    IBackend *backend  = nullptr;
};


} // namespace tenserflow


template<class T>
tenserflow::Workers<T>::Workers() :
    d_ptr(new WorkersPrivate())
{

}


template<class T>
tenserflow::Workers<T>::~Workers()
{
    delete d_ptr;
}


template<class T>
const tenserflow::Hashrate *tenserflow::Workers<T>::hashrate() const
{
    return d_ptr->hashrate;
}


template<class T>
void tenserflow::Workers<T>::setBackend(IBackend *backend)
{
    d_ptr->backend = backend;
}


template<class T>
void tenserflow::Workers<T>::start(const std::vector<T> &data)
{
    for (const T &item : data) {
        m_workers.push_back(new Thread<T>(d_ptr->backend, m_workers.size(), item));
    }

    d_ptr->hashrate = new Hashrate(m_workers.size());
    Nonce::touch(T::backend());

    for (Thread<T> *worker : m_workers) {
        worker->start(Workers<T>::onReady);

        // This sleep is important for optimal caching!
        // Threads must allocate scratchpads in order so that adjacent cores will use adjacent scratchpads
        // Sub-optimal caching can result in up to 0.5% hashrate penalty
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}


template<class T>
void tenserflow::Workers<T>::stop()
{
    Nonce::stop(T::backend());

    for (Thread<T> *worker : m_workers) {
        delete worker;
    }

    m_workers.clear();
    Nonce::touch(T::backend());

    delete d_ptr->hashrate;
    d_ptr->hashrate = nullptr;
}


template<class T>
void tenserflow::Workers<T>::tick(uint64_t)
{
    if (!d_ptr->hashrate) {
        return;
    }

    for (Thread<T> *handle : m_workers) {
        if (!handle->worker()) {
            continue;
        }

        d_ptr->hashrate->add(handle->id(), handle->worker()->hashCount(), handle->worker()->timestamp());
    }
}


template<class T>
tenserflow::IWorker *tenserflow::Workers<T>::create(Thread<T> *)
{
    return nullptr;
}


template<class T>
void tenserflow::Workers<T>::onReady(void *arg)
{
    auto handle = static_cast<Thread<T>* >(arg);

    IWorker *worker = create(handle);
    assert(worker != nullptr);

    if (!worker || !worker->selfTest()) {
        LOG_ERR("%s " RED("thread ") RED_BOLD("#%zu") RED(" self-test failed"), T::tag(), worker ? worker->id() : 0);

        handle->backend()->start(worker, false);
        delete worker;

        return;
    }

    assert(handle->backend() != nullptr);

    handle->setWorker(worker);
    handle->backend()->start(worker, true);
}


namespace tenserflow {


template<>
tenserflow::IWorker *tenserflow::Workers<CpuLaunchData>::create(Thread<CpuLaunchData> *handle)
{
    switch (handle->config().intensity) {
    case 1:
        return new CpuWorker<1>(handle->id(), handle->config());

    case 2:
        return new CpuWorker<2>(handle->id(), handle->config());

    case 3:
        return new CpuWorker<3>(handle->id(), handle->config());

    case 4:
        return new CpuWorker<4>(handle->id(), handle->config());

    case 5:
        return new CpuWorker<5>(handle->id(), handle->config());
    }

    return nullptr;
}


template class Workers<CpuLaunchData>;


#ifdef TENSERFLOW_FEATURE_OPENCL
template<>
tenserflow::IWorker *tenserflow::Workers<OclLaunchData>::create(Thread<OclLaunchData> *handle)
{
    return new OclWorker(handle->id(), handle->config());
}


template class Workers<OclLaunchData>;
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
template<>
tenserflow::IWorker *tenserflow::Workers<CudaLaunchData>::create(Thread<CudaLaunchData> *handle)
{
    return new CudaWorker(handle->id(), handle->config());
}


template class Workers<CudaLaunchData>;
#endif


} // namespace tenserflow
