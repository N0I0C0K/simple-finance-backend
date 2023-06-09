#include <drogon/drogon.h>

int main()
{
    // Set HTTP listener address and port
    // drogon::app().addListener("0.0.0.0", 6110);
    // Load config file

    drogon::app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr&,
                                                 const drogon::HttpResponsePtr& resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
    });
    drogon::app().loadConfigFile("./config.json");
    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();

    return 0;
}
