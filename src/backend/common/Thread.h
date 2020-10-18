

#ifndef TENSERFLOW_THREAD_H
#define TENSERFLOW_THREAD_H


#include "backend/common/interfaces/IWorker.h"
#include "base/tools/Object.h"


#include <thread>


namespace tenserflow {


class IBackend;


template<class T>
class Thread
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(Thread)

    inline Thread(IBackend *backend, size_t id, const T &config) : m_id(id), m_config(config), m_backend(backend) {}
    inline ~Thread() { m_thread.join(); delete m_worker; }

    inline const T &config() const                  { return m_config; }
    inline IBackend *backend() const                { return m_backend; }
    inline IWorker *worker() const                  { return m_worker; }
    inline size_t id() const                        { return m_id; }
    inline void setWorker(IWorker *worker)          { m_worker = worker; }
    inline void start(void (*callback) (void *))    { m_thread = std::thread(callback, this); }

private:
    const size_t m_id    = 0;
    const T m_config;
    IBackend *m_backend;
    IWorker *m_worker       = nullptr;
    std::thread m_thread;
};


} // namespace tenserflow


#endif /* TENSERFLOW_THREAD_H */
