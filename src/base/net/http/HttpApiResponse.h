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


#ifndef TENSERFLOW_HTTPAPIRESPONSE_H
#define TENSERFLOW_HTTPAPIRESPONSE_H


#include "3rdparty/rapidjson/document.h"
#include "base/net/http/HttpResponse.h"


namespace tenserflow {


class HttpApiResponse : public HttpResponse
{
public:
    HttpApiResponse(uint64_t id);
    HttpApiResponse(uint64_t id, int status);

    inline rapidjson::Document &doc() { return m_doc; }

    void end();

private:
    rapidjson::Document m_doc;
};


} // namespace tenserflow


#endif // TENSERFLOW_HTTPAPIRESPONSE_H

