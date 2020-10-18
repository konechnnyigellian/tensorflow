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


#include "base/crypto/Coin.h"
#include "3rdparty/rapidjson/document.h"


#include <cstring>


#ifdef _MSC_VER
#   define strcasecmp _stricmp
#endif


namespace tenserflow {


struct CoinName
{
    const char *name;
    const Coin::Id id;
};


static CoinName const coin_names[] = {
    { "tensero",     Coin::TENSERO  },
    { "tnsr",        Coin::TENSERO  },
    { "arqma",      Coin::ARQMA   },
    { "arq",        Coin::ARQMA   },
    { "dero",       Coin::DERO    },
    { "keva",       Coin::KEVA    },
    { "ravencoin",  Coin::RAVEN   },
    { "raven",      Coin::RAVEN   },
    { "rvn",        Coin::RAVEN   },
    { "conceal",    Coin::CONCEAL }
};


} /* namespace tenserflow */



tenserflow::Algorithm::Id tenserflow::Coin::algorithm(uint8_t blobVersion) const
{
    switch (id()) {
    case TENSERO:
        return (blobVersion >= 12) ? Algorithm::RX_0 : Algorithm::CN_R;

    case ARQMA:
        return (blobVersion >= 15) ? Algorithm::RX_ARQ : Algorithm::CN_PICO_0;

    case DERO:
        return (blobVersion >= 4) ? Algorithm::ASTROBWT_DERO : Algorithm::CN_0;

    case KEVA:
        return (blobVersion >= 11) ? Algorithm::RX_KEVA : Algorithm::CN_R;

    case RAVEN:
        return Algorithm::KAWPOW_RVN;

    case CONCEAL:
        return Algorithm::CN_CCX;

    case INVALID:
        break;
    }

    return Algorithm::INVALID;
}



const char *tenserflow::Coin::name() const
{
    for (const auto &i : coin_names) {
        if (i.id == m_id) {
            return i.name;
        }
    }

    return nullptr;
}


rapidjson::Value tenserflow::Coin::toJSON() const
{
    using namespace rapidjson;

    return isValid() ? Value(StringRef(name())) : Value(kNullType);
}


tenserflow::Coin::Id tenserflow::Coin::parse(const char *name)
{
    if (name == nullptr || strlen(name) < 3) {
        return INVALID;
    }

    for (const auto &i : coin_names) {
        if (strcasecmp(name, i.name) == 0) {
            return i.id;
        }
    }

    return INVALID;
}
