/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/net/http/HttpData.h"


namespace tenserflow {


const std::string HttpData::kApplicationJson    = "application/json";
const std::string HttpData::kContentType        = "Content-Type";
const std::string HttpData::kContentTypeL       = "content-type";
const std::string HttpData::kTextPlain          = "text/plain";


} // namespace tenserflow


bool tenserflow::HttpData::isJSON() const
{
    if (!headers.count(kContentTypeL)) {
        return false;
    }

    auto &type = headers.at(kContentTypeL);

    return type == kApplicationJson || type == kTextPlain;
}
