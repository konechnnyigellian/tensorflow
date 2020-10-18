


#include <uv.h>


#include "base/kernel/interfaces/IWatcherListener.h"
#include "base/io/Watcher.h"
#include "base/tools/Handle.h"
#include "base/tools/Timer.h"


tenserflow::Watcher::Watcher(const String &path, IWatcherListener *listener) :
    m_listener(listener),
    m_path(path)
{
    m_timer = new Timer(this);

    m_fsEvent = new uv_fs_event_t;
    m_fsEvent->data = this;
    uv_fs_event_init(uv_default_loop(), m_fsEvent);

    start();
}


tenserflow::Watcher::~Watcher()
{
    delete m_timer;

    Handle::close(m_fsEvent);
}


void tenserflow::Watcher::onFsEvent(uv_fs_event_t *handle, const char *filename, int, int)
{
    if (!filename) {
        return;
    }

    static_cast<Watcher *>(handle->data)->queueUpdate();
}


void tenserflow::Watcher::queueUpdate()
{
    m_timer->stop();
    m_timer->start(kDelay, 0);
}


void tenserflow::Watcher::reload()
{
    m_listener->onFileChanged(m_path);

#   ifndef _WIN32
    uv_fs_event_stop(m_fsEvent);
    start();
#   endif
}


void tenserflow::Watcher::start()
{
    uv_fs_event_start(m_fsEvent, tenserflow::Watcher::onFsEvent, m_path, 0);
}
