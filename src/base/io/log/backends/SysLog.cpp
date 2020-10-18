


#include <syslog.h>


#include "base/io/log/backends/SysLog.h"
#include "version.h"


tenserflow::SysLog::SysLog()
{
    openlog(APP_ID, LOG_PID, LOG_USER);
}


tenserflow::SysLog::~SysLog()
{
    closelog();
}


void tenserflow::SysLog::print(int level, const char *line, size_t offset, size_t, bool colors)
{
    if (colors) {
        return;
    }

    syslog(level == -1 ? LOG_INFO : level, "%s", line + offset);
}
