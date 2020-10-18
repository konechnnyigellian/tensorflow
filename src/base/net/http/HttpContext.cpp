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


#include "base/net/http/HttpContext.h"
#include "3rdparty/http-parser/http_parser.h"
#include "base/kernel/interfaces/IHttpListener.h"
#include "base/tools/Baton.h"
#include "base/tools/Chrono.h"


#include <algorithm>
#include <uv.h>


namespace tenserflow {


static http_parser_settings http_settings;
static std::map<uint64_t, HttpContext *> storage;
static uint64_t SEQUENCE = 0;


class HttpWriteBaton : public Baton<uv_write_t>
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(HttpWriteBaton)

    inline HttpWriteBaton(std::string &&body, HttpContext *ctx) :
        m_ctx(ctx),
        m_body(std::move(body))
    {
        m_buf = uv_buf_init(&m_body.front(), m_body.size());
    }

    inline ~HttpWriteBaton()
    {
        if (m_ctx) {
            m_ctx->close();
        }
    }

    void write(uv_stream_t *stream)
    {
        uv_write(&req, stream, &m_buf, 1, [](uv_write_t *req, int) { delete reinterpret_cast<HttpWriteBaton *>(req->data); });
    }

private:
    HttpContext *m_ctx;
    std::string m_body;
    uv_buf_t m_buf{};
};


} // namespace tenserflow


tenserflow::HttpContext::HttpContext(int parser_type, const std::weak_ptr<IHttpListener> &listener) :
    HttpData(SEQUENCE++),
    m_timestamp(Chrono::steadyMSecs()),
    m_listener(listener)
{
    storage[id()] = this;

    m_parser = new http_parser;
    m_tcp    = new uv_tcp_t;

    uv_tcp_init(uv_default_loop(), m_tcp);
    uv_tcp_nodelay(m_tcp, 1);

    http_parser_init(m_parser, static_cast<http_parser_type>(parser_type));

    m_parser->data = m_tcp->data = this;

    if (http_settings.on_message_complete == nullptr) {
        attach(&http_settings);
    }
}


tenserflow::HttpContext::~HttpContext()
{
    delete m_tcp;
    delete m_parser;
}


void tenserflow::HttpContext::write(std::string &&data, bool close)
{
    if (uv_is_writable(stream()) != 1) {
        return;
    }

    auto baton = new HttpWriteBaton(std::move(data), close ? this : nullptr);
    baton->write(stream());
}


bool tenserflow::HttpContext::isRequest() const
{
    return m_parser->type == HTTP_REQUEST;
}


size_t tenserflow::HttpContext::parse(const char *data, size_t size)
{
    if (size == 0) {
        return size;
    }

    return http_parser_execute(m_parser, &http_settings, data, size);
}


std::string tenserflow::HttpContext::ip() const
{
    char ip[46]           = {};
    sockaddr_storage addr = {};
    int size              = sizeof(addr);

    uv_tcp_getpeername(m_tcp, reinterpret_cast<sockaddr*>(&addr), &size);
    if (reinterpret_cast<sockaddr_in *>(&addr)->sin_family == AF_INET6) {
        uv_ip6_name(reinterpret_cast<sockaddr_in6*>(&addr), ip, 45);
    }
    else {
        uv_ip4_name(reinterpret_cast<sockaddr_in*>(&addr), ip, 16);
    }

    return ip;
}


uint64_t tenserflow::HttpContext::elapsed() const
{
    return Chrono::steadyMSecs() - m_timestamp;
}


void tenserflow::HttpContext::close(int status)
{
    auto listener = httpListener();

    if (status < 0 && listener) {
        this->status = status;
        listener->onHttpData(*this);
    }

    storage.erase(id());

    if (!uv_is_closing(handle())) {
        uv_close(handle(), [](uv_handle_t *handle) -> void { delete reinterpret_cast<HttpContext*>(handle->data); });
    }
}


tenserflow::HttpContext *tenserflow::HttpContext::get(uint64_t id)
{
    if (storage.count(id) == 0) {
        return nullptr;
    }

    return storage[id];
}


void tenserflow::HttpContext::closeAll()
{
    for (auto &kv : storage) {
        if (!uv_is_closing(kv.second->handle())) {
            uv_close(kv.second->handle(), [](uv_handle_t *handle) -> void { delete reinterpret_cast<HttpContext*>(handle->data); });
        }
    }
}


int tenserflow::HttpContext::onHeaderField(http_parser *parser, const char *at, size_t length)
{
    auto ctx = static_cast<HttpContext*>(parser->data);

    if (ctx->m_wasHeaderValue) {
        if (!ctx->m_lastHeaderField.empty()) {
            ctx->setHeader();
        }

        ctx->m_lastHeaderField = std::string(at, length);
        ctx->m_wasHeaderValue  = false;
    } else {
        ctx->m_lastHeaderField += std::string(at, length);
    }

    return 0;
}


int tenserflow::HttpContext::onHeaderValue(http_parser *parser, const char *at, size_t length)
{
    auto ctx = static_cast<HttpContext*>(parser->data);

    if (!ctx->m_wasHeaderValue) {
        ctx->m_lastHeaderValue = std::string(at, length);
        ctx->m_wasHeaderValue  = true;
    } else {
        ctx->m_lastHeaderValue += std::string(at, length);
    }

    return 0;
}


void tenserflow::HttpContext::attach(http_parser_settings *settings)
{
    settings->on_message_begin  = nullptr;
    settings->on_status         = nullptr;
    settings->on_chunk_header   = nullptr;
    settings->on_chunk_complete = nullptr;

    settings->on_url = [](http_parser *parser, const char *at, size_t length) -> int
    {
        static_cast<HttpContext*>(parser->data)->url = std::string(at, length);
        return 0;
    };

    settings->on_header_field = onHeaderField;
    settings->on_header_value = onHeaderValue;

    settings->on_headers_complete = [](http_parser* parser) -> int {
        auto ctx = static_cast<HttpContext*>(parser->data);
        ctx->status = parser->status_code;

        if (parser->type == HTTP_REQUEST) {
            ctx->method = parser->method;
        }

        if (!ctx->m_lastHeaderField.empty()) {
            ctx->setHeader();
        }

        return 0;
    };

    settings->on_body = [](http_parser *parser, const char *at, size_t len) -> int
    {
        static_cast<HttpContext*>(parser->data)->body += std::string(at, len);

        return 0;
    };

    settings->on_message_complete = [](http_parser *parser) -> int
    {
        auto ctx      = static_cast<HttpContext*>(parser->data);
        auto listener = ctx->httpListener();

        if (listener) {
            listener->onHttpData(*ctx);
            ctx->m_listener.reset();
        }

        return 0;
    };
}


void tenserflow::HttpContext::setHeader()
{
    std::transform(m_lastHeaderField.begin(), m_lastHeaderField.end(), m_lastHeaderField.begin(), ::tolower);
    headers.insert({ m_lastHeaderField, m_lastHeaderValue });

    m_lastHeaderField.clear();
    m_lastHeaderValue.clear();
}
