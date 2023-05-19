#include <drogon/HttpController.h>
#include <iostream>

drogon::HttpResponsePtr badRequest(
    const std::string& msg = "Bad Request",
    drogon::HttpStatusCode code = drogon::HttpStatusCode::k500InternalServerError)
{
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(code);
    res->setBody(msg);
    return std::move(res);
}