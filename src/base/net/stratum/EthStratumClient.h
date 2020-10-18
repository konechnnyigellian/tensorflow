/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_ETHSTRATUMCLIENT_H
#define TENSERFLOW_ETHSTRATUMCLIENT_H


#include "base/net/stratum/Client.h"


#include <utility>


namespace tenserflow {


class EthStratumClient : public Client
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(EthStratumClient)

    EthStratumClient(int id, const char *agent, IClientListener *listener);
    ~EthStratumClient() override = default;

protected:
    int64_t submit(const JobResult &result) override;
    void login() override;
    void onClose() override;

    bool handleResponse(int64_t id, const rapidjson::Value &result, const rapidjson::Value &error) override;
    void parseNotification(const char *method, const rapidjson::Value &params, const rapidjson::Value &error) override;

    void setExtraNonce(const rapidjson::Value &nonce);

private:
    const char *errorMessage(const rapidjson::Value &error) const;
    void authorize();
    void onAuthorizeResponse(const rapidjson::Value &result, bool success, uint64_t elapsed);
    void onSubscribeResponse(const rapidjson::Value &result, bool success, uint64_t elapsed);
    void subscribe();

    bool m_authorized   = false;
    std::pair<uint64_t, String> m_extraNonce{};
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_ETHSTRATUMCLIENT_H */
