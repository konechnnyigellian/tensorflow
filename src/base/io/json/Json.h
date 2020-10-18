

#ifndef TENSERFLOW_JSON_H
#define TENSERFLOW_JSON_H


#include "3rdparty/rapidjson/fwd.h"
#include "base/kernel/interfaces/IJsonReader.h"


#include <string>
#include <vector>
#include <fstream>


namespace tenserflow {


class Json
{
public:
    static bool getBool(const rapidjson::Value &obj, const char *key, bool defaultValue = false);
    static bool isEmpty(const rapidjson::Value &obj);
    static const char *getString(const rapidjson::Value &obj, const char *key, const char *defaultValue = nullptr);
    static const rapidjson::Value &getArray(const rapidjson::Value &obj, const char *key);
    static const rapidjson::Value &getObject(const rapidjson::Value &obj, const char *key);
    static const rapidjson::Value &getValue(const rapidjson::Value &obj, const char *key);
    static int getInt(const rapidjson::Value &obj, const char *key, int defaultValue = 0);
    static int64_t getInt64(const rapidjson::Value &obj, const char *key, int64_t defaultValue = 0);
    static uint64_t getUint64(const rapidjson::Value &obj, const char *key, uint64_t defaultValue = 0);
    static unsigned getUint(const rapidjson::Value &obj, const char *key, unsigned defaultValue = 0);

    static bool get(const char *fileName, rapidjson::Document &doc);
    static bool save(const char *fileName, const rapidjson::Document &doc);

    static bool convertOffset(const char *fileName, size_t offset, size_t &line, size_t &pos, std::vector<std::string> &s);
    static rapidjson::Value normalize(double value, bool zero);

private:
    static bool convertOffset(std::istream &ifs, size_t offset, size_t &line, size_t &pos, std::vector<std::string> &s);
};


class JsonReader : public IJsonReader
{
public:
    inline JsonReader(const rapidjson::Value &obj) : m_obj(obj) {}

    inline bool getBool(const char *key, bool defaultValue = false) const override                   { return Json::getBool(m_obj, key, defaultValue); }
    inline const char *getString(const char *key, const char *defaultValue = nullptr) const override { return Json::getString(m_obj, key, defaultValue); }
    inline const rapidjson::Value &getArray(const char *key) const override                          { return Json::getArray(m_obj, key); }
    inline const rapidjson::Value &getObject(const char *key) const override                         { return Json::getObject(m_obj, key); }
    inline const rapidjson::Value &getValue(const char *key) const override                          { return Json::getValue(m_obj, key); }
    inline int getInt(const char *key, int defaultValue = 0) const override                          { return Json::getInt(m_obj, key, defaultValue); }
    inline int64_t getInt64(const char *key, int64_t defaultValue = 0) const override                { return Json::getInt64(m_obj, key, defaultValue); }
    inline uint64_t getUint64(const char *key, uint64_t defaultValue = 0) const override             { return Json::getUint64(m_obj, key, defaultValue); }
    inline unsigned getUint(const char *key, unsigned defaultValue = 0) const override               { return Json::getUint(m_obj, key, defaultValue); }

    bool isEmpty() const override;

private:
    const rapidjson::Value &m_obj;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_JSON_H */
