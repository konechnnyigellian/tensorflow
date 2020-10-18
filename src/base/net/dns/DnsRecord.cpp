


#include <uv.h>


#include "base/net/dns/DnsRecord.h"


tenserflow::DnsRecord::DnsRecord(const addrinfo *addr) :
    m_type(addr->ai_family == AF_INET6 ? AAAA : A)
{
    char *buf = nullptr;

    if (m_type == AAAA) {
        buf = new char[45]();
        uv_ip6_name(reinterpret_cast<sockaddr_in6*>(addr->ai_addr), buf, 45);
    }
    else {
        buf = new char[16]();
        uv_ip4_name(reinterpret_cast<sockaddr_in*>(addr->ai_addr), buf, 16);
    }

    m_ip = buf;
}


sockaddr *tenserflow::DnsRecord::addr(uint16_t port) const
{
    if (m_type == A) {
        auto addr = new sockaddr_in();
        uv_ip4_addr(m_ip.data(), port, addr);

        return reinterpret_cast<sockaddr *>(addr);
    }

    if (m_type == AAAA) {
        auto addr = new sockaddr_in6();
        uv_ip6_addr(m_ip.data(), port, addr);

        return reinterpret_cast<sockaddr *>(addr);
    }

    return nullptr;
}
