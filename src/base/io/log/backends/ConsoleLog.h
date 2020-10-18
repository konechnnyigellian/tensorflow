/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_CONSOLELOG_H
#define TENSERFLOW_CONSOLELOG_H


using uv_stream_t = struct uv_stream_s;
using uv_tty_t    = struct uv_tty_s;


#include "base/kernel/interfaces/ILogBackend.h"
#include "base/tools/Object.h"


namespace tenserflow {


class Title;


class ConsoleLog : public ILogBackend
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(ConsoleLog)

    ConsoleLog(const Title &title);
    ~ConsoleLog() override;

protected:
    void print(int level, const char *line, size_t offset, size_t size, bool colors) override;

private:
    bool isSupported() const;

    uv_tty_t *m_tty = nullptr;

#   ifdef TENSERFLOW_OS_WIN
    bool isWritable() const;

    uv_stream_t *m_stream = nullptr;
#   endif
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CONSOLELOG_H */
