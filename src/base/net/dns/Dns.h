

#ifndef TENSERFLOW_DNS_H
#define TENSERFLOW_DNS_H


#include <vector>
#include <uv.h>


#include "base/net/dns/DnsRecord.h"
#include "base/net/tools/Storage.h"
#include "base/tools/Object.h"
#include "base/tools/String.h"


namespace tenserflow {


class IDnsListener;


class Dns
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(Dns)

    Dns(IDnsListener *listener);
    ~Dns();

    inline bool isEmpty() const       { return m_ipv4.empty() && m_ipv6.empty(); }
    inline const String &host() const { return m_host; }
    inline int status() const         { return m_status; }

    bool resolve(const String &host);
    const char *error() const;
    const DnsRecord &get(DnsRecord::Type prefered = DnsRecord::A) const;
    size_t count(DnsRecord::Type type = DnsRecord::Unknown) const;

private:
    void clear();
    void onResolved(int status, addrinfo *res);

    static void onResolved(uv_getaddrinfo_t *req, int status, addrinfo *res);

    addrinfo m_hints{};
    IDnsListener *m_listener;
    int m_status                    = 0;
    std::vector<DnsRecord> m_ipv4;
    std::vector<DnsRecord> m_ipv6;
    String m_host;
    uintptr_t m_key;
    uv_getaddrinfo_t *m_resolver    = nullptr;

    static Storage<Dns> m_storage;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_DNS_H */
