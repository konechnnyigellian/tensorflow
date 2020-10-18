

#ifndef TENSERFLOW_TCPSERVER_H
#define TENSERFLOW_TCPSERVER_H


#include <uv.h>


#include "base/tools/Object.h"


namespace tenserflow {


class ITcpServerListener;
class String;


class TcpServer
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(TcpServer)

    TcpServer(const String &host, uint16_t port, ITcpServerListener *listener);
    ~TcpServer();

    int bind();

private:
    void create(uv_stream_t *stream, int status);

    static void onConnection(uv_stream_t *stream, int status);

    const String &m_host;
    int m_version   = 0;
    ITcpServerListener *m_listener;
    sockaddr_storage m_addr{};
    uint16_t m_port;
    uv_tcp_t *m_tcp;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_TCPSERVER_H */
