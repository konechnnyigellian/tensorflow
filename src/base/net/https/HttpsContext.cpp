/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/net/https/HttpsContext.h"
#include "3rdparty/http-parser/http_parser.h"
#include "base/net/tls/TlsContext.h"


#include <openssl/bio.h>
#include <uv.h>


tenserflow::HttpsContext::HttpsContext(TlsContext *tls, const std::weak_ptr<IHttpListener> &listener) :
    HttpContext(HTTP_REQUEST, listener),
    ServerTls(tls ? tls->ctx() : nullptr)
{
    if (!tls) {
        m_mode = TLS_OFF;
    }
}


tenserflow::HttpsContext::~HttpsContext() = default;


void tenserflow::HttpsContext::append(char *data, size_t size)
{
    if (m_mode == TLS_AUTO) {
        m_mode = isTLS(data, size) ? TLS_ON : TLS_OFF;
    }

    if (m_mode == TLS_ON) {
        read(data, size);
    }
    else {
        parse(data, size);
    }
}


bool tenserflow::HttpsContext::write(BIO *bio)
{
    if (uv_is_writable(stream()) != 1) {
        return false;
    }

    char *data        = nullptr;
    const size_t size = BIO_get_mem_data(bio, &data);
    std::string body(data, size);

    (void) BIO_reset(bio);

    HttpContext::write(std::move(body), m_close);

    return true;
}


void tenserflow::HttpsContext::parse(char *data, size_t size)
{
    if (HttpContext::parse(data, size) < size) {
        close();
    }
}


void tenserflow::HttpsContext::shutdown()
{
    close();
}


void tenserflow::HttpsContext::write(std::string &&data, bool close)
{
    m_close = close;

    if (m_mode == TLS_ON) {
        send(data.data(), data.size());
    }
    else {
        HttpContext::write(std::move(data), close);
    }
}
