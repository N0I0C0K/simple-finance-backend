#pragma once

#include <drogon/HttpController.h>

namespace api {
using namespace drogon;
class User : public drogon::HttpController<User> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(User::login, "/login", Post, Options, "JsonBodyRequired");
    METHOD_ADD(User::verify_token, "/verify_token", Get, Options, "TokenRequired");
    METHOD_ADD(User::register_user, "/register", Post, Options, "JsonBodyRequired");
    METHOD_ADD(User::user_info, "/info", Get, Options, "TokenRequired");
    METHOD_ADD(User::money_deal, "/money_deal", Post, Options, "TokenRequired", "JsonBodyRequired");
    METHOD_ADD(User::save_money, "/save_money", Post, Options, "TokenRequired", "JsonBodyRequired");
    METHOD_ADD(User::withdraw_money, "/withdraw_money", Post, Options, "TokenRequired", "JsonBodyRequired");
    METHOD_LIST_END

    void verify_token(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void login(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void register_user(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void user_info(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void money_deal(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void save_money(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void withdraw_money(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback);
};
}
