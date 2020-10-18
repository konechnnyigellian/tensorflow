

#ifndef TENSERFLOW_TENSERFLOWER_H
#define TENSERFLOW_TENSERFLOWER_H


#include <vector>


#include "backend/common/interfaces/IRxListener.h"
#include "base/api/interfaces/IApiListener.h"
#include "base/crypto/Algorithm.h"
#include "base/kernel/interfaces/IBaseListener.h"
#include "base/kernel/interfaces/ITimerListener.h"
#include "base/tools/Object.h"


namespace tenserflow {


class Controller;
class Job;
class TenserflowerPrivate;
class IBackend;


class Tenserflower : public ITimerListener, public IBaseListener, public IApiListener, public IRxListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(Tenserflower)

    Tenserflower(Controller *controller);
    ~Tenserflower() override;

    bool isEnabled() const;
    bool isEnabled(const Algorithm &algorithm) const;
    const Algorithms &algorithms() const;
    const std::vector<IBackend *> &backends() const;
    Job job() const;
    void execCommand(char command);
    void pause();
    void setEnabled(bool enabled);
    void setJob(const Job &job, bool donate);
    void stop();

protected:
    void onConfigChanged(Config *config, Config *previousConfig) override;
    void onTimer(const Timer *timer) override;

#   ifdef TENSERFLOW_FEATURE_API
    void onRequest(IApiRequest &request) override;
#   endif

#   ifdef TENSERFLOW_ALGO_RANDOMX
    void onDatasetReady() override;
#   endif

private:
    TenserflowerPrivate *d_ptr;
};


} // namespace tenserflow


#endif /* TENSERFLOW_TENSERFLOWER_H */
