


#include "base/io/json/JsonRequest.h"
#include "3rdparty/rapidjson/document.h"


namespace tenserflow {


static const char *k2_0             = "2.0";
static const char *kId              = "id";
static const char *kJsonRPC         = "jsonrpc";
static const char *kMethod          = "method";
const char *JsonRequest::kParams    = "params";


} // namespace tenserflow


rapidjson::Document tenserflow::JsonRequest::create(int64_t id, const char *method)
{
    using namespace rapidjson;
    Document doc(kObjectType);
    auto &allocator = doc.GetAllocator();

    doc.AddMember(StringRef(kId),      id, allocator);
    doc.AddMember(StringRef(kJsonRPC), StringRef(k2_0), allocator);
    doc.AddMember(StringRef(kMethod),  StringRef(method), allocator);

    return doc;
}


void tenserflow::JsonRequest::create(rapidjson::Document &doc, int64_t id, const char *method, rapidjson::Value &params)
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    doc.AddMember(StringRef(kId),      id, allocator);
    doc.AddMember(StringRef(kJsonRPC), StringRef(k2_0), allocator);
    doc.AddMember(StringRef(kMethod),  StringRef(method), allocator);
    doc.AddMember(StringRef(kParams),  params, allocator);
}
