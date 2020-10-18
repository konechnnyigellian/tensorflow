


#include "base/io/Env.h"
#include "base/kernel/Process.h"
#include "version.h"


#include <regex>
#include <uv.h>
#include <map>


#ifndef _WIN32
#   include <unistd.h>
#endif


#ifndef UV_MAXHOSTNAMESIZE
#   ifdef MAXHOSTNAMELEN
#       define UV_MAXHOSTNAMESIZE (MAXHOSTNAMELEN + 1)
#   else
#       define UV_MAXHOSTNAMESIZE 256
#   endif
#endif


namespace tenserflow {


#ifdef TENSERFLOW_FEATURE_ENV
static std::map<String, String> variables;


static void createVariables()
{
    variables.insert({ "TENSERFLOW_VERSION",  APP_VERSION });
    variables.insert({ "TENSERFLOW_KIND",     APP_KIND });
    variables.insert({ "TENSERFLOW_HOSTNAME", Env::hostname() });
    variables.insert({ "TENSERFLOW_EXE",      Process::exepath() });
    variables.insert({ "TENSERFLOW_EXE_DIR",  Process::location(Process::ExeLocation) });
    variables.insert({ "TENSERFLOW_CWD",      Process::location(Process::CwdLocation) });
    variables.insert({ "TENSERFLOW_HOME_DIR", Process::location(Process::HomeLocation) });
    variables.insert({ "TENSERFLOW_TEMP_DIR", Process::location(Process::TempLocation) });
    variables.insert({ "TENSERFLOW_DATA_DIR", Process::location(Process::DataLocation) });
}
#endif


} // namespace tenserflow


tenserflow::String tenserflow::Env::expand(const char *in)
{
#   ifdef TENSERFLOW_FEATURE_ENV
    if (in == nullptr) {
        return {};
    }

    std::string text(in);
    if (text.size() < 4) {
        return text.c_str();
    }

    static const std::regex env_re{R"--(\$\{([^}]+)\})--"};

    std::map<std::string, String> vars;

    for (std::sregex_iterator i = std::sregex_iterator(text.begin(), text.end(), env_re); i != std::sregex_iterator(); ++i) {
        std::smatch m = *i;
        const auto var = m.str();

        if (vars.count(var)) {
            continue;
        }

        vars.insert({ var, get(m[1].str().c_str()) });
    }

    for (const auto &kv : vars) {
        if (kv.second.isNull()) {
            continue;
        }

        size_t pos = 0;
        while ((pos = text.find(kv.first, pos)) != std::string::npos) {
            text.replace(pos, kv.first.size(), kv.second);
            pos += kv.second.size();
        }
    }

    return text.c_str();
#   else
    return in;
#   endif
}


tenserflow::String tenserflow::Env::get(const String &name)
{
#   ifdef TENSERFLOW_FEATURE_ENV
    if (variables.empty()) {
        createVariables();
    }

    if (variables.count(name)) {
        return variables.at(name);
    }
#   endif

    return static_cast<const char *>(getenv(name));
}


tenserflow::String tenserflow::Env::hostname()
{
    char buf[UV_MAXHOSTNAMESIZE]{};
    size_t size = sizeof(buf);

#   if UV_VERSION_HEX >= 0x010c00
    if (uv_os_gethostname(buf, &size) == 0) {
        return static_cast<const char *>(buf);
    }
#   else
    if (gethostname(buf, size) == 0) {
        return static_cast<const char *>(buf);
    }
#   endif

    return {};
}
