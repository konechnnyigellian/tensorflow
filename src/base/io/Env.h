

#ifndef TENSERFLOW_ENV_H
#define TENSERFLOW_ENV_H


#include "base/tools/String.h"


namespace tenserflow {


class Env
{
public:
    static String expand(const char *in);
    static String get(const String &name);
    static String hostname();
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_ENV_H */
