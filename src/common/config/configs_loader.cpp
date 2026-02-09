/**
*****************************************************************************
*  @file    configs_loader.cpp
*  @brief   参数加载器
*
*  @author  zuckzhao
*  @date    2026-02-06
*  @version V0.1.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note V0.1.0   zuckzhao   2026-02-06   1.创建
*
*****************************************************************************
*/
#include "configs_loader.h"

bool ConfigsLoader::initConfigs(const std::string& config_file_path, Parameters& params)
{
    // 校验配置文件路径是否存在
    if (access(config_file_path.c_str(), F_OK) != 0)
    {
        spdlog::error("[ConfigsLoader::initConfigs] Config file not found: {}", config_file_path);
        return false;
    }

    // 加载配置文件参数
    if (!loadConfigs(config_file_path, params))
    {
        return false;
    }

    // 校验配置参数是否合法
    if (!validateConfigs(params))
    {
        spdlog::error("[ConfigsLoader::initConfigs] Invalid config file: {}", config_file_path);
        return false;
    }

    return true;
}

bool ConfigsLoader::loadConfigs(const std::string& config_file_path, Parameters &params)
{
    // 从配置文件中加载参数
    try
    {
        YAML::Node config = YAML::LoadFile(config_file_path);
        // 加载数据库配置
        if (!config["database"])
        {
            spdlog::error("[ConfigsLoader::loadConfigs] Database config not found in config file");
            return false;
        }
        else {
            if (config["database"]["host"])
                params.db_params.host = config["database"]["host"].as<std::string>();
            if (config["database"]["port"])
                params.db_params.port = config["database"]["port"].as<int>();
            if (config["database"]["username"])
                params.db_params.user = config["database"]["username"].as<std::string>();
            if (config["database"]["password"])
                params.db_params.password = config["database"]["password"].as<std::string>();
            if (config["database"]["database_name"])
                params.db_params.db_name = config["database"]["database_name"].as<std::string>(); 
        }

        // 校验监控数据保存时间配置是否存在
        if (!config["retention"])
        {
            spdlog::error("[ConfigsLoader::loadConfigs] Retention config not found in config file");
            return false;
        }
        else {
            if (config["retention"]["history_retention_days"])
                params.retention_params.history_retention_days = config["retention"]["history_retention_days"].as<int>();
            if (config["retention"]["trend_retention_days"])
                params.retention_params.trend_retention_days = config["retention"]["trend_retention_days"].as<int>();
        }

        // 校验日志参数
        if (!config["log"])
        {
            spdlog::error("[ConfigsLoader::loadConfigs] Log config not found in config file");
            return false;
        }
        else {
            if (config["log"]["log_dir"])
                params.log_params.log_dir = config["log"]["log_dir"].as<std::string>();
            if (config["log"]["log_level"])
                params.log_params.log_level = config["log"]["log_level"].as<std::string>();
            if (config["log"]["max_log_file_size"])
                params.log_params.max_log_file_size = config["log"]["max_log_file_size"].as<int>();
            if (config["log"]["max_log_file_num"])
                params.log_params.max_log_file_num = config["log"]["max_log_file_num"].as<int>();
        }

        if (!config["http_server"])
        {
            spdlog::error("[ConfigsLoader::loadConfigs] HTTP server config not found in config file");
            return false;
        }
        else {
            if (config["http_server"]["host"])
                params.http_server_params.host = config["http_server"]["host"].as<std::string>();
            if (config["http_server"]["port"])
                params.http_server_params.port = config["http_server"]["port"].as<int>();
        }

        if (!config["tcp_server"])
        {
            spdlog::error("[ConfigsLoader::loadConfigs] TCP server config not found in config file");
            return false;
        }
        else {
            if (config["tcp_server"]["host"])
                params.tcp_server_params.host = config["tcp_server"]["host"].as<std::string>();
            if (config["tcp_server"]["port"])
                params.tcp_server_params.port = config["tcp_server"]["port"].as<int>();
        }
    }
    catch(const std::exception& e)
    {
        spdlog::error("[ConfigsLoader::loadConfigs] Failed to load config file: {}", e.what());
        return false;
    }

    return true;
}

bool ConfigsLoader::validateConfigs(Parameters &params)
{   
    // 1. 校验 Host (非空且长度合法)
    if (params.db_params.host.empty() || params.db_params.host.length() > 255) {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid host: {}", params.db_params.host);
        return false;
    }

    // 2. 校验 Port (MySQL 端口范围通常是 1-65535)
    if (params.db_params.port <= 0 || params.db_params.port > 65535)
    {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid port: {}", params.db_params.port);
        return false;
    }

    // 3. 校验用户名 (非空)
    if (params.db_params.user.empty()) {
        spdlog::error("[ConfigsLoader::validateConfigs] Database user is empty");
        return false;
    }

    // 4. 校验密码 (非空)
    if (params.db_params.password.empty()) {
        spdlog::error("[ConfigsLoader::validateConfigs] Database password is empty");
        return false;
    }

    // 5. 校验数据库名 (非空)
    if (params.db_params.db_name.empty()) {
        spdlog::error("[ConfigsLoader::validateConfigs] Database name is empty");
        return false;
    }

    // 6. 校验日志参数
    if (params.log_params.log_dir.empty()) {
        spdlog::error("[ConfigsLoader::validateConfigs] Log directory is empty");
        params.log_params.log_dir = "./log";
        return false;
    }

    // 7. 校验日志级别
    if (params.log_params.log_level.empty()) {
        spdlog::error("[ConfigsLoader::validateConfigs] Log level is empty");
        params.log_params.log_level = "info";
        return false;
    }

    // 8. 校验日志文件大小 (非负)
    if (params.log_params.max_log_file_size <= 0) {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid max log file size: {}", params.log_params.max_log_file_size);
        params.log_params.max_log_file_size = 100;
        return false;
    }

    // 9. 校验日志文件数量 (非负)
    if (params.log_params.max_log_file_num <= 0) {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid max log file num: {}", params.log_params.max_log_file_num);
        params.log_params.max_log_file_num = 10;
        return false;
    }

    // 校验监控数据保存时间配置是否合法
    if (params.retention_params.history_retention_days < -1 || params.retention_params.trend_retention_days < -1)
    {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid retention config");
        return false;
    }

    // 10. 校验 HTTP 服务器参数
    if (params.http_server_params.host.empty()) {
        spdlog::error("[ConfigsLoader::validateConfigs] HTTP server host is empty");
        params.http_server_params.host = "localhost";
        return false;
    }
    if (params.http_server_params.port <= 0 || params.http_server_params.port > 65535) {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid HTTP server port: {}", params.http_server_params.port);
        params.http_server_params.port = 20317;
        return false;
    }

    // 11. 校验 TCP 服务器参数
    if (params.tcp_server_params.host.empty()) {
        spdlog::error("[ConfigsLoader::validateConfigs] TCP server host is empty");
        params.tcp_server_params.host = "localhost";
        return false;
    }
    if (params.tcp_server_params.port <= 0 || params.tcp_server_params.port > 65535) {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid TCP server port: {}", params.tcp_server_params.port);
        params.tcp_server_params.port = 21210;
        return false;
    }

    return true;
}