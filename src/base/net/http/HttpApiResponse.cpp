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
 *
 Copyright 2020
 */


#include "base/net/http/HttpApiResponse.h"
#include "3rdparty/http-parser/http_parser.h"
#include "3rdparty/rapidjson/prettywriter.h"
#include "3rdparty/rapidjson/stringbuffer.h"
#include "base/net/http/HttpData.h"


namespace tenserflow {

static const char *kError  = "error";
static const char *kStatus = "status";

} // namespace tenserflow


tenserflow::HttpApiResponse::HttpApiResponse(uint64_t id) :
    HttpResponse(id),
    m_doc(rapidjson::kObjectType)
{
}


tenserflow::HttpApiResponse::HttpApiResponse(uint64_t id, int status) :
    HttpResponse(id),
    m_doc(rapidjson::kObjectType)
{
    setStatus(status);
}


void tenserflow::HttpApiResponse::end()
{
    using namespace rapidjson;

    setHeader("Access-Control-Allow-Origin", "*");
    setHeader("Access-Control-Allow-Methods", "GET, PUT, POST, DELETE");
    setHeader("Access-Control-Allow-Headers", "Authorization, Content-Type");

    if (statusCode() >= 400) {
        if (!m_doc.HasMember(kStatus)) {
            m_doc.AddMember(StringRef(kStatus), statusCode(), m_doc.GetAllocator());
        }

        if (!m_doc.HasMember(kError)) {
            m_doc.AddMember(StringRef(kError), StringRef(http_status_str(static_cast<http_status>(statusCode()))), m_doc.GetAllocator());
        }
    }

    if (!m_doc.MemberCount()) {
        return HttpResponse::end();
    }

    setHeader(HttpData::kContentType, HttpData::kApplicationJson);

    StringBuffer buffer(nullptr, 4096);
    PrettyWriter<StringBuffer> writer(buffer);
    writer.SetMaxDecimalPlaces(10);
    writer.SetFormatOptions(kFormatSingleLineArray);

    m_doc.Accept(writer);

    HttpResponse::end(buffer.GetString(), buffer.GetSize());
}
