


#include "backend/common/Threads.h"
#include "3rdparty/rapidjson/document.h"
#include "backend/cpu/CpuThreads.h"
#include "crypto/cn/CnAlgo.h"


#ifdef TENSERFLOW_FEATURE_OPENCL
#   include "backend/opencl/OclThreads.h"
#endif


#ifdef TENSERFLOW_FEATURE_CUDA
#   include "backend/cuda/CudaThreads.h"
#endif


namespace tenserflow {


static const char *kAsterisk = "*";
static const char *kCn2      = "cn/2";


} // namespace tenserflow


template <class T>
const T &tenserflow::Threads<T>::get(const String &profileName) const
{
    static T empty;
    if (profileName.isNull() || !has(profileName)) {
        return empty;
    }

    return m_profiles.at(profileName);
}


template <class T>
size_t tenserflow::Threads<T>::read(const rapidjson::Value &value)
{
    using namespace rapidjson;

    for (auto &member : value.GetObject()) {
        if (member.value.IsArray() || member.value.IsObject()) {
            T threads(member.value);

            if (!threads.isEmpty()) {
                move(member.name.GetString(), std::move(threads));
            }
        }
    }

    for (auto &member : value.GetObject()) {
        if (member.value.IsArray() || member.value.IsObject()) {
            continue;
        }

        const Algorithm algo(member.name.GetString());
        if (!algo.isValid()) {
            continue;
        }

        if (member.value.IsBool() && member.value.IsFalse()) {
            disable(algo);
            continue;
        }

        if (member.value.IsString()) {
            if (has(member.value.GetString())) {
                m_aliases.insert({ algo, member.value.GetString() });
            }
            else {
                m_disabled.insert(algo);
            }
        }
    }

    return m_profiles.size();
}


template <class T>
tenserflow::String tenserflow::Threads<T>::profileName(const Algorithm &algorithm, bool strict) const
{
    if (isDisabled(algorithm)) {
        return String();
    }

    const String name = algorithm.shortName();
    if (has(name)) {
        return name;
    }

    if (m_aliases.count(algorithm) > 0) {
        return m_aliases.at(algorithm);
    }

    if (strict) {
        return String();
    }

    if (algorithm.family() == Algorithm::CN && CnAlgo<>::base(algorithm) == Algorithm::CN_2 && has(kCn2)) {
        return kCn2;
    }

    if (name.contains("/")) {
        const String base = name.split('/').at(0);
        if (has(base)) {
            return base;
        }
    }

    if (has(kAsterisk)) {
        return kAsterisk;
    }

    return String();
}


template <class T>
void tenserflow::Threads<T>::toJSON(rapidjson::Value &out, rapidjson::Document &doc) const
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    for (const auto &kv : m_profiles) {
        out.AddMember(kv.first.toJSON(), kv.second.toJSON(doc), allocator);
    }

    for (const Algorithm &algo : m_disabled) {
        out.AddMember(StringRef(algo.shortName()), false, allocator);
    }

    for (const auto &kv : m_aliases) {
        out.AddMember(StringRef(kv.first.shortName()), kv.second.toJSON(), allocator);
    }
}


namespace tenserflow {

template class Threads<CpuThreads>;

#ifdef TENSERFLOW_FEATURE_OPENCL
template class Threads<OclThreads>;
#endif

#ifdef TENSERFLOW_FEATURE_CUDA
template class Threads<CudaThreads>;
#endif

} // namespace tenserflow
