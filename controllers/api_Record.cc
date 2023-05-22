#include "api_Record.h"
#include <UserManager.hpp>
#include <drogon/orm/Mapper.h>
#include <models/Record.h>

using namespace api;

void Record::get_record_list(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{
    using namespace drogon::orm;
    using DRecord = drogon_model::sqlite3::Record;
    auto token = req->getHeader("token");
    auto user = UserManager::instance()->get_user(token);
    auto id = *user.getId();
    Mapper<DRecord> mapper(drogon::app().getDbClient());
    auto records = mapper.findBy(
        Criteria(DRecord::Cols::_fromUserId, CompareOperator::EQ, id));
    Json::Value ret;
    Json::Value list(Json::arrayValue);
    for (auto& record : records) {
        Json::Value item;
        list.append(record.toJson());
    }
    ret["list"] = list;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

std::time_t get_first_day_of_month(std::time_t time_val)
{
    std::tm* time_info = std::gmtime(&time_val);
    // time_info 结构体中的 tm_mday 表示当前日期在月份中的天数，将其设置为 1 即可获取该月第一天的时间
    time_info->tm_mday = 1;
    time_info->tm_hour = 0;
    time_info->tm_min = 0;
    time_info->tm_sec = 0;
    return std::mktime(time_info);
}

void Record::get_this_month_info(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{
    using namespace drogon::orm;
    using DRecord = drogon_model::sqlite3::Record;
    auto token = req->getHeader("token");
    auto user = UserManager::instance()->get_user(token);
    auto id = *user.getId();
    Mapper<DRecord> mapper(drogon::app().getDbClient());
    auto records = mapper.findBy(
        Criteria(DRecord::Cols::_fromUserId, CompareOperator::EQ, id) && Criteria(DRecord::Cols::_dealTime, CompareOperator::GE, get_first_day_of_month(std::time(nullptr))));
    Json::Value ret;
    double expend = 0, income = 0;
    for (auto& record : records) {
        record.getMoney() > 0 ? income += *record.getMoney() : expend += *record.getMoney();
    }
    ret["expend"] = expend;
    ret["income"] = income;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}