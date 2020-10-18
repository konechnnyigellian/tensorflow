


#include "base/api/requests/ApiRequest.h"


tenserflow::ApiRequest::ApiRequest(Source source, bool restricted) :
    m_restricted(restricted),
    m_source(source)
{
}


tenserflow::ApiRequest::~ApiRequest() = default;
