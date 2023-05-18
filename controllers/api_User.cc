#include "api_User.h"
#include "utils.hpp"
#include <UserManager.hpp>

// Add definition of your processing function here
using namespace api;

void User::login(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    std::string username = req->getParameter("username"), password = req->getParameter("password");
    LOG_DEBUG << username << password;
    if (username.length() == 0 || password.length() == 0) {
        callback(badRequest());
        return;
    }
    auto token = UserManager::instance()->login(username, password);
    Json::Value ret;
    ret["token"] = token;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::register_user(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto json = *(req->getJsonObject());
    auto username = json["username"].asString();
    auto password = json["password"].asString();

    if (username.length() == 0 || password.length() == 0) {
        callback(badRequest());
        return;
    }

    auto token = UserManager::instance()->user_register(username, password);

    Json::Value ret;
    ret["token"] = token;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::money_deal(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto json = *(req->getJsonObject());
    auto fromUser = json["fromUser"].asString();
    auto toUser = json["toUser"].asString();
    auto balance = json["balance"].asDouble();
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("money_deal");
    callback(resp);
}