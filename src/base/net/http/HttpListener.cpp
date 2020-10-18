/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/net/http/HttpListener.h"
#include "3rdparty/http-parser/http_parser.h"
#include "base/io/log/Log.h"
#include "base/net/http/HttpData.h"


void tenserflow::HttpListener::onHttpData(const HttpData &data)
{
#   ifdef APP_DEBUG
    if (!data.isRequest()) {
        LOG_DEBUG("%s " CYAN_BOLD("http%s://%s:%u ") MAGENTA_BOLD("\"%s %s\" ") CSI "1;%dm%d" CLEAR BLACK_BOLD(" received: ") CYAN_BOLD("%zu") BLACK_BOLD(" bytes"),
                  m_tag, data.tlsVersion() ? "s" : "", data.host(), data.port(), http_method_str(static_cast<http_method>(data.method)), data.url.data(),
                  (data.status >= 400 || data.status < 0) ? 31 : 32, data.status, data.body.size());

        if (data.body.size() < (Log::kMaxBufferSize - 1024) && data.isJSON()) {
            Log::print(BLUE_BG_BOLD("%s:") BLACK_BOLD_S " %.*s", data.headers.at(HttpData::kContentTypeL).c_str(), static_cast<int>(data.body.size()), data.body.c_str());
        }
    }
#   endif

    m_listener->onHttpData(data);
}
