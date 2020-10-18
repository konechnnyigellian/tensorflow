

#ifndef TENSERFLOW_OCLCONFIG_H
#define TENSERFLOW_OCLCONFIG_H


#include "backend/common/Threads.h"
#include "backend/opencl/OclLaunchData.h"
#include "backend/opencl/OclThreads.h"
#include "backend/opencl/wrappers/OclPlatform.h"


namespace tenserflow {


class OclConfig
{
public:
    OclConfig();

    OclPlatform platform() const;
    rapidjson::Value toJSON(rapidjson::Document &doc) const;
    std::vector<OclLaunchData> get(const Tenserflower *tenserflower, const Algorithm &algorithm, const OclPlatform &platform, const std::vector<OclDevice> &devices) const;
    void read(const rapidjson::Value &value);

    inline bool isCacheEnabled() const                  { return m_cache; }
    inline bool isEnabled() const                       { return m_enabled; }
    inline bool isShouldSave() const                    { return m_shouldSave; }
    inline const String &loader() const                 { return m_loader; }
    inline const Threads<OclThreads> &threads() const   { return m_threads; }

#   ifdef TENSERFLOW_FEATURE_ADL
    inline bool isAdlEnabled() const                    { return m_adl; }
#   endif

private:
    void generate();
    void setDevicesHint(const char *devicesHint);

    bool m_cache         = true;
    bool m_enabled       = false;
    bool m_shouldSave    = false;
    std::vector<uint32_t> m_devicesHint;
    String m_loader;
    Threads<OclThreads> m_threads;

#   ifndef TENSERFLOW_OS_APPLE
    void setPlatform(const rapidjson::Value &platform);

    String m_platformVendor;
    uint32_t m_platformIndex = 0;
#   endif

#   ifdef TENSERFLOW_FEATURE_ADL
    bool m_adl          = true;
#   endif
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_OCLCONFIG_H */
