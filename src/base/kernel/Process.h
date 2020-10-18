

#ifndef TENSERFLOW_PROCESS_H
#define TENSERFLOW_PROCESS_H


#include "base/tools/Arguments.h"


namespace tenserflow {


class Process
{
public:
    enum Location {
        ExeLocation,
        CwdLocation,
        DataLocation,
        HomeLocation,
        TempLocation
    };

#   ifdef WIN32
    constexpr const static char kDirSeparator = '\\';
#   else
    constexpr const static char kDirSeparator = '/';
#   endif

    Process(int argc, char **argv);

    static int pid();
    static int ppid();
    static String exepath();
    static String location(Location location, const char *fileName = nullptr);

    inline const Arguments &arguments() const { return m_arguments; }

private:
    Arguments m_arguments;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_PROCESS_H */
