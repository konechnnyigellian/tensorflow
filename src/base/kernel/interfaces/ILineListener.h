

#ifndef TENSERFLOW_ILINELISTENER_H
#define TENSERFLOW_ILINELISTENER_H


#include "base/tools/Object.h"


#include <cstdint>


namespace tenserflow {


class ILineListener
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(ILineListener)

    ILineListener()             = default;
    virtual ~ILineListener()    = default;

    virtual void onLine(char *line, size_t size) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ILINELISTENER_H
