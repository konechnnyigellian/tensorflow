

#ifndef TENSERFLOW_SYSLOG_H
#define TENSERFLOW_SYSLOG_H


#include "base/kernel/interfaces/ILogBackend.h"


namespace tenserflow {


class SysLog : public ILogBackend
{
public:
    SysLog();
    ~SysLog() override;

protected:
    void print(int level, const char *line, size_t offset, size_t size, bool colors) override;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_SYSLOG_H */
