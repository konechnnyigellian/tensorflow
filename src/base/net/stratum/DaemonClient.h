

#ifndef TENSERFLOW_DAEMONCLIENT_H
#define TENSERFLOW_DAEMONCLIENT_H


#include "base/kernel/interfaces/ITimerListener.h"
#include "base/net/http/HttpListener.h"
#include "base/net/stratum/BaseClient.h"
#include "base/tools/Object.h"


#include <memory>


namespace tenserflow {


class DaemonClient : public BaseClient, public ITimerListener, public IHttpListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(DaemonClient)

    DaemonClient(int id, IClientListener *listener);
    ~DaemonClient() override;

protected:
    bool disconnect() override;
    bool isTLS() const override;
    int64_t submit(const JobResult &result) override;
    void connect() override;
    void connect(const Pool &pool) override;

    void onHttpData(const HttpData &data) override;
    void onTimer(const Timer *timer) override;

    inline bool hasExtension(Extension) const noexcept override         { return false; }
    inline const char *mode() const override                            { return "daemon"; }
    inline const char *tlsFingerprint() const override                  { return m_tlsFingerprint; }
    inline const char *tlsVersion() const override                      { return m_tlsVersion; }
    inline int64_t send(const rapidjson::Value &, Callback) override    { return -1; }
    inline int64_t send(const rapidjson::Value &) override              { return -1; }
    inline void deleteLater() override                                  { delete this; }
    inline void tick(uint64_t) override                                 {}

private:
    bool isOutdated(uint64_t height, const char *hash) const;
    bool parseJob(const rapidjson::Value &params, int *code);
    bool parseResponse(int64_t id, const rapidjson::Value &result, const rapidjson::Value &error);
    int64_t getBlockTemplate();
    int64_t rpcSend(const rapidjson::Document &doc);
    void retry();
    void send(const char *path);
    void setState(SocketState state);

    enum {
        API_CRYPTONOTE_DEFAULT,
        API_TENSERO,
        API_DERO,
    } m_apiVersion = API_TENSERO;

    std::shared_ptr<IHttpListener> m_httpListener;
    String m_blocktemplate;
    String m_blockhashingblob;
    String m_prevHash;
    String m_tlsFingerprint;
    String m_tlsVersion;
    Timer *m_timer;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_DAEMONCLIENT_H */
