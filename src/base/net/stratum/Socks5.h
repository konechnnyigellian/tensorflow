/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_SOCKS5_H
#define TENSERFLOW_SOCKS5_H


#include "base/net/stratum/Client.h"


namespace tenserflow {


class Client::Socks5
{
public:
    Socks5(Client *client);

    inline bool isReady() const     { return m_state == Ready; }

    bool read(const char *data, size_t size);
    void handshake();

private:
    enum State {
        Created,
        SentInitialHandshake,
        SentFinalHandshake,
        Ready
    };

    bool isIPv4(const String &host, sockaddr_storage *addr) const;
    bool isIPv6(const String &host, sockaddr_storage *addr) const;
    void connect();

    Client *m_client;
    size_t m_nextSize   = 0;
    State m_state       = Created;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_SOCKS5_H */
