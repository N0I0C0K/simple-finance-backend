/**
 *
 *  TokenRequired.cc
 *
 */

#include "TokenRequired.h"
#include <core/UserManager.hpp>

using namespace drogon;

void TokenRequired::doFilter(const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb)
{

    auto token = req->getHeader("token");
    if (token.length() == 0 || !UserManager::instance()->verify_token(token)) {
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k502BadGateway);
        res->setBody("token is invaild");
        fcb(res);
    }
    fccb();
}
