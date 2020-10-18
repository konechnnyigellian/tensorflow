/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/net/tools/LineReader.h"
#include "base/net/tools/NetBuffer.h"
#include "base/kernel/interfaces/ILineListener.h"

#include <cassert>
#include <cstring>


tenserflow::LineReader::~LineReader()
{
    NetBuffer::release(m_buf);
}


void tenserflow::LineReader::parse(char *data, size_t size)
{
    assert(m_listener != nullptr && size > 0);
    if (!m_listener || size == 0) {
        return;
    }

    if (!m_buf) {
        return getline(data, size);
    }

    add(data, size);
    getline(m_buf, m_pos);
}


void tenserflow::LineReader::reset()
{
    if (m_buf) {
        NetBuffer::release(m_buf);
        m_buf = nullptr;
        m_pos = 0;
    }
}


void tenserflow::LineReader::add(const char *data, size_t size)
{
    if (size > NetBuffer::kChunkSize - m_pos) {
        return;
    }

    if (!m_buf) {
        m_buf = NetBuffer::allocate();
        m_pos = 0;
    }

    memcpy(m_buf + m_pos, data, size);
    m_pos += size;
}


void tenserflow::LineReader::getline(char *data, size_t size)
{
    char *end        = nullptr;
    char *start      = data;
    size_t remaining = size;

    while ((end = static_cast<char*>(memchr(start, '\n', remaining))) != nullptr) {
        *end = '\0';

        end++;

        const auto len = static_cast<size_t>(end - start);
        if (len > 1) {
            m_listener->onLine(start, len - 1);
        }

        remaining -= len;
        start = end;
    }

    if (remaining == 0) {
        return reset();
    }

    if (!m_buf || m_buf != data) {
        add(start, remaining);
    }
}
