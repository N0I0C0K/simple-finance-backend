#include <drogon/HttpController.h>
#include <iostream>

drogon::HttpResponsePtr badRequest(
    drogon::HttpStatusCode code = drogon::HttpStatusCode::k500InternalServerError)
{
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(code);
    return std::move(res);
}