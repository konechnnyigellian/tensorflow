/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_LINEREADER_H
#define TENSERFLOW_LINEREADER_H


#include "base/tools/Object.h"


#include <cstddef>


namespace tenserflow {


class ILineListener;


class LineReader
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(LineReader)

    LineReader() = default;
    LineReader(ILineListener *listener) : m_listener(listener) {}
    ~LineReader();

    inline void setListener(ILineListener *listener) { m_listener = listener; }

    void parse(char *data, size_t size);
    void reset();

private:
    void add(const char *data, size_t size);
    void getline(char *data, size_t size);

    char *m_buf                 = nullptr;
    ILineListener *m_listener   = nullptr;
    size_t m_pos                = 0;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_NETBUFFER_H */
