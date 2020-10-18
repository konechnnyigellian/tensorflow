/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_VIRTUALMEMORY_H
#define TENSERFLOW_VIRTUALMEMORY_H


#include "base/tools/Object.h"
#include "crypto/common/HugePagesInfo.h"


#include <bitset>
#include <cstddef>
#include <cstdint>
#include <utility>


namespace tenserflow {


class VirtualMemory
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(VirtualMemory)

    VirtualMemory(size_t size, bool hugePages, bool oneGbPages, bool usePool, uint32_t node = 0, size_t alignSize = 64);
    ~VirtualMemory();

    inline bool isHugePages() const     { return m_flags.test(FLAG_HUGEPAGES); }
    inline bool isOneGbPages() const    { return m_flags.test(FLAG_1GB_PAGES); }
    inline size_t size() const          { return m_size; }
    inline size_t capacity() const      { return m_capacity; }
    inline uint8_t *raw() const         { return m_scratchpad; }
    inline uint8_t *scratchpad() const  { return m_scratchpad; }

    HugePagesInfo hugePages() const;

    static bool isHugepagesAvailable();
    static bool isOneGbPagesAvailable();
    static uint32_t bindToNUMANode(int64_t affinity);
    static void *allocateExecutableMemory(size_t size);
    static void *allocateLargePagesMemory(size_t size);
    static void *allocateOneGbPagesMemory(size_t size);
    static void destroy();
    static void flushInstructionCache(void *p, size_t size);
    static void freeLargePagesMemory(void *p, size_t size);
    static void init(size_t poolSize, bool hugePages);
    static void protectExecutableMemory(void *p, size_t size);
    static void unprotectExecutableMemory(void *p, size_t size);

    static inline constexpr size_t align(size_t pos, size_t align = 2097152) { return ((pos - 1) / align + 1) * align; }

private:
    enum Flags {
        FLAG_HUGEPAGES,
        FLAG_1GB_PAGES,
        FLAG_LOCK,
        FLAG_EXTERNAL,
        FLAG_MAX
    };

    static void osInit(bool hugePages);

    bool allocateLargePagesMemory();
    bool allocateOneGbPagesMemory();
    void freeLargePagesMemory();

    const size_t m_size;
    size_t m_capacity;
    const uint32_t m_node;
    std::bitset<FLAG_MAX> m_flags;
    uint8_t *m_scratchpad = nullptr;
};


} /* namespace tenserflow */



#endif /* TENSERFLOW_VIRTUALMEMORY_H */
