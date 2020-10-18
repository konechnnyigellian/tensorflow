/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/net/stratum/AutoClient.h"
#include "3rdparty/rapidjson/document.h"
#include "base/io/json/Json.h"


tenserflow::AutoClient::AutoClient(int id, const char *agent, IClientListener *listener) :
    EthStratumClient(id, agent, listener)
{
}


bool tenserflow::AutoClient::handleResponse(int64_t id, const rapidjson::Value &result, const rapidjson::Value &error)
{
    if (m_mode == DEFAULT_MODE) {
        return Client::handleResponse(id, result, error);
    }

    return EthStratumClient::handleResponse(id, result, error);
}


bool tenserflow::AutoClient::parseLogin(const rapidjson::Value &result, int *code)
{
    if (result.HasMember("job")) {
        return Client::parseLogin(result, code);
    }

    setRpcId(Json::getString(result, "id"));
    if (rpcId().isNull()) {
        *code = 1;
        return false;
    }

    const Algorithm algo(Json::getString(result, "algo"));
    if (algo.family() != Algorithm::KAWPOW) {
        *code = 6;
        return false;
    }

    try {
        setExtraNonce(Json::getValue(result, "extra_nonce"));
    } catch (const std::exception &ex) {
        *code = 6;
        return false;
    }

    m_mode = ETH_MODE;
    setAlgo(algo);

    return true;
}


int64_t tenserflow::AutoClient::submit(const JobResult &result)
{
    if (m_mode == DEFAULT_MODE) {
        return Client::submit(result);
    }

    return EthStratumClient::submit(result);
}


void tenserflow::AutoClient::parseNotification(const char *method, const rapidjson::Value &params, const rapidjson::Value &error)
{
    if (m_mode == DEFAULT_MODE) {
        return Client::parseNotification(method, params, error);
    }

    return EthStratumClient::parseNotification(method, params, error);
}
