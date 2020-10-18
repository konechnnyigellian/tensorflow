

#ifndef TENSERFLOW_BUFFER_H
#define TENSERFLOW_BUFFER_H


#include "base/tools/String.h"


namespace tenserflow {


class Buffer
{
public:
    Buffer() = default;
    Buffer(Buffer &&other) noexcept;
    Buffer(const Buffer &other);
    Buffer(const char *data, size_t size);
    Buffer(size_t size);
    ~Buffer();


    inline bool isEmpty() const                     { return size() == 0; }
    inline bool isEqual(const Buffer &other) const  { return m_size == other.m_size && (m_size == 0 || memcmp(m_data, other.m_data, m_size) == 0); }
    inline char *data()                             { return m_data; }
    inline const char *data() const                 { return m_data; }
    inline size_t size() const                      { return m_size; }
    inline void from(const Buffer &other)           { from(other.data(), other.size()); }


    void from(const char *data, size_t size);


    inline bool operator!=(const Buffer &other) const   { return !isEqual(other); }
    inline bool operator==(const Buffer &other) const   { return isEqual(other); }
    inline Buffer &operator=(Buffer &&other) noexcept   { move(std::move(other)); return *this; }
    inline Buffer &operator=(const Buffer &other)       { from(other); return *this; }


    static Buffer allocUnsafe(size_t size);
    static Buffer randomBytes(const size_t size);

    static inline Buffer alloc(size_t size) { return Buffer(size); }


    inline static bool fromHex(const char *in, size_t size, char *out)    { return fromHex(reinterpret_cast<const uint8_t *>(in), size, reinterpret_cast<uint8_t *>(out)); }
    inline static bool fromHex(const char *in, size_t size, uint8_t *out) { return fromHex(reinterpret_cast<const uint8_t *>(in), size, out); }
    inline static Buffer fromHex(const char *data)                        { return fromHex(data, strlen(data)); }
    inline static Buffer fromHex(const String &str)                       { return fromHex(str.data(), str.size()); }
    inline static String toHex(const char *in, size_t size)               { return Buffer(in, size).toHex(); }
    inline static String toHex(const uint8_t *in, size_t size)            { return Buffer(reinterpret_cast<const char *>(in), size).toHex(); }
    inline static void toHex(const char *in, size_t size, char *out)      { return toHex(reinterpret_cast<const uint8_t *>(in), size, reinterpret_cast<uint8_t *>(out)); }
    inline static void toHex(const uint8_t *in, size_t size, char *out)   { return toHex(in, size, reinterpret_cast<uint8_t *>(out)); }

    static bool fromHex(const uint8_t *in, size_t size, uint8_t *out);
    static Buffer fromHex(const char *data, size_t size);
    static void toHex(const uint8_t *in, size_t size, uint8_t *out);
    String toHex() const;

private:
    void copy(const char *data, size_t size);
    void move(Buffer &&other);

    char *m_data    = nullptr;
    size_t m_size   = 0;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_BUFFER_H */
