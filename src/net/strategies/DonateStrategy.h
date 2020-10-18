

#ifndef TENSERFLOW_DONATESTRATEGY_H
#define TENSERFLOW_DONATESTRATEGY_H


#include <vector>


#include "base/kernel/interfaces/IClientListener.h"
#include "base/kernel/interfaces/IStrategy.h"
#include "base/kernel/interfaces/IStrategyListener.h"
#include "base/kernel/interfaces/ITimerListener.h"
#include "base/net/stratum/Pool.h"
#include "base/tools/Object.h"


namespace tenserflow {


class Client;
class Controller;
class IStrategyListener;


class DonateStrategy : public IStrategy, public IStrategyListener, public ITimerListener, public IClientListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(DonateStrategy)

    DonateStrategy(Controller *controller, IStrategyListener *listener);
    ~DonateStrategy() override;

protected:
    inline bool isActive() const override                                                                              { return state() == STATE_ACTIVE; }
    inline IClient *client() const override                                                                            { return m_proxy ? m_proxy : m_strategy->client(); }
    inline void onJob(IStrategy *, IClient *client, const Job &job, const rapidjson::Value &params) override           { setJob(client, job, params); }
    inline void onJobReceived(IClient *client, const Job &job, const rapidjson::Value &params) override                { setJob(client, job, params); }
    inline void onResultAccepted(IClient *client, const SubmitResult &result, const char *error) override              { setResult(client, result, error); }
    inline void onResultAccepted(IStrategy *, IClient *client, const SubmitResult &result, const char *error) override { setResult(client, result, error); }
    inline void resume() override                                                                                      {}

    int64_t submit(const JobResult &result) override;
    void connect() override;
    void setAlgo(const Algorithm &algo) override;
    void setProxy(const ProxyUrl &proxy) override;
    void stop() override;
    void tick(uint64_t now) override;

    void onActive(IStrategy *strategy, IClient *client) override;
    void onPause(IStrategy *strategy) override;

    void onClose(IClient *client, int failures) override;
    void onLogin(IClient *client, rapidjson::Document &doc, rapidjson::Value &params) override;
    void onLogin(IStrategy *strategy, IClient *client, rapidjson::Document &doc, rapidjson::Value &params) override;
    void onLoginSuccess(IClient *client) override;
    void onVerifyAlgorithm(const IClient *client, const Algorithm &algorithm, bool *ok) override;
    void onVerifyAlgorithm(IStrategy *strategy, const  IClient *client, const Algorithm &algorithm, bool *ok) override;

    void onTimer(const Timer *timer) override;

private:
    enum State {
        STATE_NEW,
        STATE_IDLE,
        STATE_CONNECT,
        STATE_ACTIVE,
        STATE_WAIT
    };

    inline State state() const { return m_state; }

    IClient *createProxy();
    void idle(double min, double max);
    void setAlgorithms(rapidjson::Document &doc, rapidjson::Value &params);
    void setJob(IClient *client, const Job &job, const rapidjson::Value &params);
    void setResult(IClient *client, const SubmitResult &result, const char *error);
    void setState(State state);

    Algorithm m_algorithm;
    bool m_tls                      = false;
    char m_userId[65]               = { 0 };
    const uint64_t m_donateTime;
    const uint64_t m_idleTime;
    Controller *m_controller;
    IClient *m_proxy                = nullptr;
    IStrategy *m_strategy           = nullptr;
    IStrategyListener *m_listener;
    State m_state                   = STATE_NEW;
    std::vector<Pool> m_pools;
    Timer *m_timer                  = nullptr;
    uint64_t m_now                  = 0;
    uint64_t m_timestamp            = 0;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_DONATESTRATEGY_H */
