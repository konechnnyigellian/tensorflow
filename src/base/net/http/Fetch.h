/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#ifndef TENSERFLOW_FETCH_H
#define TENSERFLOW_FETCH_H


#include "3rdparty/http-parser/http_parser.h"
#include "3rdparty/rapidjson/fwd.h"
#include "base/tools/String.h"


#include <map>
#include <memory>
#include <string>


namespace tenserflow {


class IHttpListener;


class FetchRequest
{
public:
    FetchRequest() = default;
    FetchRequest(http_method method, const String &host, uint16_t port, const String &path, bool tls = false, bool quiet = false, const char *data = nullptr, size_t size = 0, const char *contentType = nullptr);
    FetchRequest(http_method method, const String &host, uint16_t port, const String &path, const rapidjson::Value &value, bool tls = false, bool quiet = false);

    void setBody(const char *data, size_t size, const char *contentType = nullptr);
    void setBody(const rapidjson::Value &value);

    inline bool hasBody() const { return method != HTTP_GET && method != HTTP_HEAD && !body.empty(); }

    bool quiet          = false;
    bool tls            = false;
    http_method method  = HTTP_GET;
    std::map<const std::string, const std::string> headers;
    std::string body;
    String fingerprint;
    String host;
    String path;
    uint16_t port       = 0;
};


void fetch(FetchRequest &&req, const std::weak_ptr<IHttpListener> &listener, int type = 0);


} // namespace tenserflow


#endif // TENSERFLOW_FETCH_H

