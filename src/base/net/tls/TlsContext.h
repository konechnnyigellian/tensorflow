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

#ifndef TENSERFLOW_TLSCONTEXT_H
#define TENSERFLOW_TLSCONTEXT_H


#include "base/tools/Object.h"


#include <cstdint>


using SSL_CTX = struct ssl_ctx_st;


namespace tenserflow {


class TlsConfig;


class TlsContext
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(TlsContext)

    ~TlsContext();

    static TlsContext *create(const TlsConfig &config);

    inline SSL_CTX *ctx() const { return m_ctx; }

private:
    TlsContext() = default;

    bool load(const TlsConfig &config);
    bool setCiphers(const char *ciphers);
    bool setCipherSuites(const char *ciphersuites);
    bool setDH(const char *dhparam);
    void setProtocols(uint32_t protocols);

    SSL_CTX *m_ctx = nullptr;
};


} /* namespace tenserflow */

#endif /* TENSERFLOW_TLSCONTEXT_H */
