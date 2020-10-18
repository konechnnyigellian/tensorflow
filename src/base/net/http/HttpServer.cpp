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


#include <functional>
#include <uv.h>


#include "base/net/http/HttpServer.h"
#include "3rdparty/http-parser/http_parser.h"
#include "base/net/http/HttpContext.h"
#include "base/net/tools/NetBuffer.h"


tenserflow::HttpServer::HttpServer(const std::shared_ptr<IHttpListener> &listener) :
    m_listener(listener)
{
}


tenserflow::HttpServer::~HttpServer()
{
    HttpContext::closeAll();
}


void tenserflow::HttpServer::onConnection(uv_stream_t *stream, uint16_t)
{
    auto ctx = new HttpContext(HTTP_REQUEST, m_listener);
    uv_accept(stream, ctx->stream());

    uv_read_start(ctx->stream(), NetBuffer::onAlloc,
        [](uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf)
        {
            auto ctx = static_cast<HttpContext*>(tcp->data);

            if (nread >= 0) {
                const auto size   = static_cast<size_t>(nread);
                const auto parsed = ctx->parse(buf->base, size);

                if (parsed < size) {
                    ctx->close();
                }
            } else {
                ctx->close();
            }

            NetBuffer::release(buf);
        });
}
