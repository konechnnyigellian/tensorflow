


#include "base/api/Httpd.h"
#include "3rdparty/http-parser/http_parser.h"
#include "base/api/Api.h"
#include "base/io/log/Log.h"
#include "base/net/http/HttpApiResponse.h"
#include "base/net/http/HttpData.h"
#include "base/net/tools/TcpServer.h"
#include "core/config/Config.h"
#include "core/Controller.h"


#ifdef TENSERFLOW_FEATURE_TLS
#   include "base/net/https/HttpsServer.h"
#else
#   include "base/net/http/HttpServer.h"
#endif


namespace tenserflow {

static const char *kAuthorization = "authorization";

#ifdef _WIN32
static const char *favicon = nullptr;
static size_t faviconSize  = 0;
#endif

} // namespace tenserflow


tenserflow::Httpd::Httpd(Base *base) :
    m_base(base)
{
    m_httpListener = std::make_shared<HttpListener>(this);

    base->addListener(this);
}


tenserflow::Httpd::~Httpd() = default;


bool tenserflow::Httpd::start()
{
    const auto &config = m_base->config()->http();

    if (!config.isEnabled()) {
        return true;
    }

    bool tls = false;

#   ifdef TENSERFLOW_FEATURE_TLS
    m_http = new HttpsServer(m_httpListener);
    tls = m_http->setTls(m_base->config()->tls());
#   else
    m_http = new HttpServer(m_httpListener);
#   endif

    m_server = new TcpServer(config.host(), config.port(), m_http);

    const int rc = m_server->bind();
    Log::print(GREEN_BOLD(" * ") WHITE_BOLD("%-13s") CSI "1;%dm%s:%d" " " RED_BOLD("%s"),
               "HTTP API",
               tls ? 32 : 36,
               config.host().data(),
               rc < 0 ? config.port() : rc,
               rc < 0 ? uv_strerror(rc) : ""
               );

    if (rc < 0) {
        stop();

        return false;
    }

    m_port = static_cast<uint16_t>(rc);

#   ifdef _WIN32
    HRSRC src = FindResource(nullptr, MAKEINTRESOURCE(1), RT_ICON);
    if (src != nullptr) {
        HGLOBAL res = LoadResource(nullptr, src);
        if (res != nullptr) {
            favicon     = static_cast<const char *>(LockResource(res));
            faviconSize = SizeofResource(nullptr, src);
        }
    }
#   endif

    return true;
}


void tenserflow::Httpd::stop()
{
    delete m_server;
    delete m_http;

    m_server = nullptr;
    m_http   = nullptr;
    m_port   = 0;
}



void tenserflow::Httpd::onConfigChanged(Config *config, Config *previousConfig)
{
    if (config->http() == previousConfig->http()) {
        return;
    }

    stop();
    start();
}


void tenserflow::Httpd::onHttpData(const HttpData &data)
{
    if (data.method == HTTP_OPTIONS) {
        return HttpApiResponse(data.id()).end();
    }

    if (data.method == HTTP_GET && data.url == "/favicon.ico") {
#       ifdef _WIN32
        if (favicon != nullptr) {
            HttpResponse response(data.id());
            response.setHeader(HttpData::kContentType, "image/x-icon");

            return response.end(favicon, faviconSize);
        }
#       endif

        return HttpResponse(data.id(), HTTP_STATUS_NOT_FOUND).end();
    }

    if (data.method > 4) {
        return HttpApiResponse(data.id(), HTTP_STATUS_METHOD_NOT_ALLOWED).end();
    }

    const int status = auth(data);
    if (status != HTTP_STATUS_OK) {
        return HttpApiResponse(data.id(), status).end();
    }

    if (data.method != HTTP_GET) {
        if (m_base->config()->http().isRestricted()) {
            return HttpApiResponse(data.id(), HTTP_STATUS_FORBIDDEN).end();
        }

        if (!data.headers.count(HttpData::kContentTypeL) || data.headers.at(HttpData::kContentTypeL) != HttpData::kApplicationJson) {
            return HttpApiResponse(data.id(), HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE).end();
        }
    }

    m_base->api()->request(data);
}


int tenserflow::Httpd::auth(const HttpData &req) const
{
    const Http &config = m_base->config()->http();

    if (!req.headers.count(kAuthorization)) {
        return config.isAuthRequired() ? HTTP_STATUS_UNAUTHORIZED : HTTP_STATUS_OK;
    }

    if (config.token().isNull()) {
        return HTTP_STATUS_UNAUTHORIZED;
    }

    const std::string &token = req.headers.at(kAuthorization);
    const size_t size        = token.size();

    if (token.size() < 8 || config.token().size() != size - 7 || memcmp("Bearer ", token.c_str(), 7) != 0) {
        return HTTP_STATUS_FORBIDDEN;
    }

    return strncmp(config.token().data(), token.c_str() + 7, config.token().size()) == 0 ? HTTP_STATUS_OK : HTTP_STATUS_FORBIDDEN;
}
