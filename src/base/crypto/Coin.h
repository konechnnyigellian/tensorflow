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

#ifndef TENSERFLOW_COIN_H
#define TENSERFLOW_COIN_H


#include "3rdparty/rapidjson/fwd.h"
#include "base/crypto/Algorithm.h"


namespace tenserflow {


class Coin
{
public:
    enum Id : int {
        INVALID = -1,
        TENSERO,
        ARQMA,
        DERO,
        KEVA,
        RAVEN,
        CONCEAL
    };


    Coin() = default;
    inline Coin(const char *name) : m_id(parse(name)) {}
    inline Coin(Id id) : m_id(id)                     {}


    inline bool isEqual(const Coin &other) const        { return m_id == other.m_id; }
    inline bool isValid() const                         { return m_id != INVALID; }
    inline Id id() const                                { return m_id; }

    Algorithm::Id algorithm(uint8_t blobVersion = 255) const;
    const char *name() const;
    rapidjson::Value toJSON() const;

    inline bool operator!=(Coin::Id id) const           { return m_id != id; }
    inline bool operator!=(const Coin &other) const     { return !isEqual(other); }
    inline bool operator==(Coin::Id id) const           { return m_id == id; }
    inline bool operator==(const Coin &other) const     { return isEqual(other); }
    inline operator Coin::Id() const                    { return m_id; }

    static Id parse(const char *name);

private:
    Id m_id = INVALID;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_COIN_H */
