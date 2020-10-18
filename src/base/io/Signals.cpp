


#include <uv.h>


#include "base/io/Signals.h"
#include "base/kernel/interfaces/ISignalListener.h"
#include "base/tools/Handle.h"


static const int signums[tenserflow::Signals::kSignalsCount] = { SIGHUP, SIGINT, SIGTERM };


tenserflow::Signals::Signals(ISignalListener *listener)
    : m_listener(listener)
{
    for (size_t i = 0; i < kSignalsCount; ++i) {
        uv_signal_t *signal = new uv_signal_t;
        signal->data        = this;

        m_signals[i] = signal;

        uv_signal_init(uv_default_loop(), signal);
        uv_signal_start(signal, Signals::onSignal, signums[i]);
    }
}


tenserflow::Signals::~Signals()
{
    stop();
}


void tenserflow::Signals::stop()
{
    if (!m_signals[0]) {
        return;
    }

    for (size_t i = 0; i < kSignalsCount; ++i) {
        Handle::close(m_signals[i]);
        m_signals[i] = nullptr;
    }
}


void tenserflow::Signals::onSignal(uv_signal_t *handle, int signum)
{
    static_cast<Signals *>(handle->data)->m_listener->onSignal(signum);
}
