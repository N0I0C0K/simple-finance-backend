#pragma once

#include <drogon/drogon.h>
#include <drogon/orm/Mapper.h>
#include <iostream>
#include <map>
#include <models/User.h>

#include <core/utils.hpp>

using namespace std;

class LoginUser {
private:
    const string name;
    const string token;

public:
    LoginUser(const string& name, const string& token)
        : name(name)
        , token(token)
    {
    }
};

class UserManager {
private:
    /* data */
    map<string, shared_ptr<LoginUser>> token_dic;
    UserManager()
    {
    }
    ~UserManager()
    {
    }

    static UserManager* instance;

public:
    const UserManager* instance() noexcept
    {
        if (this->instance == nullptr) {
            this->instance = new UserManager();
        }
        return this->instance;
    }

    bool verify_token(const string& token) const
    {
        return this->token_dic.contains(token);
    }

    string login(const string& username, const string& password)
    {
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);

        auto target = mp.findOne(Criteria(DUser::Cols::_name, CompareOperator::EQ, username));
        if (*target.getPassword() != password) {
            throw exception("password eror");
        }

        auto token = drogon::utils::getUuid();
        token_dic[token] = make_shared<LoginUser>(username, token);
        return token;
    }
};