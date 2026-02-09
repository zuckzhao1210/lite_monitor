#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once

#include "http/httplib.h"
#include "spdlog/spdlog.h"

class HTTPServer
{
  public:
    HTTPServer(const std::string &host, int port);
    ~HTTPServer();

    /**
     * @brief 启动HTTP服务器
     */
    void start();

    /**
     * @brief 停止HTTP服务器
     */
    void stop();

  private:
    /**
     * @brief 设置HTTP路由映射
     * @details 该函数负责设置HTTP路由映射，将不同的URL路径映射到对应的处理函数。
     */
    void set_routes();

  private:
    std::string host_;
    int port_;
    httplib::Server server_;
};
#endif