#include "http_server.h"

HTTPServer::HTTPServer(const std::string& host, int port) : host_(host), port_(port)
{
}

HTTPServer::~HTTPServer()
{
    stop();
}

void HTTPServer::start()
{
    set_routes();
    server_.listen(host_.c_str(), port_);
    spdlog::info("[HTTPServer::start] HTTP server {}:{} start", host_, port_);
}

void HTTPServer::stop()
{
    spdlog::info("[HTTPServer::stop] HTTP server {}:{} stop", host_, port_);
    server_.stop();
}

void HTTPServer::set_routes()
{
    // 测试路由
    server_.Get("/hello", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Hello, World!", "text/plain");
    });

    // todo: 其他路由
}