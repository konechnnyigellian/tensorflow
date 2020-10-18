/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/net/stratum/ProxyUrl.h"
#include "3rdparty/rapidjson/document.h"


namespace tenserflow {

static const String kLocalhost = "127.0.0.1";

} // namespace tenserflow


tenserflow::ProxyUrl::ProxyUrl(const rapidjson::Value &value)
{
    m_port = 0;

    if (value.IsString()) {
        parse(value.GetString());
    }
    else if (value.IsUint()) {
        m_port = value.GetUint();
    }
}


const tenserflow::String &tenserflow::ProxyUrl::host() const
{
    return m_host.isNull() && isValid() ? kLocalhost : m_host;
}


rapidjson::Value tenserflow::ProxyUrl::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;
    if (!isValid()) {
        return Value(kNullType);
    }

    if (!m_host.isNull()) {
        return m_url.toJSON(doc);
    }

    return Value(m_port);
}
