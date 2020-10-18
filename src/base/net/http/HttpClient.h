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


#ifndef TENSERFLOW_HTTPCLIENT_H
#define TENSERFLOW_HTTPCLIENT_H


#include "base/kernel/interfaces/IDnsListener.h"
#include "base/net/http/Fetch.h"
#include "base/net/http/HttpContext.h"
#include "base/tools/Object.h"


namespace tenserflow {


class String;


class HttpClient : public HttpContext, public IDnsListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(HttpClient);

    HttpClient(FetchRequest &&req, const std::weak_ptr<IHttpListener> &listener);
    ~HttpClient() override;

    inline bool isQuiet() const                 { return m_req.quiet; }
    inline const char *host() const override    { return m_req.host; }
    inline uint16_t port() const override       { return m_req.port; }

    bool connect();

protected:
    void onResolved(const Dns &dns, int status) override;

    virtual void handshake();
    virtual void read(const char *data, size_t size);

protected:
    inline const FetchRequest &req() const  { return m_req; }

private:
    static void onConnect(uv_connect_t *req, int status);

    Dns *m_dns;
    FetchRequest m_req;
};


} // namespace tenserflow


#endif // TENSERFLOW_HTTPCLIENT_H

