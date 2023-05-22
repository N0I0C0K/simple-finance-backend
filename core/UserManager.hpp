#pragma once

#include <ctime>
#include <drogon/drogon.h>
#include <drogon/orm/Mapper.h>
#include <iostream>
#include <map>
#include <models/Record.h>
#include <models/User.h>

using namespace std;

class LoginUser {
private:
public:
    const string id;
    const string token;
    LoginUser(const string& id, const string& token)
        : id(id)
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

    drogon_model::sqlite3::User get_user(const string& token) const
    {
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        auto id = this->token_dic.at(token)->id;
        auto target = mp.findOne(Criteria(DUser::Cols::_id, CompareOperator::EQ, id));

        return target;
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
        token_dic[token] = make_shared<LoginUser>(*target.getId(), token);
        return token;
    }

    void logout(const string& token)
    {
        token_dic.erase(token);
    }

    string user_register(const string& username, const string& password)
    {
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        if (mp.count(Criteria(DUser::Cols::_username, CompareOperator::EQ, username)) > 0) {
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

    void add_user_balance(drogon_model::sqlite3::User& user, double money)
    {
        if (*user.getBalance() + money < 0) {
            throw exception("money is invaild");
        }
        user.setBalance(*user.getBalance() + money);
        calculateInterest(user);
    }

    /// @brief 转出交易
    /// @param fromUser 来自用户
    /// @param toUser 转去用户, 空为转出本系统
    /// @param money 转账金额
    void money_deal(const string& fromUserId, const string& toUserId, double money)
    {
        if (money < 0) {
            throw exception("money must be positive");
        }
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        auto from = mp.findOne(Criteria(DUser::Cols::_id, CompareOperator::EQ, fromUserId));
        add_user_balance(from, -money);
        mp.update(from);
        if (toUserId.length() != 0) {
            auto to = mp.findOne(Criteria(DUser::Cols::_id, CompareOperator::EQ, toUserId));
            add_user_balance(to, money);
            mp.update(to);
        }

        new_record(*from.getId(), money, std::format("转账给 {} {}", toUserId.length() == 0 ? "外部" : toUserId, money), toUserId);
    }

    void userSaveMoney(const string& username, double money)
    {
        if (money < 0) {
            throw exception("money must be positive");
        }
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        auto user = mp.findOne(Criteria(DUser::Cols::_username, CompareOperator::EQ, username));
        add_user_balance(user, money);
        mp.update(user);
        new_record(*user.getId(), money, std::format("{} 存入 {}", username, money), "");
    }

    void userWithdrawMoney(const string& username, double money)
    {
        if (money < 0) {
            throw exception("money must be positive");
        }
        using namespace drogon::orm;
        using DUser = drogon_model::sqlite3::User;
        auto db = drogon::app().getDbClient();
        Mapper<DUser> mp(db);
        auto user = mp.findOne(Criteria(DUser::Cols::_username, CompareOperator::EQ, username));
        if (*user.getBalance() < money) {
            throw exception("balance not enough");
        }
        add_user_balance(user, -money);
        mp.update(user);
        new_record(*user.getId(), money, std::format("{} 取出 {}", username, money), "");
    }

    void calculateInterest(drogon_model::sqlite3::User& user)
    {
        auto now = time(nullptr);
        auto last = *user.getLastdealtime();
        auto diff = now - last;
        auto interest = diff * (*user.getBalance()) * 0.0005 / 31536000.0;
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
            auto inter = *user.getInterest();
            user.setBalance(*user.getBalance() + inter);
            user.setInterest(0);
            mp.update(user);
            new_record(*user.getId(), inter, std::format("{} 利息结算 {}", *user.getUsername(), inter), "");
        }
    }

    void new_record(const string& fromUserId, double money, const string& commeny, const string& toUserId = "")
    {
        using namespace drogon::orm;
        using DRecord = drogon_model::sqlite3::Record;
        auto db = drogon::app().getDbClient();
        Mapper<DRecord> mp(db);
        DRecord new_record;
        new_record.setId(drogon::utils::getUuid());
        new_record.setFromuserid(fromUserId);
        if (toUserId.length() != 0) {
            new_record.setTouserid(toUserId);
        }
        new_record.setMoney(money);
        new_record.setComment(commeny);
        new_record.setDealtime(time(nullptr));
        mp.insert(new_record);
        LOG_DEBUG << "new record: " << Json::FastWriter().write(new_record.toJson());
    }
};