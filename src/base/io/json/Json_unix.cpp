


#include <fstream>


#include "base/io/json/Json.h"
#include "3rdparty/rapidjson/document.h"
#include "3rdparty/rapidjson/istreamwrapper.h"
#include "3rdparty/rapidjson/ostreamwrapper.h"
#include "3rdparty/rapidjson/prettywriter.h"


bool tenserflow::Json::get(const char *fileName, rapidjson::Document &doc)
{
    std::ifstream ifs(fileName, std::ios_base::in | std::ios_base::binary);
    if (!ifs.is_open()) {
        return false;
    }

    rapidjson::IStreamWrapper isw(ifs);
    doc.ParseStream<rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag>(isw);

    return !doc.HasParseError() && doc.IsObject();
}


bool tenserflow::Json::save(const char *fileName, const rapidjson::Document &doc)
{
    std::ofstream ofs(fileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (!ofs.is_open()) {
        return false;
    }

    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);

    doc.Accept(writer);

    return true;
}


bool tenserflow::Json::convertOffset(const char* fileName, size_t offset, size_t& line, size_t& pos, std::vector<std::string>& s)
{
    std::ifstream ifs(fileName, std::ios_base::in | std::ios_base::binary);
    if (!ifs.is_open()) {
        return false;
    }

    return convertOffset(ifs, offset, line, pos, s);
}
