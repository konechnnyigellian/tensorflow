

#ifndef TENSERFLOW_CHRONO_H
#define TENSERFLOW_CHRONO_H


#include <chrono>


namespace tenserflow {


class Chrono
{
public:
    static inline uint64_t highResolutionMSecs()
    {
        using namespace std::chrono;

        return static_cast<uint64_t>(time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch().count());
    }


    static inline uint64_t steadyMSecs()
    {
        using namespace std::chrono;
        if (high_resolution_clock::is_steady) {
            return static_cast<uint64_t>(time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch().count());
        }

        return static_cast<uint64_t>(time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count());
    }


    static inline uint64_t currentMSecsSinceEpoch()
    {
        using namespace std::chrono;

        return static_cast<uint64_t>(time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count());
    }
};


} /* namespace tenserflow */

#endif /* TENSERFLOW_CHRONO_H */
