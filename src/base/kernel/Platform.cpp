


#include "base/kernel/Platform.h"


#include <cstring>
#include <uv.h>


#ifdef TENSERFLOW_FEATURE_TLS
#   include <openssl/ssl.h>
#   include <openssl/err.h>
#endif


namespace tenserflow {

String Platform::m_userAgent;

} // namespace tenserflow


void tenserflow::Platform::init(const char *userAgent)
{
#   ifdef TENSERFLOW_FEATURE_TLS
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_digests();
#   endif

    if (userAgent) {
        m_userAgent = userAgent;
    }
    else {
        m_userAgent = createUserAgent();
    }
}
