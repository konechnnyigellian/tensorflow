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


#include "base/net/http/HttpResponse.h"
#include "3rdparty/http-parser/http_parser.h"
#include "base/io/log/Log.h"
#include "base/net/http/HttpContext.h"


#include <cinttypes>
#include <cstring>
#include <sstream>
#include <uv.h>


namespace tenserflow {


static const char *kCRLF      = "\r\n";
static const char *kUserAgent = "user-agent";


} // namespace tenserflow


tenserflow::HttpResponse::HttpResponse(uint64_t id, int statusCode) :
    m_id(id),
    m_statusCode(statusCode)
{
}


bool tenserflow::HttpResponse::isAlive() const
{
    auto ctx = HttpContext::get(m_id);

    return ctx && uv_is_writable(ctx->stream());
}


void tenserflow::HttpResponse::end(const char *data, size_t size)
{
    if (!isAlive()) {
        return;
    }

    if (data && !size) {
        size = strlen(data);
    }

    if (size) {
        setHeader("Content-Length", std::to_string(size));
    }

    setHeader("Connection", "close");

    std::stringstream ss;
    ss << "HTTP/1.1 " << statusCode() << " " << http_status_str(static_cast<http_status>(statusCode())) << kCRLF;

    for (auto &header : m_headers) {
        ss << header.first << ": " << header.second << kCRLF;
    }

    ss << kCRLF;

    auto ctx         = HttpContext::get(m_id);
    std::string body = data ? (ss.str() + std::string(data, size)) : ss.str();

#   ifndef APP_DEBUG
    if (statusCode() >= 400)
#   endif
    {
        const bool err = statusCode() >= 400;

        Log::print(err ? Log::ERR : Log::INFO, CYAN("%s ") CLEAR MAGENTA_BOLD("%s") WHITE_BOLD(" %s ") CSI "1;%dm%d " CLEAR WHITE_BOLD("%zu ") CYAN_BOLD("%" PRIu64 "ms ") BLACK_BOLD("\"%s\""),
                   ctx->ip().c_str(),
                   http_method_str(static_cast<http_method>(ctx->method)),
                   ctx->url.c_str(),
                   err ? 31 : 32,
                   statusCode(),
                   body.size(),
                   ctx->elapsed(),
                   ctx->headers.count(kUserAgent) ? ctx->headers.at(kUserAgent).c_str() : nullptr
                   );
    }

    ctx->write(std::move(body), true);
}
