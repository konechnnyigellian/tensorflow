

#include <sys/stat.h>
#include <unistd.h>


#include "backend/opencl/OclCache.h"


void tenserflow::OclCache::createDirectory()
{
    std::string path = prefix() + "/.cache";
    mkdir(path.c_str(), 0744);
}


std::string tenserflow::OclCache::prefix()
{
    return ".";
}
