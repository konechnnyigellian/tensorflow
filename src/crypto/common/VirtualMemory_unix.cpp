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


#include <cstdlib>
#include <sys/mman.h>


#include "backend/cpu/Cpu.h"
#include "crypto/common/portable/mm_malloc.h"
#include "crypto/common/VirtualMemory.h"


#if defined(__APPLE__)
#   include <mach/vm_statistics.h>
#endif


#if defined(TENSERFLOW_OS_LINUX)
#   if (defined(MAP_HUGE_1GB) || defined(MAP_HUGE_SHIFT))
#       define TENSERFLOW_HAS_1GB_PAGES
#   endif
#   include "crypto/common/LinuxMemory.h"
#endif


bool tenserflow::VirtualMemory::isHugepagesAvailable()
{
    return true;
}


bool tenserflow::VirtualMemory::isOneGbPagesAvailable()
{
#   ifdef TENSERFLOW_HAS_1GB_PAGES
    return Cpu::info()->hasOneGbPages();
#   else
    return false;
#   endif
}


void *tenserflow::VirtualMemory::allocateExecutableMemory(size_t size)
{
#   if defined(__APPLE__)
    void *mem = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANON, -1, 0);
#   else
    void *mem = mmap(0, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#   endif

    return mem == MAP_FAILED ? nullptr : mem;
}


void *tenserflow::VirtualMemory::allocateLargePagesMemory(size_t size)
{
#   if defined(__APPLE__)
    void *mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, VM_FLAGS_SUPERPAGE_SIZE_2MB, 0);
#   elif defined(__FreeBSD__)
    void *mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED_SUPER | MAP_PREFAULT_READ, -1, 0);
#   else

#   if defined(MAP_HUGE_2MB)
    constexpr int flag_2mb = MAP_HUGE_2MB;
#   elif defined(MAP_HUGE_SHIFT)
    constexpr int flag_2mb = (21 << MAP_HUGE_SHIFT);
#   else
    constexpr int flag_2mb = 0;
#   endif

    void *mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE | flag_2mb, 0, 0);

#   endif

    return mem == MAP_FAILED ? nullptr : mem;
}


void *tenserflow::VirtualMemory::allocateOneGbPagesMemory(size_t size)
{
#   ifdef TENSERFLOW_HAS_1GB_PAGES
    if (isOneGbPagesAvailable()) {
#       if defined(MAP_HUGE_1GB)
        constexpr int flag_1gb = MAP_HUGE_1GB;
#       elif defined(MAP_HUGE_SHIFT)
        constexpr int flag_1gb = (30 << MAP_HUGE_SHIFT);
#       else
        constexpr int flag_1gb = 0;
#       endif

        void *mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE | flag_1gb, 0, 0);

        return mem == MAP_FAILED ? nullptr : mem;
    }
#   endif

    return nullptr;
}


void tenserflow::VirtualMemory::flushInstructionCache(void *p, size_t size)
{
#   ifdef HAVE_BUILTIN_CLEAR_CACHE
    __builtin___clear_cache(reinterpret_cast<char*>(p), reinterpret_cast<char*>(p) + size);
#   endif
}


void tenserflow::VirtualMemory::freeLargePagesMemory(void *p, size_t size)
{
    munmap(p, size);
}


void tenserflow::VirtualMemory::protectExecutableMemory(void *p, size_t size)
{
    mprotect(p, size, PROT_READ | PROT_EXEC);
}


void tenserflow::VirtualMemory::unprotectExecutableMemory(void *p, size_t size)
{
    mprotect(p, size, PROT_WRITE | PROT_EXEC);
}


void tenserflow::VirtualMemory::osInit(bool)
{
}


bool tenserflow::VirtualMemory::allocateLargePagesMemory()
{
#   if defined(TENSERFLOW_OS_LINUX)
    LinuxMemory::reserve(m_size, m_node);
#   endif

    m_scratchpad = static_cast<uint8_t*>(allocateLargePagesMemory(m_size));
    if (m_scratchpad) {
        m_flags.set(FLAG_HUGEPAGES, true);

        madvise(m_scratchpad, m_size, MADV_RANDOM | MADV_WILLNEED);

        if (mlock(m_scratchpad, m_size) == 0) {
            m_flags.set(FLAG_LOCK, true);
        }

        return true;
    }

    return false;
}


bool tenserflow::VirtualMemory::allocateOneGbPagesMemory()
{
#   if defined(TENSERFLOW_HAS_1GB_PAGES)
    LinuxMemory::reserve(m_size, m_node, true);
#   endif

    m_scratchpad = static_cast<uint8_t*>(allocateOneGbPagesMemory(m_size));
    if (m_scratchpad) {
        m_flags.set(FLAG_1GB_PAGES, true);

        madvise(m_scratchpad, m_size, MADV_RANDOM | MADV_WILLNEED);

        if (mlock(m_scratchpad, m_size) == 0) {
            m_flags.set(FLAG_LOCK, true);
        }

        return true;
    }

    return false;
}


void tenserflow::VirtualMemory::freeLargePagesMemory()
{
    if (m_flags.test(FLAG_LOCK)) {
        munlock(m_scratchpad, m_size);
    }

    freeLargePagesMemory(m_scratchpad, m_size);
}
