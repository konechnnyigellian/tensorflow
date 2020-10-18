

#ifndef TENSERFLOW_DNSRECORD_H
#define TENSERFLOW_DNSRECORD_H


struct addrinfo;
struct sockaddr;


#include "base/tools/String.h"


namespace tenserflow {


class DnsRecord
{
public:
    enum Type {
        Unknown,
        A,
        AAAA
    };

    DnsRecord() {}
    DnsRecord(const addrinfo *addr);

    sockaddr *addr(uint16_t port = 0) const;

    inline bool isValid() const     { return m_type != Unknown; }
    inline const String &ip() const { return m_ip; }
    inline Type type() const        { return m_type; }

private:
    Type m_type = Unknown;
    String m_ip;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_DNSRECORD_H */
