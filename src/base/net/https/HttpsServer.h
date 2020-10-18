/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#ifndef TENSERFLOW_HTTPSSERVER_H
#define TENSERFLOW_HTTPSSERVER_H


using uv_tcp_t  = struct uv_tcp_s;

struct http_parser;
struct http_parser_settings;
struct uv_buf_t;


#include "base/kernel/interfaces/ITcpServerListener.h"
#include "base/tools/Object.h"


#include <memory>


namespace tenserflow {


class IHttpListener;
class TlsContext;
class TlsConfig;


class HttpsServer : public ITcpServerListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(HttpsServer)

    HttpsServer(const std::shared_ptr<IHttpListener> &listener);
    ~HttpsServer() override;

    bool setTls(const TlsConfig &config);

protected:
    void onConnection(uv_stream_t *stream, uint16_t port) override;

private:
    static void onRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

    std::weak_ptr<IHttpListener> m_listener;
    TlsContext *m_tls   = nullptr;
};


} // namespace tenserflow


#endif // TENSERFLOW_HTTPSSERVER_H

