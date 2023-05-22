#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
class Record : public drogon::HttpController<Record> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Record::get_record_list, "/list", Get, Options, "TokenRequired");
    METHOD_ADD(Record::get_this_month_info, "/this_month_summary", Get, Options, "TokenRequired");
    METHOD_LIST_END
    void get_record_list(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback) const;
    void get_this_month_info(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback) const;
};
}
