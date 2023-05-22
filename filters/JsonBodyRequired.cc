/**
 *
 *  JsonBodyRequired.cc
 *
 */

#include "JsonBodyRequired.h"

using namespace drogon;

void JsonBodyRequired::doFilter(const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb)
{
    if (req->getJsonObject() != nullptr || req->getMethod() == Options) {
        fccb();
        return;
    }
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k500InternalServerError);
    res->setBody(req->getJsonError());
    fcb(res);
}
