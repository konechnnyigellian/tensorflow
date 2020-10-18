


#include "base/net/dns/Dns.h"
#include "base/kernel/interfaces/IDnsListener.h"


namespace tenserflow {
    Storage<Dns> Dns::m_storage;
    static const DnsRecord defaultRecord;
}


tenserflow::Dns::Dns(IDnsListener *listener) :
    m_listener(listener)
{
    m_key = m_storage.add(this);

    m_resolver = new uv_getaddrinfo_t;
    m_resolver->data = m_storage.ptr(m_key);

    m_hints.ai_family   = AF_UNSPEC;
    m_hints.ai_socktype = SOCK_STREAM;
    m_hints.ai_protocol = IPPROTO_TCP;
}


tenserflow::Dns::~Dns()
{
    m_storage.release(m_key);

    delete m_resolver;
}


bool tenserflow::Dns::resolve(const String &host)
{
    if (m_host != host) {
        m_host = host;

        clear();
    }

    m_status = uv_getaddrinfo(uv_default_loop(), m_resolver, Dns::onResolved, m_host.data(), nullptr, &m_hints);

    return m_status == 0;
}


const char *tenserflow::Dns::error() const
{
    return uv_strerror(m_status);
}


const tenserflow::DnsRecord &tenserflow::Dns::get(DnsRecord::Type prefered) const
{
    if (count() == 0) {
        return defaultRecord;
    }

    const size_t ipv4 = m_ipv4.size();
    const size_t ipv6 = m_ipv6.size();

    if (ipv6 && (prefered == DnsRecord::AAAA || !ipv4)) {
        return m_ipv6[ipv6 == 1 ? 0 : static_cast<size_t>(rand()) % ipv6];
    }

    if (ipv4) {
        return m_ipv4[ipv4 == 1 ? 0 : static_cast<size_t>(rand()) % ipv4];
    }

    return defaultRecord;
}


size_t tenserflow::Dns::count(DnsRecord::Type type) const
{
    if (type == DnsRecord::A) {
        return m_ipv4.size();
    }

    if (type == DnsRecord::AAAA) {
        return m_ipv6.size();
    }

    return m_ipv4.size() + m_ipv6.size();
}


void tenserflow::Dns::clear()
{
    m_ipv4.clear();
    m_ipv6.clear();
}


void tenserflow::Dns::onResolved(int status, addrinfo *res)
{
    m_status = status;

    if (m_status < 0) {
        return m_listener->onResolved(*this, status);
    }

    clear();

    addrinfo *ptr = res;
    while (ptr != nullptr) {
        if (ptr->ai_family == AF_INET) {
            m_ipv4.emplace_back(ptr);
        }

        if (ptr->ai_family == AF_INET6) {
            m_ipv6.emplace_back(ptr);
        }

        ptr = ptr->ai_next;
    }

    if (isEmpty()) {
        m_status = UV_EAI_NONAME;
    }

    m_listener->onResolved(*this, m_status);
}


void tenserflow::Dns::onResolved(uv_getaddrinfo_t *req, int status, addrinfo *res)
{
    Dns *dns = m_storage.get(req->data);
    if (dns) {
        dns->onResolved(status, res);
    }

    uv_freeaddrinfo(res);
}
