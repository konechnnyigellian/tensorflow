

#ifndef TENSERFLOW_SIGNALS_H
#define TENSERFLOW_SIGNALS_H


#include <stddef.h>


typedef struct uv_signal_s uv_signal_t;


namespace tenserflow {


class ISignalListener;


class Signals
{
public:
    constexpr static const size_t kSignalsCount = 3;

    Signals(ISignalListener *listener);
    ~Signals();

    void stop();

private:
    void close(int signum);

    static void onSignal(uv_signal_t *handle, int signum);

    ISignalListener *m_listener;
    uv_signal_t *m_signals[kSignalsCount];
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_SIGNALS_H */
