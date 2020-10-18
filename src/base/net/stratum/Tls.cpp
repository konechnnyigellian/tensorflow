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


#include "base/net/stratum/Tls.h"
#include "base/io/log/Log.h"
#include "base/net/stratum/Client.h"
#include "base/tools/Buffer.h"


#ifdef _MSC_VER
#   define strncasecmp(x,y,z) _strnicmp(x,y,z)
#endif


#include <cassert>
#include <openssl/ssl.h>


tenserflow::Client::Tls::Tls(Client *client) :
    m_client(client)
{
    m_ctx = SSL_CTX_new(SSLv23_method());
    assert(m_ctx != nullptr);

    if (!m_ctx) {
        return;
    }

    m_write = BIO_new(BIO_s_mem());
    m_read  = BIO_new(BIO_s_mem());
    SSL_CTX_set_options(m_ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
}


tenserflow::Client::Tls::~Tls()
{
    if (m_ctx) {
        SSL_CTX_free(m_ctx);
    }

    if (m_ssl) {
        SSL_free(m_ssl);
    }
}


bool tenserflow::Client::Tls::handshake()
{
    m_ssl = SSL_new(m_ctx);
    assert(m_ssl != nullptr);

    if (!m_ssl) {
        return false;
    }

    SSL_set_connect_state(m_ssl);
    SSL_set_bio(m_ssl, m_read, m_write);
    SSL_do_handshake(m_ssl);

    return send();
}


bool tenserflow::Client::Tls::send(const char *data, size_t size)
{
    SSL_write(m_ssl, data, size);

    return send();
}


const char *tenserflow::Client::Tls::fingerprint() const
{
    return m_ready ? m_fingerprint : nullptr;
}


const char *tenserflow::Client::Tls::version() const
{
    return m_ready ? SSL_get_version(m_ssl) : nullptr;
}


void tenserflow::Client::Tls::read(const char *data, size_t size)
{
    BIO_write(m_read, data, size);

    if (!SSL_is_init_finished(m_ssl)) {
        const int rc = SSL_connect(m_ssl);

        if (rc < 0 && SSL_get_error(m_ssl, rc) == SSL_ERROR_WANT_READ) {
            send();
        } else if (rc == 1) {
            X509 *cert = SSL_get_peer_certificate(m_ssl);
            if (!verify(cert)) {
                X509_free(cert);
                m_client->close();

                return;
            }

            X509_free(cert);
            m_ready = true;
            m_client->login();
      }

      return;
    }

    static char buf[16384]{};
    int bytes_read = 0;

    while ((bytes_read = SSL_read(m_ssl, buf, sizeof(buf))) > 0) {
        m_client->m_reader.parse(buf, static_cast<size_t>(bytes_read));
    }
}


bool tenserflow::Client::Tls::send()
{
    return m_client->send(m_write);
}


bool tenserflow::Client::Tls::verify(X509 *cert)
{
    if (cert == nullptr) {
        LOG_ERR("[%s] Failed to get server certificate", m_client->url());

        return false;
    }

    if (!verifyFingerprint(cert)) {
        LOG_ERR("[%s] Failed to verify server certificate fingerprint", m_client->url());

        const char *fingerprint = m_client->m_pool.fingerprint();
        if (strlen(m_fingerprint) == 64 && fingerprint != nullptr) {
            LOG_ERR("\"%s\" was given", m_fingerprint);
            LOG_ERR("\"%s\" was configured", fingerprint);
        }

        return false;
    }

    return true;
}


bool tenserflow::Client::Tls::verifyFingerprint(X509 *cert)
{
    const EVP_MD *digest = EVP_get_digestbyname("sha256");
    if (digest == nullptr) {
        return false;
    }

    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int dlen;

    if (X509_digest(cert, digest, md, &dlen) != 1) {
        return false;
    }

    Buffer::toHex(md, 32, m_fingerprint);
    const char *fingerprint = m_client->m_pool.fingerprint();

    return fingerprint == nullptr || strncasecmp(m_fingerprint, fingerprint, 64) == 0;
}
