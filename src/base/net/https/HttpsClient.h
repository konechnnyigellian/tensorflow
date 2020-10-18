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


#ifndef TENSERFLOW_HTTPSCLIENT_H
#define TENSERFLOW_HTTPSCLIENT_H


using BIO       = struct bio_st;
using SSL_CTX   = struct ssl_ctx_st;
using SSL       = struct ssl_st;
using X509      = struct x509_st;


#include "base/net/http/HttpClient.h"
#include "base/tools/String.h"


namespace tenserflow {


class HttpsClient : public HttpClient
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(HttpsClient)

    HttpsClient(FetchRequest &&req, const std::weak_ptr<IHttpListener> &listener);
    ~HttpsClient() override;

    const char *tlsFingerprint() const override;
    const char *tlsVersion() const override;

protected:
    void handshake() override;
    void read(const char *data, size_t size) override;

private:
    void write(std::string &&data, bool close) override;

    bool verify(X509 *cert);
    bool verifyFingerprint(X509 *cert);
    void flush(bool close);

    BIO *m_read                         = nullptr;
    BIO *m_write                        = nullptr;
    bool m_ready                        = false;
    char m_fingerprint[32 * 2 + 8]{};
    SSL *m_ssl                          = nullptr;
    SSL_CTX *m_ctx                      = nullptr;
};


} // namespace tenserflow


#endif // TENSERFLOW_HTTPSCLIENT_H
