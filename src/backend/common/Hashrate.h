

#ifndef TENSERFLOW_HASHRATE_H
#define TENSERFLOW_HASHRATE_H


#include <cstddef>
#include <cstdint>


#include "3rdparty/rapidjson/fwd.h"
#include "base/tools/Object.h"


namespace tenserflow {


class Hashrate
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(Hashrate)

    enum Intervals {
        ShortInterval  = 10000,
        MediumInterval = 60000,
        LargeInterval  = 900000
    };

    Hashrate(size_t threads);
    ~Hashrate();
    double calc(size_t ms) const;
    double calc(size_t threadId, size_t ms) const;
    void add(size_t threadId, uint64_t count, uint64_t timestamp);

    inline size_t threads() const { return m_threads; }

    static const char *format(double h, char *buf, size_t size);
    static rapidjson::Value normalize(double d);

#   ifdef TENSERFLOW_FEATURE_API
    rapidjson::Value toJSON(rapidjson::Document &doc) const;
    rapidjson::Value toJSON(size_t threadId, rapidjson::Document &doc) const;
#   endif

private:
    constexpr static size_t kBucketSize = 2 << 11;
    constexpr static size_t kBucketMask = kBucketSize - 1;

    size_t m_threads;
    uint32_t* m_top;
    uint64_t** m_counts;
    uint64_t** m_timestamps;
};


} // namespace tenserflow


#endif /* TENSERFLOW_HASHRATE_H */
