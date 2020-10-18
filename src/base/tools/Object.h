

#ifndef TENSERFLOW_OBJECT_H
#define TENSERFLOW_OBJECT_H


#include <chrono>


namespace tenserflow {


#define TENSERFLOW_DISABLE_COPY_MOVE(X) \
    X(const X &other)            = delete; \
    X(X &&other)                 = delete; \
    X &operator=(const X &other) = delete; \
    X &operator=(X &&other)      = delete;


#define TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(X) \
    X()                          = delete; \
    X(const X &other)            = delete; \
    X(X &&other)                 = delete; \
    X &operator=(const X &other) = delete; \
    X &operator=(X &&other)      = delete;


} /* namespace tenserflow */

#endif /* TENSERFLOW_OBJECT_H */
