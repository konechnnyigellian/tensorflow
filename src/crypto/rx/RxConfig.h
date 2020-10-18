

#ifndef TENSERFLOW_RXCONFIG_H
#define TENSERFLOW_RXCONFIG_H


#include "3rdparty/rapidjson/fwd.h"


#ifdef TENSERFLOW_FEATURE_MSR
#   include "crypto/rx/msr/MsrItem.h"
#endif


#include <vector>


namespace tenserflow {


class RxConfig
{
public:
    enum Mode : uint32_t {
        AutoMode,
        FastMode,
        LightMode,
        ModeMax
    };

    enum ScratchpadPrefetchMode : uint32_t {
        ScratchpadPrefetchOff,
        ScratchpadPrefetchT0,
        ScratchpadPrefetchNTA,
        ScratchpadPrefetchMov,
        ScratchpadPrefetchMax,
    };

    bool read(const rapidjson::Value &value);
    rapidjson::Value toJSON(rapidjson::Document &doc) const;

#   ifdef TENSERFLOW_FEATURE_HWLOC
    std::vector<uint32_t> nodeset() const;
#   else
    inline std::vector<uint32_t> nodeset() const { return std::vector<uint32_t>(); }
#   endif

    const char *modeName() const;
    uint32_t threads(uint32_t limit = 100) const;

    inline bool isOneGbPages() const    { return m_oneGbPages; }
    inline bool rdmsr() const           { return m_rdmsr; }
    inline bool wrmsr() const           { return m_wrmsr; }
    inline bool cacheQoS() const        { return m_cacheQoS; }
    inline Mode mode() const            { return m_mode; }

    inline ScratchpadPrefetchMode scratchpadPrefetchMode() const { return m_scratchpadPrefetchMode; }

#   ifdef TENSERFLOW_FEATURE_MSR
    const char *msrPresetName() const;
    const MsrItems &msrPreset() const;
#   endif

private:
#   ifdef TENSERFLOW_FEATURE_MSR
    uint32_t msrMod() const;
    void readMSR(const rapidjson::Value &value);

    bool m_wrmsr = true;
    MsrItems m_msrPreset;
#   else
    bool m_wrmsr = false;
#   endif

    bool m_cacheQoS = false;

    Mode readMode(const rapidjson::Value &value) const;

    bool m_numa         = true;
    bool m_oneGbPages   = false;
    bool m_rdmsr        = true;
    int m_threads       = -1;
    Mode m_mode         = AutoMode;

    ScratchpadPrefetchMode m_scratchpadPrefetchMode = ScratchpadPrefetchT0;

#   ifdef TENSERFLOW_FEATURE_HWLOC
    std::vector<uint32_t> m_nodeset;
#   endif

};


} /* namespace tenserflow */


#endif /* TENSERFLOW_RXCONFIG_H */
