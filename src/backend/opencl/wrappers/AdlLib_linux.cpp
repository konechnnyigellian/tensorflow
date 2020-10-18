/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "backend/opencl/wrappers/AdlLib.h"
#include "backend/opencl/wrappers/OclDevice.h"


#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>


namespace tenserflow {


bool AdlLib::m_initialized          = false;
bool AdlLib::m_ready                = false;
static const std::string kPrefix    = "/sys/bus/pci/drivers/amdgpu/";


static inline bool sysfs_is_file(const std::string &path)
{
    struct stat sb;

    return stat(path.c_str(), &sb) == 0 && ((sb.st_mode & S_IFMT) == S_IFREG);
}


static inline bool sysfs_is_amdgpu(const std::string &path)
{
    if (!sysfs_is_file(path)) {
        return false;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::string name;
    std::getline(file, name);

    return name == "amdgpu";
}


uint32_t sysfs_read(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return 0;
    }

    uint32_t value = 0;
    file >> value;

    return value;
}


static inline std::string sysfs_prefix(const PciTopology &topology)
{
    const std::string path = kPrefix + "0000:" + topology.toString().data() + "/hwmon/hwmon";

    for (uint32_t i = 1; i < 10; ++i) {
        const std::string prefix = path + std::to_string(i) + "/";
        if (sysfs_is_amdgpu(prefix + "name") && (sysfs_read(prefix + "temp1_input") || sysfs_read(prefix + "power1_average"))) {
            return prefix;
        }
    }

    return {};
}


} // namespace tenserflow


bool tenserflow::AdlLib::init()
{
    if (!m_initialized) {
        m_ready       = dlopen() && load();
        m_initialized = true;
    }

    return m_ready;
}


const char *tenserflow::AdlLib::lastError() noexcept
{
    return nullptr;
}


void tenserflow::AdlLib::close()
{
}


// https://dri.freedesktop.org/docs/drm/gpu/amdgpu.html#gpu-power-thermal-controls-and-monitoring
AdlHealth tenserflow::AdlLib::health(const OclDevice &device)
{
    if (!isReady() || device.vendorId() != OCL_VENDOR_AMD) {
        return {};
    }

    const auto prefix = sysfs_prefix(device.topology());
    if (prefix.empty()) {
        return {};
    }

    AdlHealth health;
    health.clock        = sysfs_read(prefix + "freq1_input") / 1000000;
    health.memClock     = sysfs_read(prefix + "freq2_input") / 1000000;
    health.power        = sysfs_read(prefix + "power1_average") / 1000000;
    health.rpm          = sysfs_read(prefix + "fan1_input");
    health.temperature  = sysfs_read(prefix + "temp2_input") / 1000;

    if (!health.temperature) {
        health.temperature = sysfs_read(prefix + "temp1_input") / 1000;
    }

    return health;
}


bool tenserflow::AdlLib::dlopen()
{
    struct stat sb;
    if (stat(kPrefix.c_str(), &sb) == -1) {
        return false;
    }

    return (sb.st_mode & S_IFMT) == S_IFDIR;
}


bool tenserflow::AdlLib::load()
{
    return true;
}
