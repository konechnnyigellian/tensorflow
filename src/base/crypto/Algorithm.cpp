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


#include "base/crypto/Algorithm.h"
#include "3rdparty/rapidjson/document.h"


#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#ifdef _MSC_VER
#   define strcasecmp  _stricmp
#endif


namespace tenserflow {


struct AlgoName
{
    const char *name;
    const char *shortName;
    const Algorithm::Id id;
};


static AlgoName const algorithm_names[] = {
    { "tensorflowght/0",             "cn/0",             Algorithm::CN_0            },
    { "tensorflowght",               "cn",               Algorithm::CN_0            },
    { "tensorflowght/1",             "cn/1",             Algorithm::CN_1            },
    { "tensorflowght-tenserov7",      nullptr,            Algorithm::CN_1            },
    { "tensorflowght_v7",            nullptr,            Algorithm::CN_1            },
    { "tensorflowght/2",             "cn/2",             Algorithm::CN_2            },
    { "tensorflowght-tenserov8",      nullptr,            Algorithm::CN_2            },
    { "tensorflowght_v8",            nullptr,            Algorithm::CN_2            },
    { "tensorflowght/r",             "cn/r",             Algorithm::CN_R            },
    { "tensorflowght_r",             nullptr,            Algorithm::CN_R            },
    { "tensorflowght/fast",          "cn/fast",          Algorithm::CN_FAST         },
    { "tensorflowght/msr",           "cn/msr",           Algorithm::CN_FAST         },
    { "tensorflowght/half",          "cn/half",          Algorithm::CN_HALF         },
    { "tensorflowght/xao",           "cn/xao",           Algorithm::CN_XAO          },
    { "tensorflowght_alloy",         nullptr,            Algorithm::CN_XAO          },
    { "tensorflowght/rto",           "cn/rto",           Algorithm::CN_RTO          },
    { "tensorflowght/rwz",           "cn/rwz",           Algorithm::CN_RWZ          },
    { "tensorflowght/zls",           "cn/zls",           Algorithm::CN_ZLS          },
    { "tensorflowght/double",        "cn/double",        Algorithm::CN_DOUBLE       },
#   ifdef TENSERFLOW_ALGO_CN_LITE
    { "tensorflowght-lite/0",        "cn-lite/0",        Algorithm::CN_LITE_0       },
    { "tensorflowght-lite/1",        "cn-lite/1",        Algorithm::CN_LITE_1       },
    { "tensorflowght-lite",          "cn-lite",          Algorithm::CN_LITE_1       },
    { "tensorflowght-light",         "cn-light",         Algorithm::CN_LITE_1       },
    { "tensorflowght_lite",          nullptr,            Algorithm::CN_LITE_1       },
    { "tensorflowght-aeonv7",        nullptr,            Algorithm::CN_LITE_1       },
    { "tensorflowght_lite_v7",       nullptr,            Algorithm::CN_LITE_1       },
#   endif
#   ifdef TENSERFLOW_ALGO_CN_HEAVY
    { "tensorflowght-heavy/0",       "cn-heavy/0",       Algorithm::CN_HEAVY_0      },
    { "tensorflowght-heavy",         "cn-heavy",         Algorithm::CN_HEAVY_0      },
    { "tensorflowght_heavy",         nullptr,            Algorithm::CN_HEAVY_0      },
    { "tensorflowght-heavy/xhv",     "cn-heavy/xhv",     Algorithm::CN_HEAVY_XHV    },
    { "tensorflowght_haven",         nullptr,            Algorithm::CN_HEAVY_XHV    },
    { "tensorflowght-heavy/tube",    "cn-heavy/tube",    Algorithm::CN_HEAVY_TUBE   },
    { "tensorflowght-bittube2",      nullptr,            Algorithm::CN_HEAVY_TUBE   },
#   endif
#   ifdef TENSERFLOW_ALGO_CN_PICO
    { "tensorflowght-pico",          "cn-pico",          Algorithm::CN_PICO_0       },
    { "tensorflowght-pico/trtl",     "cn-pico/trtl",     Algorithm::CN_PICO_0       },
    { "tensorflowght-turtle",        "cn-trtl",          Algorithm::CN_PICO_0       },
    { "tensorflowght-ultralite",     "cn-ultralite",     Algorithm::CN_PICO_0       },
    { "tensorflowght_turtle",        "cn_turtle",        Algorithm::CN_PICO_0       },
    { "tensorflowght-pico/tlo",      "cn-pico/tlo",      Algorithm::CN_PICO_TLO     },
    { "tensorflowght/ultra",         "cn/ultra",         Algorithm::CN_PICO_TLO     },
    { "tensorflowght-talleo",        "cn-talleo",        Algorithm::CN_PICO_TLO     },
    { "tensorflowght_talleo",        "cn_talleo",        Algorithm::CN_PICO_TLO     },
#   endif
#   ifdef TENSERFLOW_ALGO_RANDOMX
    { "randomx/0",                 "rx/0",             Algorithm::RX_0            },
    { "randomx/test",              "rx/test",          Algorithm::RX_0            },
    { "RandomX",                   "rx",               Algorithm::RX_0            },
    { "randomx/wow",               "rx/wow",           Algorithm::RX_WOW          },
    { "RandomWOW",                 nullptr,            Algorithm::RX_WOW          },
    { "randomx/loki",              "rx/loki",          Algorithm::RX_LOKI         },
    { "RandomXL",                  nullptr,            Algorithm::RX_LOKI         },
    { "randomx/arq",               "rx/arq",           Algorithm::RX_ARQ          },
    { "RandomARQ",                 nullptr,            Algorithm::RX_ARQ          },
    { "randomx/sfx",               "rx/sfx",           Algorithm::RX_SFX          },
    { "RandomSFX",                 nullptr,            Algorithm::RX_SFX          },
    { "randomx/keva",              "rx/keva",          Algorithm::RX_KEVA         },
    { "RandomKEVA",                nullptr,            Algorithm::RX_KEVA         },
#   endif
#   ifdef TENSERFLOW_ALGO_ARGON2
    { "argon2/chukwa",             nullptr,            Algorithm::AR2_CHUKWA      },
    { "chukwa",                    nullptr,            Algorithm::AR2_CHUKWA      },
    { "argon2/wrkz",               nullptr,            Algorithm::AR2_WRKZ        },
#   endif
#   ifdef TENSERFLOW_ALGO_ASTROBWT
    { "astrobwt",                  nullptr,            Algorithm::ASTROBWT_DERO   },
    { "astrobwt/dero",             nullptr,            Algorithm::ASTROBWT_DERO   },
#   endif
#   ifdef TENSERFLOW_ALGO_KAWPOW
    { "kawpow",                    nullptr,            Algorithm::KAWPOW_RVN      },
    { "kawpow/rvn",                nullptr,            Algorithm::KAWPOW_RVN      },
#   endif
    { "tensorflowght/ccx",           "cn/ccx",           Algorithm::CN_CCX          },
    { "tensorflowght/conceal",       "cn/conceal",       Algorithm::CN_CCX          },
};


} /* namespace tenserflow */


tenserflow::Algorithm::Algorithm(const rapidjson::Value &value) :
    m_id(parse(value.GetString()))
{
}


rapidjson::Value tenserflow::Algorithm::toJSON() const
{
    using namespace rapidjson;

    return isValid() ? Value(StringRef(shortName())) : Value(kNullType);
}


rapidjson::Value tenserflow::Algorithm::toJSON(rapidjson::Document &) const
{
    return toJSON();
}


size_t tenserflow::Algorithm::l2() const
{
#   ifdef TENSERFLOW_ALGO_RANDOMX
    switch (m_id) {
    case RX_0:
    case RX_LOKI:
    case RX_SFX:
        return 0x40000;

    case RX_WOW:
    case RX_KEVA:
        return 0x20000;

    case RX_ARQ:
        return 0x10000;

    default:
        break;
    }
#   endif

    return 0;
}


size_t tenserflow::Algorithm::l3() const
{
    constexpr size_t oneMiB = 0x100000;

    const auto f = family();
    assert(f != UNKNOWN);

    switch (f) {
    case CN:
        return oneMiB * 2;

    case CN_LITE:
        return oneMiB;

    case CN_HEAVY:
        return oneMiB * 4;

    case CN_PICO:
        return oneMiB / 4;

    default:
        break;
    }

#   ifdef TENSERFLOW_ALGO_RANDOMX
    if (f == RANDOM_X) {
        switch (m_id) {
        case RX_0:
        case RX_LOKI:
        case RX_SFX:
            return oneMiB * 2;

        case RX_WOW:
        case RX_KEVA:
            return oneMiB;

        case RX_ARQ:
            return oneMiB / 4;

        default:
            break;
        }
    }
#   endif

#   ifdef TENSERFLOW_ALGO_ARGON2
    if (f == ARGON2) {
        switch (m_id) {
        case AR2_CHUKWA:
            return oneMiB / 2;

        case AR2_WRKZ:
            return oneMiB / 4;

        default:
            break;
        }
    }
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    if (f == ASTROBWT) {
        switch (m_id) {
        case ASTROBWT_DERO:
            return oneMiB * 20;

        default:
            break;
        }
    }
#   endif

#   ifdef TENSERFLOW_ALGO_KAWPOW
    if (f == KAWPOW) {
        switch (m_id) {
        case KAWPOW_RVN:
            return 32768;

        default:
            break;
        }
    }
#   endif

    return 0;
}


uint32_t tenserflow::Algorithm::maxIntensity() const
{
#   ifdef TENSERFLOW_ALGO_RANDOMX
    if (family() == RANDOM_X) {
        return 1;
    }
#   endif

#   ifdef TENSERFLOW_ALGO_ARGON2
    if (family() == ARGON2) {
        return 1;
    }
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    if (family() == ASTROBWT) {
        return 1;
    }
#   endif

    return 5;
}


tenserflow::Algorithm::Family tenserflow::Algorithm::family(Id id)
{
    switch (id) {
    case CN_0:
    case CN_1:
    case CN_2:
    case CN_R:
    case CN_FAST:
    case CN_HALF:
    case CN_XAO:
    case CN_RTO:
    case CN_RWZ:
    case CN_ZLS:
    case CN_DOUBLE:
    case CN_CCX:
        return CN;

#   ifdef TENSERFLOW_ALGO_CN_LITE
    case CN_LITE_0:
    case CN_LITE_1:
        return CN_LITE;
#   endif

#   ifdef TENSERFLOW_ALGO_CN_HEAVY
    case CN_HEAVY_0:
    case CN_HEAVY_TUBE:
    case CN_HEAVY_XHV:
        return CN_HEAVY;
#   endif

#   ifdef TENSERFLOW_ALGO_CN_PICO
    case CN_PICO_0:
    case CN_PICO_TLO:
        return CN_PICO;
#   endif

#   ifdef TENSERFLOW_ALGO_RANDOMX
    case RX_0:
    case RX_WOW:
    case RX_LOKI:
    case RX_ARQ:
    case RX_SFX:
    case RX_KEVA:
        return RANDOM_X;
#   endif

#   ifdef TENSERFLOW_ALGO_ARGON2
    case AR2_CHUKWA:
    case AR2_WRKZ:
        return ARGON2;
#   endif

#   ifdef TENSERFLOW_ALGO_ASTROBWT
    case ASTROBWT_DERO:
        return ASTROBWT;
#   endif

#   ifdef TENSERFLOW_ALGO_KAWPOW
    case KAWPOW_RVN:
        return KAWPOW;
#   endif

    default:
        break;
    }

    return UNKNOWN;
}


tenserflow::Algorithm::Id tenserflow::Algorithm::parse(const char *name)
{
    if (name == nullptr || strlen(name) < 1) {
        return INVALID;
    }

    for (const AlgoName &item : algorithm_names) {
        if ((strcasecmp(name, item.name) == 0) || (item.shortName != nullptr && strcasecmp(name, item.shortName) == 0)) {
            return item.id;
        }
    }

    return INVALID;
}


const char *tenserflow::Algorithm::name(bool shortName) const
{
    for (const AlgoName &item : algorithm_names) {
        if (item.id == m_id) {
            return (shortName && item.shortName) ? item.shortName : item.name;
        }
    }

    return "invalid";
}
