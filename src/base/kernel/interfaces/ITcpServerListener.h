

#ifndef TENSERFLOW_ITCPSERVERLISTENER_H
#define TENSERFLOW_ITCPSERVERLISTENER_H


#include <stdint.h>


typedef struct uv_stream_s uv_stream_t;


namespace tenserflow {


class String;


class ITcpServerListener
{
public:
    virtual ~ITcpServerListener() = default;

    virtual void onConnection(uv_stream_t *stream, uint16_t port) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ITCPSERVERLISTENER_H
