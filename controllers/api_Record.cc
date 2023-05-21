#include "api_Record.h"
#include <UserManager.hpp>
#include <drogon/orm/Mapper.h>
#include <models/Record.h>

using namespace api;

// Add definition of your processing function here
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