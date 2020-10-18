


//#include <iostream>

#include "crypto/common/LinuxMemory.h"
#include "base/io/log/Log.h"
#include "crypto/common/VirtualMemory.h"
#include "backend/cpu/Cpu.h"


#include <algorithm>
#include <fstream>
#include <string>
#include <mutex>


namespace tenserflow {


static std::mutex mutex;
constexpr size_t twoMiB = 2U * 1024U * 1024U;
constexpr size_t oneGiB = 1024U * 1024U * 1024U;


static inline std::string sysfs_path(uint32_t node, bool oneGbPages, bool nr)
{
    return "/sys/devices/system/node/node" + std::to_string(node) + "/hugepages/hugepages-" + (oneGbPages ? "1048576" : "2048") + "kB/" + (nr ? "nr" : "free") + "_hugepages";
}


static inline bool write_nr_hugepages(uint32_t node, bool oneGbPages, uint64_t count)    { return LinuxMemory::write(sysfs_path(node, oneGbPages, true).c_str(), count); }
static inline int64_t free_hugepages(uint32_t node, bool oneGbPages)                     { return LinuxMemory::read(sysfs_path(node, oneGbPages, false).c_str()); }
static inline int64_t nr_hugepages(uint32_t node, bool oneGbPages)                       { return LinuxMemory::read(sysfs_path(node, oneGbPages, true).c_str()); }


} // namespace tenserflow


bool tenserflow::LinuxMemory::reserve(size_t size, uint32_t node, bool oneGbPages)
{
    std::lock_guard<std::mutex> lock(mutex);

    const size_t pageSize = oneGbPages ? oneGiB : twoMiB;
    const size_t required = VirtualMemory::align(size, pageSize) / pageSize;

    const auto available = free_hugepages(node, oneGbPages);
    if (available < 0 || static_cast<size_t>(available) >= required) {
        return false;
    }

    return write_nr_hugepages(node, oneGbPages, std::max<size_t>(nr_hugepages(node, oneGbPages), 0) + (required - available));
}


bool tenserflow::LinuxMemory::write(const char *path, uint64_t value)
{
    std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    file << value;
    file.flush();

    return true;
}


int64_t tenserflow::LinuxMemory::read(const char *path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return -1;
    }

    uint64_t value = 0;
    file >> value;

    return value;
}
