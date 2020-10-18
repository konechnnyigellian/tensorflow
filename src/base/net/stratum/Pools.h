

#ifndef TENSERFLOW_POOLS_H
#define TENSERFLOW_POOLS_H


#include <vector>


#include "base/net/stratum/Pool.h"


namespace tenserflow {


class IJsonReader;
class IStrategy;
class IStrategyListener;


class Pools
{
public:
    static const char *kDonateLevel;
    static const char *kDonateOverProxy;
    static const char *kPools;
    static const char *kRetries;
    static const char *kRetryPause;

    enum ProxyDonate {
        PROXY_DONATE_NONE,
        PROXY_DONATE_AUTO,
        PROXY_DONATE_ALWAYS
    };

    Pools();

    inline const std::vector<Pool> &data() const        { return m_data; }
    inline int donateLevel() const                      { return m_donateLevel; }
    inline int retries() const                          { return m_retries; }
    inline int retryPause() const                       { return m_retryPause; }
    inline ProxyDonate proxyDonate() const              { return m_proxyDonate; }

    inline bool operator!=(const Pools &other) const    { return !isEqual(other); }
    inline bool operator==(const Pools &other) const    { return isEqual(other); }

    bool isEqual(const Pools &other) const;
    IStrategy *createStrategy(IStrategyListener *listener) const;
    rapidjson::Value toJSON(rapidjson::Document &doc) const;
    size_t active() const;
    void load(const IJsonReader &reader);
    void print() const;

private:
    void setDonateLevel(int level);
    void setProxyDonate(int value);
    void setRetries(int retries);
    void setRetryPause(int retryPause);

    int m_donateLevel;
    int m_retries               = 5;
    int m_retryPause            = 5;
    ProxyDonate m_proxyDonate   = PROXY_DONATE_AUTO;
    std::vector<Pool> m_data;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_POOLS_H */
