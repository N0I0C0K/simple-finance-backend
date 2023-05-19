/**
 *
 *  JsonBodyRequired.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;

class JsonBodyRequired : public HttpFilter<JsonBodyRequired> {
public:
    JsonBodyRequired() { }
    void doFilter(const HttpRequestPtr& req,
        FilterCallback&& fcb,
        FilterChainCallback&& fccb) override;
};
