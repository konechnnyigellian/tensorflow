


#include "crypto/rx/RxConfig.h"
#include "3rdparty/rapidjson/document.h"
#include "backend/cpu/Cpu.h"
#include "base/io/json/Json.h"


#ifdef TENSERFLOW_FEATURE_HWLOC
#   include "backend/cpu/platform/HwlocCpuInfo.h"
#endif


#include <array>
#include <algorithm>
#include <cmath>


#ifdef _MSC_VER
#   define strcasecmp  _stricmp
#endif


namespace tenserflow {

static const char *kInit        = "init";
static const char *kMode        = "mode";
static const char *kOneGbPages  = "1gb-pages";
static const char *kRdmsr       = "rdmsr";
static const char *kWrmsr       = "wrmsr";
static const char *kCacheQoS    = "cache_qos";

#ifdef TENSERFLOW_FEATURE_HWLOC
static const char *kNUMA        = "numa";
#endif

static const char *kScratchpadPrefetchMode = "scratchpad_prefetch_mode";

static const std::array<const char *, RxConfig::ModeMax> modeNames = { "auto", "fast", "light" };


#ifdef TENSERFLOW_FEATURE_MSR
constexpr size_t kMsrArraySize = 4;

static const std::array<MsrItems, kMsrArraySize> msrPresets = {
    MsrItems(),
    MsrItems{{ 0xC0011020, 0x0 }, { 0xC0011021, 0x40, ~0x20ULL }, { 0xC0011022, 0x510000 }, { 0xC001102b, 0x1808cc16 }},
    MsrItems{{ 0x1a4, 0xf }},
    MsrItems()
};

static const std::array<const char *, kMsrArraySize> modNames = { "none", "ryzen", "intel", "custom" };

static_assert (kMsrArraySize == ICpuInfo::MSR_MOD_MAX, "kMsrArraySize and MSR_MOD_MAX mismatch");
#endif


}


bool tenserflow::RxConfig::read(const rapidjson::Value &value)
{
    if (value.IsObject()) {
        m_threads    = Json::getInt(value, kInit, m_threads);
        m_mode       = readMode(Json::getValue(value, kMode));
        m_rdmsr      = Json::getBool(value, kRdmsr, m_rdmsr);

#       ifdef TENSERFLOW_FEATURE_MSR
        readMSR(Json::getValue(value, kWrmsr));
#       endif

        m_cacheQoS = Json::getBool(value, kCacheQoS, m_cacheQoS);

#       ifdef TENSERFLOW_OS_LINUX
        m_oneGbPages = Json::getBool(value, kOneGbPages, m_oneGbPages);
#       endif

#       ifdef TENSERFLOW_FEATURE_HWLOC
        if (m_mode == LightMode) {
            m_numa = false;

            return true;
        }

        const auto &numa = Json::getValue(value, kNUMA);
        if (numa.IsArray()) {
            m_nodeset.reserve(numa.Size());

            for (const auto &node : numa.GetArray()) {
                if (node.IsUint()) {
                    m_nodeset.emplace_back(node.GetUint());
                }
            }
        }
        else if (numa.IsBool()) {
            m_numa = numa.GetBool();
        }
#       endif

        const uint32_t mode = static_cast<uint32_t>(Json::getInt(value, kScratchpadPrefetchMode, static_cast<int>(m_scratchpadPrefetchMode)));
        if (mode < ScratchpadPrefetchMax) {
            m_scratchpadPrefetchMode = static_cast<ScratchpadPrefetchMode>(mode);
        }

        return true;
    }

    return false;
}


rapidjson::Value tenserflow::RxConfig::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;
    auto &allocator = doc.GetAllocator();

    Value obj(kObjectType);
    obj.AddMember(StringRef(kInit),         m_threads, allocator);
    obj.AddMember(StringRef(kMode),         StringRef(modeName()), allocator);
    obj.AddMember(StringRef(kOneGbPages),   m_oneGbPages, allocator);
    obj.AddMember(StringRef(kRdmsr),        m_rdmsr, allocator);

#   ifdef TENSERFLOW_FEATURE_MSR
    if (!m_msrPreset.empty()) {
        Value wrmsr(kArrayType);
        wrmsr.Reserve(m_msrPreset.size(), allocator);

        for (const auto &i : m_msrPreset) {
            wrmsr.PushBack(i.toJSON(doc), allocator);
        }

        obj.AddMember(StringRef(kWrmsr), wrmsr, allocator);
    }
    else {
        obj.AddMember(StringRef(kWrmsr), m_wrmsr, allocator);
    }
#   else
    obj.AddMember(StringRef(kWrmsr), false, allocator);
#   endif

    obj.AddMember(StringRef(kCacheQoS), m_cacheQoS, allocator);

#   ifdef TENSERFLOW_FEATURE_HWLOC
    if (!m_nodeset.empty()) {
        Value numa(kArrayType);

        for (uint32_t i : m_nodeset) {
            numa.PushBack(i, allocator);
        }

        obj.AddMember(StringRef(kNUMA), numa, allocator);
    }
    else {
        obj.AddMember(StringRef(kNUMA), m_numa, allocator);
    }
#   endif

    obj.AddMember(StringRef(kScratchpadPrefetchMode), static_cast<int>(m_scratchpadPrefetchMode), allocator);

    return obj;
}


#ifdef TENSERFLOW_FEATURE_HWLOC
std::vector<uint32_t> tenserflow::RxConfig::nodeset() const
{
    if (!m_nodeset.empty()) {
        return m_nodeset;
    }

    return (m_numa && Cpu::info()->nodes() > 1) ? static_cast<HwlocCpuInfo *>(Cpu::info())->nodeset() : std::vector<uint32_t>();
}
#endif


const char *tenserflow::RxConfig::modeName() const
{
    return modeNames[m_mode];
}


uint32_t tenserflow::RxConfig::threads(uint32_t limit) const
{
    if (m_threads > 0) {
        return m_threads;
    }

    if (limit < 100) {
        return std::max(static_cast<uint32_t>(round(Cpu::info()->threads() * (limit / 100.0))), 1U);
    }

    return Cpu::info()->threads();
}


#ifdef TENSERFLOW_FEATURE_MSR
const char *tenserflow::RxConfig::msrPresetName() const
{
    return modNames[msrMod()];
}


const tenserflow::MsrItems &tenserflow::RxConfig::msrPreset() const
{
    const auto mod = msrMod();

    if (mod == ICpuInfo::MSR_MOD_CUSTOM) {
        return m_msrPreset;
    }

    return msrPresets[mod];
}


uint32_t tenserflow::RxConfig::msrMod() const
{
    if (!wrmsr()) {
        return ICpuInfo::MSR_MOD_NONE;
    }

    if (!m_msrPreset.empty()) {
        return ICpuInfo::MSR_MOD_CUSTOM;
    }

    return Cpu::info()->msrMod();
}


void tenserflow::RxConfig::readMSR(const rapidjson::Value &value)
{
    if (value.IsBool()) {
        m_wrmsr = value.GetBool();

        return;
    }

    if (value.IsInt() && Cpu::info()->vendor() == ICpuInfo::VENDOR_INTEL) {
        const int i = std::min(value.GetInt(), 15);
        if (i >= 0) {
            m_msrPreset.emplace_back(0x1a4, i);
        }
        else {
            m_wrmsr = false;
        }
    }

    if (value.IsArray()) {
        for (const auto &i : value.GetArray()) {
            MsrItem item(i);
            if (item.isValid()) {
                m_msrPreset.emplace_back(item);
            }
        }

        m_wrmsr = !m_msrPreset.empty();
    }
}
#endif


tenserflow::RxConfig::Mode tenserflow::RxConfig::readMode(const rapidjson::Value &value) const
{
    if (value.IsUint()) {
        return static_cast<Mode>(std::min(value.GetUint(), ModeMax - 1));
    }

    if (value.IsString()) {
        auto mode = value.GetString();

        for (size_t i = 0; i < modeNames.size(); i++) {
            if (strcasecmp(mode, modeNames[i]) == 0) {
                return static_cast<Mode>(i);
            }
        }
    }

    return AutoMode;
}
