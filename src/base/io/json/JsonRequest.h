

#ifndef TENSERFLOW_JSONREQUEST_H
#define TENSERFLOW_JSONREQUEST_H


#include "3rdparty/rapidjson/fwd.h"


namespace tenserflow {


class JsonRequest
{
public:
    static const char *kParams;

    static rapidjson::Document create(int64_t id, const char *method);
    static void create(rapidjson::Document &doc, int64_t id, const char *method, rapidjson::Value &params);
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_JSONREQUEST_H */
