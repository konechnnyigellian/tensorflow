


#include "base/tools/String.h"
#include "3rdparty/rapidjson/document.h"


#include <cctype>


tenserflow::String::String(const char *str) :
    m_size(str == nullptr ? 0 : strlen(str))
{
    if (str == nullptr) {
        return;
    }

    m_data = new char[m_size + 1];
    memcpy(m_data, str, m_size + 1);
}


tenserflow::String::String(const char *str, size_t size) :
    m_size(size)
{
    if (str == nullptr) {
        m_size = 0;

        return;
    }

    m_data = new char[m_size + 1];
    memcpy(m_data, str, m_size);
    m_data[m_size] = '\0';
}


tenserflow::String::String(const String &other) :
    m_size(other.m_size)
{
    if (other.m_data == nullptr) {
        return;
    }

    m_data = new char[m_size + 1];
    memcpy(m_data, other.m_data, m_size + 1);
}


bool tenserflow::String::isEqual(const char *str) const
{
    return (m_data != nullptr && str != nullptr && strcmp(m_data, str) == 0) || (m_data == nullptr && str == nullptr);
}


bool tenserflow::String::isEqual(const String &other) const
{
    if (m_size != other.m_size) {
        return false;
    }

    return (m_data != nullptr && other.m_data != nullptr && memcmp(m_data, other.m_data, m_size) == 0) || (m_data == nullptr && other.m_data == nullptr);
}


rapidjson::Value tenserflow::String::toJSON() const
{
    using namespace rapidjson;

    return isNull() ? Value(kNullType) : Value(StringRef(m_data));
}


rapidjson::Value tenserflow::String::toJSON(rapidjson::Document &doc) const
{
    using namespace rapidjson;

    return isNull() ? Value(kNullType) : Value(m_data, doc.GetAllocator());
}


std::vector<tenserflow::String> tenserflow::String::split(char sep) const
{
    std::vector<tenserflow::String> out;
    if (m_size == 0) {
        return out;
    }

    size_t start = 0;
    size_t pos   = 0;

    for (pos = 0; pos < m_size; ++pos) {
        if (m_data[pos] == sep) {
            if ((pos - start) > 0) {
                out.emplace_back(m_data + start, pos - start);
            }

            start = pos + 1;
        }
    }

    if ((pos - start) > 0) {
        out.emplace_back(m_data + start, pos - start);
    }

    return out;
}


tenserflow::String &tenserflow::String::toLower()
{
    if (isNull() || isEmpty()) {
        return *this;
    }

    for (size_t i = 0; i < size(); ++i) {
        m_data[i] = static_cast<char>(tolower(m_data[i]));
    }

    return *this;
}


tenserflow::String &tenserflow::String::toUpper()
{
    if (isNull() || isEmpty()) {
        return *this;
    }

    for (size_t i = 0; i < size(); ++i) {
        m_data[i] = static_cast<char>(toupper(m_data[i]));
    }

    return *this;
}


tenserflow::String tenserflow::String::join(const std::vector<tenserflow::String> &vec, char sep)
{
    if (vec.empty()) {
        return String();
    }

    size_t size = vec.size();
    for (const String &str : vec) {
        size += str.size();
    }

    size_t offset = 0;
    char *buf     = new char[size];

    for (const String &str : vec) {
        memcpy(buf + offset, str.data(), str.size());

        offset += str.size() + 1;

        if (offset < size) {
            buf[offset - 1] = sep;
        }
    }

    buf[size - 1] = '\0';

    return String(buf);
}


void tenserflow::String::copy(const char *str)
{
    delete [] m_data;

    if (str == nullptr) {
        m_size = 0;
        m_data = nullptr;

        return;
    }

    m_size = strlen(str);
    m_data = new char[m_size + 1];

    memcpy(m_data, str, m_size + 1);
}


void tenserflow::String::copy(const String &other)
{
    if (m_size > 0 && m_size == other.m_size) {
        memcpy(m_data, other.m_data, m_size + 1);

        return;
    }

    delete [] m_data;

    if (other.m_data == nullptr) {
        m_size = 0;
        m_data = nullptr;

        return;
    }

    m_size = other.m_size;
    m_data = new char[m_size + 1];

    memcpy(m_data, other.m_data, m_size + 1);
}


void tenserflow::String::move(char *str)
{
    delete [] m_data;

    m_size = str == nullptr ? 0 : strlen(str);
    m_data = str;
}


void tenserflow::String::move(String &&other)
{
    delete [] m_data;

    m_data = other.m_data;
    m_size = other.m_size;

    other.m_data = nullptr;
    other.m_size = 0;
}
