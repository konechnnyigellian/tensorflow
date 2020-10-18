

#ifndef TENSERFLOW_CUDABACKEND_H
#define TENSERFLOW_CUDABACKEND_H


#include <utility>


#include "backend/common/interfaces/IBackend.h"
#include "base/tools/Object.h"


namespace tenserflow {


class Controller;
class CudaBackendPrivate;
class Tenserflower;


class CudaBackend : public IBackend
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(CudaBackend)

    CudaBackend(Controller *controller);

    ~CudaBackend() override;

protected:
    bool isEnabled() const override;
    bool isEnabled(const Algorithm &algorithm) const override;
    const Hashrate *hashrate() const override;
    const String &profileName() const override;
    const String &type() const override;
    void execCommand(char command) override;
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
    CudaBackendPrivate *d_ptr;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CUDABACKEND_H */
