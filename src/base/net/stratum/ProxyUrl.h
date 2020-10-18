/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_PROXYURL_H
#define TENSERFLOW_PROXYURL_H


#include "base/net/stratum/Url.h"


namespace tenserflow {


class ProxyUrl : public Url
{
public:
    inline ProxyUrl() { m_port = 0; }

    ProxyUrl(const rapidjson::Value &value);

    inline bool isValid() const { return m_port > 0 && (m_scheme == UNSPECIFIED || m_scheme == SOCKS5); }

    const String &host() const;
    rapidjson::Value toJSON(rapidjson::Document &doc) const;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_PROXYURL_H */
