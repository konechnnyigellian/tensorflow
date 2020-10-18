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


#ifndef TENSERFLOW_HTTPSERVER_H
#define TENSERFLOW_HTTPSERVER_H


using http_parser           = struct http_parser;
using http_parser_settings  = struct http_parser_settings;


#include "base/kernel/interfaces/ITcpServerListener.h"
#include "base/tools/Object.h"


#include <memory>


namespace tenserflow {


class IHttpListener;


class HttpServer : public ITcpServerListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(HttpServer)

    HttpServer(const std::shared_ptr<IHttpListener> &listener);
    ~HttpServer() override;

protected:
    void onConnection(uv_stream_t *stream, uint16_t port) override;

private:
    std::weak_ptr<IHttpListener> m_listener;
};


} // namespace tenserflow


#endif // TENSERFLOW_HTTPSERVER_H

