


#include "crypto/common/HugePagesInfo.h"
#include "crypto/common/VirtualMemory.h"


namespace tenserflow {

constexpr size_t twoMiB = 2U * 1024U * 1024U;
constexpr size_t oneGiB = 1024U * 1024U * 1024U;

} // namespace tenserflow


tenserflow::HugePagesInfo::HugePagesInfo(const VirtualMemory *memory)
{
    if (memory->isOneGbPages()) {
        size        = VirtualMemory::align(memory->size(), oneGiB);
        total       = size / oneGiB;
        allocated   = size / oneGiB;
    }
    else {
        size        = memory->size();
        total       = size / twoMiB;
        allocated   = memory->isHugePages() ? total : 0;
    }
}
