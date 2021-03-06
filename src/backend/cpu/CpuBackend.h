

#ifndef TENSERFLOW_CPUBACKEND_H
#define TENSERFLOW_CPUBACKEND_H


#include "backend/common/interfaces/IBackend.h"
#include "base/tools/Object.h"


#include <utility>


namespace tenserflow {


class Controller;
class CpuBackendPrivate;
class Tenserflower;


class CpuBackend : public IBackend
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(CpuBackend)

    CpuBackend(Controller *controller);
    ~CpuBackend() override;

protected:
    inline void execCommand(char) override {}

    bool isEnabled() const override;
    bool isEnabled(const Algorithm &algorithm) const override;
    const Hashrate *hashrate() const override;
    const String &profileName() const override;
    const String &type() const override;
    void prepare(const Job &nextJob) override;
    void printHashrate(bool details) override;
    void printHealth() override;
    void setJob(const Job &job) override;
    void start(IWorker *worker, bool ready) override;
    void stop() override;
    void tick(uint64_t ticks) override;

#   ifdef TENSERFLOW_FEATURE_API
    rapidjson::Value toJSON(rapidjson::Document &doc) const override;
    void handleRequest(IApiRequest &request) override;
#   endif

private:
    CpuBackendPrivate *d_ptr;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CPUBACKEND_H */
