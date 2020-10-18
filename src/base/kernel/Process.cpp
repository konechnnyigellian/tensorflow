


#include <ctime>
#include <string>
#include <uv.h>


#include "base/kernel/Process.h"
#include "base/tools/Chrono.h"


namespace tenserflow {


static char pathBuf[520];
static std::string dataDir;


static std::string getPath(Process::Location location)
{
    size_t size = sizeof(pathBuf);

    if (location == Process::DataLocation) {
        if (!dataDir.empty()) {
            return dataDir;
        }

        location = Process::ExeLocation;
    }

    if (location == Process::HomeLocation) {
#       if UV_VERSION_HEX >= 0x010600
        return uv_os_homedir(pathBuf, &size) < 0 ? "" : std::string(pathBuf, size);
#       else
        location = Process::ExeLocation;
#       endif
    }

    if (location == Process::TempLocation) {
#       if UV_VERSION_HEX >= 0x010900
        return uv_os_tmpdir(pathBuf, &size) < 0 ? "" : std::string(pathBuf, size);
#       else
        location = Process::ExeLocation;
#       endif
    }

    if (location == Process::ExeLocation) {
        if (uv_exepath(pathBuf, &size) < 0) {
            return {};
        }

        const auto path = std::string(pathBuf, size);
        const auto pos  = path.rfind(Process::kDirSeparator);

        if (pos != std::string::npos) {
            return path.substr(0, pos);
        }

        return path;
    }

    if (location == Process::CwdLocation) {
        return uv_cwd(pathBuf, &size) < 0 ? "" : std::string(pathBuf, size);
    }

    return {};
}


static void setDataDir(const char *path)
{
    if (path == nullptr) {
        return;
    }

    std::string dir = path;
    if (!dir.empty() && (dir.back() == '/' || dir.back() == '\\')) {
        dir.pop_back();
    }

    if (!dir.empty() && uv_chdir(dir.c_str()) == 0) {
        dataDir = dir;
    }
}


} // namespace tenserflow


tenserflow::Process::Process(int argc, char **argv) :
    m_arguments(argc, argv)
{
    srand(static_cast<unsigned int>(Chrono::currentMSecsSinceEpoch() ^ reinterpret_cast<uintptr_t>(this)));

    setDataDir(m_arguments.value("--data-dir", "-d"));
}


int tenserflow::Process::pid()
{
#   if UV_VERSION_HEX >= 0x011200
    return uv_os_getpid();
#   else
    return 0;
#   endif
}


int tenserflow::Process::ppid()
{
#   if UV_VERSION_HEX >= 0x011000
    return uv_os_getppid();
#   else
    return 0;
#   endif
}


tenserflow::String tenserflow::Process::exepath()
{
    size_t size = sizeof(pathBuf);

    return uv_exepath(pathBuf, &size) < 0 ? "" : String(pathBuf, size);
}


tenserflow::String tenserflow::Process::location(Location location, const char *fileName)
{
    auto path = getPath(location);
    if (path.empty() || fileName == nullptr) {
        return path.c_str();
    }

    return (path + kDirSeparator + fileName).c_str();
}
