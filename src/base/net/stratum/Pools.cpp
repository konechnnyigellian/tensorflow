


#include "base/net/stratum/Pools.h"
#include "3rdparty/rapidjson/document.h"
#include "base/io/log/Log.h"
#include "base/kernel/interfaces/IJsonReader.h"
#include "base/net/stratum/strategies/FailoverStrategy.h"
#include "base/net/stratum/strategies/SinglePoolStrategy.h"
#include "donate.h"


namespace tenserflow {


const char *Pools::kDonateLevel     = "donate-level";
const char *Pools::kDonateOverProxy = "donate-over-proxy";
const char *Pools::kPools           = "pools";
const char *Pools::kRetries         = "retries";
const char *Pools::kRetryPause      = "retry-pause";


} // namespace tenserflow


tenserflow::Pools::Pools() :
    m_donateLevel(kDefaultDonateLevel)
{
#   ifdef TENSERFLOW_PROXY_PROJECT
    m_retries    = 2;
    m_retryPause = 1;
#   endif
}


bool tenserflow::Pools::isEqual(const Pools &other) const
{
    if (m_data.size() != other.m_data.size() || m_retries != other.m_retries || m_retryPause != other.m_retryPause) {
        return false;
    }

    return std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
}


tenserflow::IStrategy *tenserflow::Pools::createStrategy(IStrategyListener *listener) const
{
    if (active() == 1) {
        for (const Pool &pool : m_data) {
            if (pool.isEnabled()) {
                return new SinglePoolStrategy(pool, retryPause(), retries(), listener);
            }
        }
    }

    auto strategy = new FailoverStrategy(retryPause(), retries(), listener);
    for (const Pool &pool : m_data) {
        if (pool.isEnabled()) {
            strategy->add(pool);
        }
    }

    return strategy;
}


rapidjson::Value tenserflow::Pools::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    Value pools(kArrayType);

    for (const Pool &pool : m_data) {
        pools.PushBack(pool.toJSON(doc), allocator);
    }

    return pools;
}


size_t tenserflow::Pools::active() const
{
    size_t count = 0;
    for (const Pool &pool : m_data) {
        if (pool.isEnabled()) {
            count++;
        }
    }

    return count;
}


void tenserflow::Pools::load(const IJsonReader &reader)
{
    m_data.clear();

    const rapidjson::Value &pools = reader.getArray(kPools);
    if (!pools.IsArray()) {
        return;
    }

    for (const rapidjson::Value &value : pools.GetArray()) {
        if (!value.IsObject()) {
            continue;
        }

        Pool pool(value);
        if (pool.isValid()) {
            m_data.push_back(std::move(pool));
        }
    }

    setDonateLevel(reader.getInt(kDonateLevel, kDefaultDonateLevel));
    setProxyDonate(reader.getInt(kDonateOverProxy, PROXY_DONATE_AUTO));
    setRetries(reader.getInt(kRetries));
    setRetryPause(reader.getInt(kRetryPause));
}


void tenserflow::Pools::print() const
{
    size_t i = 1;
    for (const Pool &pool : m_data) {
        Log::print(GREEN_BOLD(" * ") WHITE_BOLD("POOL #%-7zu") "%s", i, pool.printableName().c_str());

        i++;
    }

#   ifdef APP_DEBUG
    LOG_NOTICE("POOLS --------------------------------------------------------------------");
    for (const Pool &pool : m_data) {
        pool.print();
    }
    LOG_NOTICE("--------------------------------------------------------------------------");
#   endif
}


void tenserflow::Pools::setDonateLevel(int level)
{
    if (level >= kMinimumDonateLevel && level <= 99) {
        m_donateLevel = level;
    }
}


void tenserflow::Pools::setProxyDonate(int value)
{
    switch (value) {
    case PROXY_DONATE_NONE:
    case PROXY_DONATE_AUTO:
    case PROXY_DONATE_ALWAYS:
        m_proxyDonate = static_cast<ProxyDonate>(value);
    }
}


void tenserflow::Pools::setRetries(int retries)
{
    if (retries > 0 && retries <= 1000) {
        m_retries = retries;
    }
}


void tenserflow::Pools::setRetryPause(int retryPause)
{
    if (retryPause > 0 && retryPause <= 3600) {
        m_retryPause = retryPause;
    }
}
