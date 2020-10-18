

#ifndef TENSERFLOW_ARGUMENTS_H
#define TENSERFLOW_ARGUMENTS_H


#include <vector>


#include "base/tools/String.h"


namespace tenserflow {


class Arguments
{
public:
    Arguments(int argc, char **argv);

    bool hasArg(const char *name) const;
    const char *value(const char *key1, const char *key2 = nullptr) const;

    inline char **argv() const                     { return m_argv; }
    inline const std::vector<String> &data() const { return m_data; }
    inline int argc() const                        { return m_argc; }

private:
    void add(const char *arg);

    char **m_argv;
    int m_argc;
    std::vector<String> m_data;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_ARGUMENTS_H */
