


#include <cassert>
#include <memory>


#include "base/kernel/Base.h"
#include "base/io/json/Json.h"
#include "base/io/json/JsonChain.h"
#include "base/io/log/backends/ConsoleLog.h"
#include "base/io/log/backends/FileLog.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/io/Watcher.h"
#include "base/kernel/interfaces/IBaseListener.h"
#include "base/kernel/Platform.h"
#include "base/kernel/Process.h"
#include "base/net/tools/NetBuffer.h"
#include "core/config/Config.h"
#include "core/config/ConfigTransform.h"


#ifdef HAVE_SYSLOG_H
#   include "base/io/log/backends/SysLog.h"
#endif


#ifdef TENSERFLOW_FEATURE_API
#   include "base/api/Api.h"
#   include "base/api/interfaces/IApiRequest.h"

namespace tenserflow {

static const char *kConfigPathV1 = "/1/config";
static const char *kConfigPathV2 = "/2/config";

} // namespace tenserflow
#endif


#ifdef TENSERFLOW_FEATURE_EMBEDDED_CONFIG
#   include "core/config/Config_default.h"
#endif


namespace tenserflow {


class BasePrivate
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(BasePrivate)


    inline BasePrivate(Process *process) : config(load(process)) {}


    inline ~BasePrivate()
    {
#       ifdef TENSERFLOW_FEATURE_API
        delete api;
#       endif

        delete config;
        delete watcher;

        NetBuffer::destroy();
    }


    inline bool read(const JsonChain &chain, std::unique_ptr<Config> &config)
    {
        config = std::unique_ptr<Config>(new Config());

        return config->read(chain, chain.fileName());
    }


    inline void replace(Config *newConfig)
    {
        Config *previousConfig = config;
        config = newConfig;

        for (IBaseListener *listener : listeners) {
            listener->onConfigChanged(config, previousConfig);
        }

        delete previousConfig;
    }


    Api *api            = nullptr;
    Config *config      = nullptr;
    std::vector<IBaseListener *> listeners;
    Watcher *watcher    = nullptr;


private:
    inline Config *load(Process *process)
    {
        JsonChain chain;
        ConfigTransform transform;
        std::unique_ptr<Config> config;

        ConfigTransform::load(chain, process, transform);

        if (read(chain, config)) {
            return config.release();
        }

        chain.addFile(Process::location(Process::DataLocation, "config.json"));

        if (read(chain, config)) {
            return config.release();
        }

#       ifdef TENSERFLOW_FEATURE_EMBEDDED_CONFIG
        chain.addRaw(default_config);

        if (read(chain, config)) {
            return config.release();
        }
#       endif

        return nullptr;
    }
};


} // namespace tenserflow


tenserflow::Base::Base(Process *process)
    : d_ptr(new BasePrivate(process))
{
}


tenserflow::Base::~Base()
{
    delete d_ptr;
}


bool tenserflow::Base::isReady() const
{
    return d_ptr->config != nullptr;
}


int tenserflow::Base::init()
{
#   ifdef TENSERFLOW_FEATURE_API
    d_ptr->api = new Api(this);
    d_ptr->api->addListener(this);
#   endif

    Platform::init(config()->userAgent());

    if (isBackground()) {
        Log::setBackground(true);
    }
    else {
        Log::add(new ConsoleLog(config()->title()));
    }

    if (config()->logFile()) {
        Log::add(new FileLog(config()->logFile()));
    }

#   ifdef HAVE_SYSLOG_H
    if (config()->isSyslog()) {
        Log::add(new SysLog());
    }
#   endif

    return 0;
}


void tenserflow::Base::start()
{
#   ifdef TENSERFLOW_FEATURE_API
    api()->start();
#   endif

    if (config()->isShouldSave()) {
        config()->save();
    }

    if (config()->isWatch()) {
        d_ptr->watcher = new Watcher(config()->fileName(), this);
    }
}


void tenserflow::Base::stop()
{
#   ifdef TENSERFLOW_FEATURE_API
    api()->stop();
#   endif

    delete d_ptr->watcher;
    d_ptr->watcher = nullptr;
}


tenserflow::Api *tenserflow::Base::api() const
{
    assert(d_ptr->api != nullptr);

    return d_ptr->api;
}


bool tenserflow::Base::isBackground() const
{
    return d_ptr->config && d_ptr->config->isBackground();
}


bool tenserflow::Base::reload(const rapidjson::Value &json)
{
    JsonReader reader(json);
    if (reader.isEmpty()) {
        return false;
    }

    auto config = new Config();
    if (!config->read(reader, d_ptr->config->fileName())) {
        delete config;

        return false;
    }

    const bool saved = config->save();

    if (config->isWatch() && d_ptr->watcher && saved) {
        delete config;

        return true;
    }

    d_ptr->replace(config);

    return true;
}


tenserflow::Config *tenserflow::Base::config() const
{
    assert(d_ptr->config != nullptr);

    return d_ptr->config;
}


void tenserflow::Base::addListener(IBaseListener *listener)
{
    d_ptr->listeners.push_back(listener);
}


void tenserflow::Base::onFileChanged(const String &fileName)
{
    LOG_WARN("%s " YELLOW("\"%s\" was changed, reloading configuration"), Tags::config(), fileName.data());

    JsonChain chain;
    chain.addFile(fileName);

    auto config = new Config();

    if (!config->read(chain, chain.fileName())) {
        LOG_ERR("%s " RED("reloading failed"), Tags::config());

        delete config;
        return;
    }

    d_ptr->replace(config);
}


#ifdef TENSERFLOW_FEATURE_API
void tenserflow::Base::onRequest(IApiRequest &request)
{
    if (request.method() == IApiRequest::METHOD_GET) {
        if (request.url() == kConfigPathV1 || request.url() == kConfigPathV2) {
            if (request.isRestricted()) {
                return request.done(403);
            }

            request.accept();
            config()->getJSON(request.doc());
        }
    }
    else if (request.method() == IApiRequest::METHOD_PUT || request.method() == IApiRequest::METHOD_POST) {
        if (request.url() == kConfigPathV1 || request.url() == kConfigPathV2) {
            request.accept();

            if (!reload(request.json())) {
                return request.done(400);
            }

            request.done(204);
        }
    }
}
#endif
