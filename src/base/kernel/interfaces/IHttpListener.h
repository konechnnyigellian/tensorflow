

#ifndef TENSERFLOW_IHTTPLISTENER_H
#define TENSERFLOW_IHTTPLISTENER_H


namespace tenserflow {


class HttpData;
class HttpResponse;


class IHttpListener
{
public:
    virtual ~IHttpListener() = default;

    virtual void onHttpData(const HttpData &data) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IHTTPLISTENER_H
