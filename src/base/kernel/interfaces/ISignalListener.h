

#ifndef TENSERFLOW_ISIGNALLISTENER_H
#define TENSERFLOW_ISIGNALLISTENER_H


namespace tenserflow {


class String;


class ISignalListener
{
public:
    virtual ~ISignalListener() = default;

    virtual void onSignal(int signum) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ISIGNALLISTENER_H
