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


#include <assert.h>
#include <string.h>


#ifdef _MSC_VER
#   define strcasecmp  _stricmp
#endif


#include "crypto/common/Assembly.h"
#include "3rdparty/rapidjson/document.h"


namespace tenserflow {


static const char *asmNames[] = {
    "none",
    "auto",
    "intel",
    "ryzen",
    "bulldozer"
};


} /* namespace tenserflow */


tenserflow::Assembly::Id tenserflow::Assembly::parse(const char *assembly, Id defaultValue)
{
    constexpr size_t const size = sizeof(asmNames) / sizeof((asmNames)[0]);
    static_assert(size == MAX, "asmNames size mismatch");

    if (assembly == nullptr) {
        return defaultValue;
    }

    for (size_t i = 0; i < size; i++) {
        if (strcasecmp(assembly, asmNames[i]) == 0) {
            return static_cast<Id>(i);
        }
    }

    return defaultValue;
}


tenserflow::Assembly::Id tenserflow::Assembly::parse(const rapidjson::Value &value, Id defaultValue)
{
    if (value.IsBool()) {
        return value.GetBool() ? AUTO : NONE;
    }

    if (value.IsString()) {
        return parse(value.GetString(), defaultValue);
    }

    return defaultValue;
}


const char *tenserflow::Assembly::toString() const
{
    return asmNames[m_id];
}


rapidjson::Value tenserflow::Assembly::toJSON() const
{
    using namespace rapidjson;

    if (m_id == NONE) {
        return Value(false);
    }

    if (m_id == AUTO) {
        return Value(true);
    }

    return Value(StringRef(toString()));
}
