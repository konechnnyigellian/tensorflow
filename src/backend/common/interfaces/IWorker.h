

#ifndef TENSERFLOW_IWORKER_H
#define TENSERFLOW_IWORKER_H


#include <cstdint>
#include <cstddef>


namespace tenserflow {


class VirtualMemory;
class Job;


class IWorker
{
public:
    virtual ~IWorker() = default;

    virtual bool selfTest()                         = 0;
    virtual const VirtualMemory *memory() const     = 0;
    virtual size_t id() const                       = 0;
    virtual size_t intensity() const                = 0;
    virtual uint64_t hashCount() const              = 0;
    virtual uint64_t timestamp() const              = 0;
    virtual void start()                            = 0;
    virtual void jobEarlyNotification(const Job&)   = 0;
};


} // namespace tenserflow


#endif // TENSERFLOW_IWORKER_H
