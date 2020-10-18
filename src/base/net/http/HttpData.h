/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#ifndef TENSERFLOW_HTTPDATA_H
#define TENSERFLOW_HTTPDATA_H


#include "base/tools/Object.h"


#include <map>
#include <string>


namespace tenserflow {


class HttpData
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(HttpData)

    static const std::string kApplicationJson;
    static const std::string kContentType;
    static const std::string kContentTypeL;
    static const std::string kTextPlain;


    inline HttpData(uint64_t id) : m_id(id) {}
    virtual ~HttpData() = default;

    inline uint64_t id() const  { return m_id; }

    virtual bool isRequest() const                      = 0;
    virtual const char *host() const                    = 0;
    virtual const char *tlsFingerprint() const          = 0;
    virtual const char *tlsVersion() const              = 0;
    virtual std::string ip() const                      = 0;
    virtual uint16_t port() const                       = 0;
    virtual void write(std::string &&data, bool close)  = 0;

    bool isJSON() const;

    int method      = 0;
    int status      = 0;
    int userType    = 0;
    std::map<const std::string, const std::string> headers;
    std::string body;
    std::string url;

private:
    const uint64_t m_id;
};


} // namespace tenserflow


#endif // TENSERFLOW_HTTPDATA_H

