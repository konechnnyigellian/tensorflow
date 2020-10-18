

#ifndef TENSERFLOW_BASE_H
#define TENSERFLOW_BASE_H


#include "3rdparty/rapidjson/fwd.h"
#include "base/api/interfaces/IApiListener.h"
#include "base/kernel/interfaces/IConfigListener.h"
#include "base/kernel/interfaces/IWatcherListener.h"
#include "base/tools/Object.h"


namespace tenserflow {


class Api;
class BasePrivate;
class Config;
class IBaseListener;
class Process;


class Base : public IWatcherListener, public IApiListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(Base)

    Base(Process *process);
    ~Base() override;

    virtual bool isReady() const;
    virtual int init();
    virtual void start();
    virtual void stop();

    Api *api() const;
    bool isBackground() const;
    bool reload(const rapidjson::Value &json);
    Config *config() const;
    void addListener(IBaseListener *listener);

protected:
    void onFileChanged(const String &fileName) override;

#   ifdef TENSERFLOW_FEATURE_API
    void onRequest(IApiRequest &request) override;
#   endif

private:
    BasePrivate *d_ptr;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_BASE_H */
