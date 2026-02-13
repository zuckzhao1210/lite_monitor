#ifndef MYSQL_CONFIG_H
#define MYSQL_CONFIG_H
#pragma once
#include <string>

struct DBConfig {
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    unsigned int port = 3306;

    unsigned int connect_timeout = 3;
    unsigned int read_timeout = 3;
    unsigned int write_timeout = 3;
    unsigned int pool_timeout_ms = 3000;

    size_t pool_size = 10;
};

#endif
