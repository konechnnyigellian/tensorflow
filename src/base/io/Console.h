

#ifndef TENSERFLOW_CONSOLE_H
#define TENSERFLOW_CONSOLE_H


#include "base/tools/Object.h"


#include <uv.h>


namespace tenserflow {


class IConsoleListener;


class Console
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(Console)

    Console(IConsoleListener *listener);
    ~Console();

    void stop();

private:
    bool isSupported() const;

    static void onAllocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void onRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

    char m_buf[1] = { 0 };
    IConsoleListener *m_listener;
    uv_tty_t *m_tty = nullptr;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CONSOLE_H */
