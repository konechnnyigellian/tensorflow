/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_HTTPLISTENER_H
#define TENSERFLOW_HTTPLISTENER_H


#include "base/kernel/interfaces/IHttpListener.h"


namespace tenserflow {


class HttpListener : public IHttpListener
{
public:
    inline HttpListener(IHttpListener *listener, const char *tag = nullptr) :
#       ifdef APP_DEBUG
        m_tag(tag),
#       endif
        m_listener(listener)
    {}

protected:
    void onHttpData(const HttpData &data) override;

private:
#   ifdef APP_DEBUG
    const char *m_tag;
#   endif
    IHttpListener *m_listener;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_HTTPLISTENER_H
