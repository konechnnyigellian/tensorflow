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
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_MSRITEM_H
#define TENSERFLOW_MSRITEM_H


#include "base/tools/String.h"


#include <limits>
#include <vector>


namespace tenserflow
{


class RxDataset;


class MsrItem
{
public:
    constexpr static uint64_t kNoMask = std::numeric_limits<uint64_t>::max();

    inline MsrItem() = default;
    inline MsrItem(uint32_t reg, uint64_t value, uint64_t mask = kNoMask) : m_reg(reg), m_value(value), m_mask(mask) {}

    MsrItem(const rapidjson::Value &value);

    inline bool isValid() const     { return m_reg > 0; }
    inline uint32_t reg() const     { return m_reg; }
    inline uint64_t value() const   { return m_value; }
    inline uint64_t mask() const    { return m_mask; }

    rapidjson::Value toJSON(rapidjson::Document &doc) const;
    String toString() const;

private:
    uint32_t m_reg      = 0;
    uint64_t m_value    = 0;
    uint64_t m_mask     = kNoMask;
};


using MsrItems = std::vector<MsrItem>;


} /* namespace tenserflow */


#endif /* TENSERFLOW_MSRITEM_H */
