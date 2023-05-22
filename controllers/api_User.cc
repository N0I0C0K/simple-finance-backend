#include "api_User.h"
#include "utils.hpp"
#include <UserManager.hpp>

// Add definition of your processing function here
namespace api {

void User::verify_token(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto res = HttpResponse::newHttpResponse();
    res->setStatusCode(k200OK);
    callback(res);
}

void User::login(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto jsonbody = *req->getJsonObject();
    std::string username = jsonbody["username"].asString(), password = jsonbody["password"].asString();
    LOG_DEBUG << username << password;
    if (username.length() == 0 || password.length() == 0) {
        callback(badRequest());
        return;
    }
    try {
        auto token = UserManager::instance()->login(username, password);
        Json::Value ret;
        ret["token"] = token;
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    } catch (const std::exception& e) {
        LOG_DEBUG << e.what();
        callback(badRequest());
    }
}

void User::logout(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto token = req->getHeader("token");
    UserManager::instance()->logout(token);
    auto res = HttpResponse::newHttpResponse();
    res->setStatusCode(k200OK);
    callback(res);
}

void User::user_info(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto token = req->getHeader("token");
    auto user = UserManager::instance()->get_user(token);
    Json::Value ret;
    ret["username"] = *user.getUsername();
    ret["balance"] = *user.getBalance();
    ret["lastDealTime"] = *user.getLastdealtime();
    ret["interest"] = *user.getInterest();

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::register_user(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto&& json = *(req->getJsonObject());
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
    auto jsonptr = req->getJsonObject();
    if (jsonptr == nullptr) {
        callback(badRequest());
        return;
    }
    auto&& json = *(jsonptr);
    auto fromUser = UserManager::instance()->get_user(req->getHeader("token"));
    string toUserId = "";
    if (json.isMember("toUserId")) {
        toUserId = json["toUserId"].asString();
    }
    auto money = json["money"].asDouble();

    if (money <= 0) {
        callback(badRequest());
        return;
    }

    try {
        UserManager::instance()->money_deal(*fromUser.getId(), toUserId, money);
    } catch (const std::exception& e) {
        LOG_DEBUG << e.what();
        callback(badRequest(e.what()));
        return;
    }

    Json::Value ret;
    ret["code"] = 0;
    ret["msg"] = "success";

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::save_money(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto jsonptr = req->getJsonObject();
    if (jsonptr == nullptr) {
        callback(badRequest());
        return;
    }
    auto&& json = *(jsonptr);
    auto user = UserManager::instance()->get_user(req->getHeader("token"));
    auto money = json["money"].asDouble();

    if (money <= 0) {
        callback(badRequest());
        return;
    }

    try {
        UserManager::instance()->userSaveMoney(*user.getUsername(), money);
    } catch (const std::exception& e) {
        LOG_DEBUG << e.what();
        callback(badRequest(e.what()));
        return;
    }

    Json::Value ret;
    ret["code"] = 0;
    ret["msg"] = "success";
    ret["balance"] = *user.getBalance() + money;

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::withdraw_money(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto json = *(req->getJsonObject());
    auto user = UserManager::instance()->get_user(req->getHeader("token"));
    auto money = json["money"].asDouble();

    if (money <= 0) {
        callback(badRequest());
        return;
    }
    try {
        UserManager::instance()->userWithdrawMoney(*user.getUsername(), money);
    } catch (const std::exception& e) {
        LOG_DEBUG << e.what();
        callback(badRequest(e.what()));
        return;
    }

    Json::Value ret;
    ret["code"] = 0;
    ret["msg"] = "success";
    ret["balance"] = *user.getBalance() - money;

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

};
