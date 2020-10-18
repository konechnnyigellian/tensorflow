

#ifndef TENSERFLOW_IBACKEND_H
#define TENSERFLOW_IBACKEND_H


#include <cstdint>


#include "3rdparty/rapidjson/fwd.h"


namespace tenserflow {


class Algorithm;
class Hashrate;
class IApiRequest;
class IWorker;
class Job;
class String;


class IBackend
{
public:
    virtual ~IBackend() = default;

    virtual bool isEnabled() const                                      = 0;
    virtual bool isEnabled(const Algorithm &algorithm) const            = 0;
    virtual const Hashrate *hashrate() const                            = 0;
    virtual const String &profileName() const                           = 0;
    virtual const String &type() const                                  = 0;
    virtual void execCommand(char command)                              = 0;
    virtual void prepare(const Job &nextJob)                            = 0;
    virtual void printHashrate(bool details)                            = 0;
    virtual void printHealth()                                          = 0;
    virtual void setJob(const Job &job)                                 = 0;
    virtual void start(IWorker *worker, bool ready)                     = 0;
    virtual void stop()                                                 = 0;
    virtual void tick(uint64_t ticks)                                   = 0;

#   ifdef TENSERFLOW_FEATURE_API
    virtual rapidjson::Value toJSON(rapidjson::Document &doc) const     = 0;
    virtual void handleRequest(IApiRequest &request)                    = 0;
#   endif
};


} // namespace tenserflow


#endif // TENSERFLOW_IBACKEND_H
