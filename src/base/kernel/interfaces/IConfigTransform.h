

#ifndef TENSERFLOW_ICONFIGTRANSFORM_H
#define TENSERFLOW_ICONFIGTRANSFORM_H


#include "3rdparty/rapidjson/fwd.h"


namespace tenserflow {


class IJsonReader;
class String;


class IConfigTransform
{
public:
    virtual ~IConfigTransform() = default;

    virtual void finalize(rapidjson::Document &doc)                             = 0;
    virtual void transform(rapidjson::Document &doc, int key, const char *arg)  = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ICONFIGTRANSFORM_H
