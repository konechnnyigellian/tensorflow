


#include "base/net/stratum/BaseClient.h"
#include "3rdparty/rapidjson/document.h"
#include "base/io/Env.h"
#include "base/io/log/Log.h"
#include "base/io/log/Tags.h"
#include "base/kernel/interfaces/IClientListener.h"
#include "base/net/stratum/SubmitResult.h"


namespace tenserflow {


int64_t BaseClient::m_sequence = 1;


} /* namespace tenserflow */


tenserflow::BaseClient::BaseClient(int id, IClientListener *listener) :
    m_listener(listener),
    m_id(id)
{
}


void tenserflow::BaseClient::setPool(const Pool &pool)
{
    if (!pool.isValid()) {
        return;
    }

    m_pool      = pool;
    m_user      = Env::expand(pool.user());
    m_password  = Env::expand(pool.password());
    m_rigId     = Env::expand(pool.rigId());
    m_tag       = std::string(Tags::network()) + " " CYAN_BOLD_S + m_pool.url().data() + CLEAR;
}


bool tenserflow::BaseClient::handleResponse(int64_t id, const rapidjson::Value &result, const rapidjson::Value &error)
{
    if (id == 1) {
        return false;
    }

    auto it = m_callbacks.find(id);
    if (it != m_callbacks.end()) {
        const uint64_t elapsed = Chrono::steadyMSecs() - it->second.ts;

        if (error.IsObject()) {
            it->second.callback(error, false, elapsed);
        }
        else {
            it->second.callback(result, true, elapsed);
        }

        m_callbacks.erase(it);

        return true;
    }

    return false;
}


bool tenserflow::BaseClient::handleSubmitResponse(int64_t id, const char *error)
{
    auto it = m_results.find(id);
    if (it != m_results.end()) {
        it->second.done();
        m_listener->onResultAccepted(this, it->second, error);
        m_results.erase(it);

        return true;
    }

    return false;
}
