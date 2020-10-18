


#include <algorithm>


#include "backend/opencl/OclThreads.h"
#include "3rdparty/rapidjson/document.h"
#include "base/io/json/Json.h"


tenserflow::OclThreads::OclThreads(const rapidjson::Value &value)
{
    if (value.IsArray()) {
        for (auto &v : value.GetArray()) {
            OclThread thread(v);
            if (thread.isValid()) {
                add(std::move(thread));
            }
        }
    }
}


tenserflow::OclThreads::OclThreads(const std::vector<OclDevice> &devices, const Algorithm &algorithm)
{
    for (const auto &device : devices) {
        device.generate(algorithm, *this);
    }
}


bool tenserflow::OclThreads::isEqual(const OclThreads &other) const
{
    if (isEmpty() && other.isEmpty()) {
        return true;
    }

    return count() == other.count() && std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
}


rapidjson::Value tenserflow::OclThreads::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    Value out(kArrayType);

    out.SetArray();

    for (const OclThread &thread : m_data) {
        out.PushBack(thread.toJSON(doc), allocator);
    }

    return out;
}
