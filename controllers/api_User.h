#pragma once

#include <drogon/HttpController.h>

using namespace drogon;
namespace api {
class User : public drogon::HttpController<User> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(User::login, "/login", Get);
    METHOD_ADD(User::register_user, "/register", Get);
    METHOD_ADD(User::money_deal, "/money_deal", Get, "TokenRequired");
    METHOD_LIST_END

    void login(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void register_user(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void money_deal(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);
};
}
