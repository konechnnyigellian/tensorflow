

#ifndef TENSERFLOW_ILOGBACKEND_H
#define TENSERFLOW_ILOGBACKEND_H


#include <stdarg.h>
#include <stddef.h>


namespace tenserflow {


class ILogBackend
{
public:
    virtual ~ILogBackend() = default;

    virtual void print(int level, const char *line, size_t offset, size_t size, bool colors) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ILOGBACKEND_H
