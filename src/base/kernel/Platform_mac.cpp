/* TENSERflow
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


#include <IOKit/IOKitLib.h>
#include <IOKit/ps/IOPowerSources.h>
#include <mach/thread_act.h>
#include <mach/thread_policy.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <uv.h>
#include <thread>
#include <fstream>


#include "base/kernel/Platform.h"
#include "version.h"


char *tenserflow::Platform::createUserAgent()
{
    constexpr const size_t max = 256;

    char *buf = new char[max]();
    int length = snprintf(buf, max, "%s/%s (Macintosh; Intel Mac OS X) libuv/%s", APP_NAME, APP_VERSION, uv_version_string());

#   ifdef __clang__
    length += snprintf(buf + length, max - length, " clang/%d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
#   elif defined(__GNUC__)
    length += snprintf(buf + length, max - length, " gcc/%d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#   endif

    return buf;
}


#ifndef TENSERFLOW_FEATURE_HWLOC
bool tenserflow::Platform::setThreadAffinity(uint64_t cpu_id)
{
    thread_port_t mach_thread;
    thread_affinity_policy_data_t policy = { static_cast<integer_t>(cpu_id) };
    mach_thread = pthread_mach_thread_np(pthread_self());

    const bool result = (thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1) == KERN_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return result;
}
#endif


void tenserflow::Platform::setProcessPriority(int)
{
}


void tenserflow::Platform::setThreadPriority(int priority)
{
    if (priority == -1) {
        return;
    }

    int prio = 19;
    switch (priority)
    {
    case 1:
        prio = 5;
        break;

    case 2:
        prio = 0;
        break;

    case 3:
        prio = -5;
        break;

    case 4:
        prio = -10;
        break;

    case 5:
        prio = -15;
        break;

    default:
        break;
    }

    setpriority(PRIO_PROCESS, 0, prio);
}


bool tenserflow::Platform::isOnBatteryPower()
{
    return IOPSGetTimeRemainingEstimate() != kIOPSTimeRemainingUnlimited;
}
