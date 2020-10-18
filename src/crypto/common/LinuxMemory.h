

#ifndef TENSERFLOW_LINUXMEMORY_H
#define TENSERFLOW_LINUXMEMORY_H


#include <cstdint>
#include <cstddef>


namespace tenserflow {


class LinuxMemory
{
public:
    static bool reserve(size_t size, uint32_t node, bool oneGbPages = false);

    static bool write(const char *path, uint64_t value);
    static int64_t read(const char *path);
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_LINUXMEMORY_H */
