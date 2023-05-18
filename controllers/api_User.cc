#include "api_User.h"
#include "utils.hpp"
#include <UserManager.hpp>

// Add definition of your processing function here
using namespace api;

void User::login(const HttpRequestPtr& req,
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