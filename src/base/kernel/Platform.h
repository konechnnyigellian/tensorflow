

#ifndef TENSERFLOW_PLATFORM_H
#define TENSERFLOW_PLATFORM_H


#include <cstdint>


#include "base/tools/String.h"


namespace tenserflow {


class Platform
{
public:
    static inline bool trySetThreadAffinity(int64_t cpu_id)
    {
        if (cpu_id < 0) {
            return false;
        }

        return setThreadAffinity(static_cast<uint64_t>(cpu_id));
    }

    static bool setThreadAffinity(uint64_t cpu_id);
    static void init(const char *userAgent);
    static void setProcessPriority(int priority);
    static void setThreadPriority(int priority);

    static inline const char *userAgent() { return m_userAgent; }

    static bool isOnBatteryPower();

private:
    static char *createUserAgent();

    static String m_userAgent;
};


} // namespace tenserflow


#endif /* TENSERFLOW_PLATFORM_H */
