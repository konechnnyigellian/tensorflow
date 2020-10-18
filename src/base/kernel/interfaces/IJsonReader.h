

#ifndef TENSERFLOW_IJSONREADER_H
#define TENSERFLOW_IJSONREADER_H


#include "3rdparty/rapidjson/fwd.h"


namespace tenserflow {


class IJsonReader
{
public:
    virtual ~IJsonReader() = default;

    virtual bool getBool(const char *key, bool defaultValue = false) const                       = 0;
    virtual bool isEmpty() const                                                                 = 0;
    virtual const char *getString(const char *key, const char *defaultValue = nullptr) const     = 0;
    virtual const rapidjson::Value &getArray(const char *key) const                              = 0;
    virtual const rapidjson::Value &getObject(const char *key) const                             = 0;
    virtual const rapidjson::Value &getValue(const char *key) const                              = 0;
    virtual int getInt(const char *key, int defaultValue = 0) const                              = 0;
    virtual int64_t getInt64(const char *key, int64_t defaultValue = 0) const                    = 0;
    virtual uint64_t getUint64(const char *key, uint64_t defaultValue = 0) const                 = 0;
    virtual unsigned getUint(const char *key, unsigned defaultValue = 0) const                   = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IJSONREADER_H
