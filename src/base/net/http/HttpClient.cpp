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


#include "base/net/http/HttpClient.h"
#include "3rdparty/http-parser/http_parser.h"
#include "base/io/log/Log.h"
#include "base/kernel/Platform.h"
#include "base/net/dns/Dns.h"
#include "base/net/tools/NetBuffer.h"


#include <sstream>


namespace tenserflow {


static const char *kCRLF = "\r\n";


} // namespace tenserflow


tenserflow::HttpClient::HttpClient(FetchRequest &&req, const std::weak_ptr<IHttpListener> &listener) :
    HttpContext(HTTP_RESPONSE, listener),
    m_req(std::move(req))
{
    method  = m_req.method;
    url     = std::move(m_req.path);
    body    = std::move(m_req.body);
    headers = std::move(m_req.headers);
    m_dns   = new Dns(this);
}


tenserflow::HttpClient::~HttpClient()
{
    delete m_dns;
}


bool tenserflow::HttpClient::connect()
{
    return m_dns->resolve(m_req.host);
}


void tenserflow::HttpClient::onResolved(const Dns &dns, int status)
{
    this->status = status;

    if (status < 0 && dns.isEmpty()) {
        if (!isQuiet()) {
            LOG_ERR("[%s:%d] DNS error: \"%s\"", dns.host().data(), port(), uv_strerror(status));
        }

        return;
    }

    sockaddr *addr = dns.get().addr(port());

    auto req  = new uv_connect_t;
    req->data = this;

    uv_tcp_connect(req, m_tcp, addr, onConnect);

    delete addr;
}


void tenserflow::HttpClient::handshake()
{
    headers.insert({ "Host",       host() });
    headers.insert({ "Connection", "close" });
    headers.insert({ "User-Agent", Platform::userAgent() });

    if (!body.empty()) {
        headers.insert({ "Content-Length", std::to_string(body.size()) });
    }

    std::stringstream ss;
    ss << http_method_str(static_cast<http_method>(method)) << " " << url << " HTTP/1.1" << kCRLF;

    for (auto &header : headers) {
        ss << header.first << ": " << header.second << kCRLF;
    }

    ss << kCRLF;

    headers.clear();

    body.insert(0, ss.str());
    write(std::move(body), false);
}


void tenserflow::HttpClient::read(const char *data, size_t size)
{
    if (parse(data, size) < size) {
        close(UV_EPROTO);
    }
}


void tenserflow::HttpClient::onConnect(uv_connect_t *req, int status)
{
    auto client = static_cast<HttpClient *>(req->data);
    delete req;

    if (!client) {
        return;
    }

    if (status < 0) {
        if (!client->isQuiet()) {
            LOG_ERR("[%s:%d] connect error: \"%s\"", client->m_dns->host().data(), client->port(), uv_strerror(status));
        }

        return client->close(status);
    }

    uv_read_start(client->stream(), NetBuffer::onAlloc,
        [](uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf)
        {
            auto client = static_cast<HttpClient*>(tcp->data);

            if (nread >= 0) {
                client->read(buf->base, static_cast<size_t>(nread));
            } else {
                if (!client->isQuiet() && nread != UV_EOF) {
                    LOG_ERR("[%s:%d] read error: \"%s\"", client->m_dns->host().data(), client->port(), uv_strerror(static_cast<int>(nread)));
                }

                client->close(static_cast<int>(nread));
            }

            NetBuffer::release(buf);
        });

    client->handshake();
}
