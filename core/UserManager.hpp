#pragma once

#include <ctime>
#include <drogon/drogon.h>
#include <drogon/orm/Mapper.h>
#include <iostream>
#include <map>
#include <models/User.h>

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

public:
    UserManager()
    {
    }
    ~UserManager()
    {
    }
    static shared_ptr<UserManager> instance() noexcept
    {
        static shared_ptr<UserManager> s_instance = make_shared<UserManager>();
        return s_instance;
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
        auto target = mp.findOne(Criteria(DUser::Cols::_username, CompareOperator::EQ, username));
        if (*target.getPassword() != password) {
            throw exception("password eror");
        }

        auto token = drogon::utils::getUuid();
        token_dic[token] = make_shared<LoginUser>(username, token);
        return token;
    }

    string user_register(const string& username, const string& password)
    {
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        if (mp.count(Criteria(DUser::Cols::_username, CompareOperator::EQ, username))) {
            throw exception("user already exists");
        }
        DUser new_user;
        new_user.setId(drogon::utils::getUuid());
        new_user.setLastdealtime(time(nullptr));
        new_user.setUsername(username);
        new_user.setPassword(password);
        mp.insert(new_user);
        return login(username, password);
    }

    /// @brief 交易
    /// @param fromUser 来自用户
    /// @param toUser 转去用户, 空为转出本系统
    /// @param balance 转账金额
    void money_deal(const string& fromUser, const string& toUser, double balance)
    {
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        auto from = mp.findOne(Criteria(DUser::Cols::_username, CompareOperator::EQ, fromUser));
        if (*from.getBalance() < balance) {
            throw exception("balance not enough");
        }
        from.setBalance(*from.getBalance() - balance);

        if (toUser.length() != 0) {
            auto to = mp.findOne(Criteria(DUser::Cols::_username, CompareOperator::EQ, toUser));
            to.setBalance(*to.getBalance() + balance);
            calculateInterest(to);
            mp.update(to);
        }

        calculateInterest(from);
        mp.update(from);
    }

    void logout(const string& token)
    {
        token_dic.erase(token);
    }

    void calculateInterest(drogon_model::sqlite3::User& user)
    {
        auto now = time(nullptr);
        auto last = *user.getLastdealtime();
        auto diff = now - last;
        auto interest = diff * (*user.getBalance()) * 0.0001;
        user.setInterest(*user.getInterest() + interest);
        user.setLastdealtime(now);
    }

    void settlementAllUserInterest()
    {
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        auto all = mp.findAll();
        for (auto& user : all) {
            calculateInterest(user);
            mp.update(user);
        }
    }
};