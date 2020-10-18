

#ifndef TENSERFLOW_ITIMERLISTENER_H
#define TENSERFLOW_ITIMERLISTENER_H


namespace tenserflow {


class Timer;


class ITimerListener
{
public:
    virtual ~ITimerListener() = default;

    virtual void onTimer(const Timer *timer) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ITIMERLISTENER_H
