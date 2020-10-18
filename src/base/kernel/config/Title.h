/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_TITLE_H
#define TENSERFLOW_TITLE_H


#include "3rdparty/rapidjson/fwd.h"
#include "base/tools/String.h"


namespace tenserflow {


class Title
{
public:
    Title() = default;
    Title(const rapidjson::Value &value);

    inline bool isEnabled() const   { return m_enabled; }

    rapidjson::Value toJSON() const;
    String value() const;

private:
    bool m_enabled  = true;
    String m_value;
};


} // namespace tenserflow


#endif /* TENSERFLOW_TITLE_H */
