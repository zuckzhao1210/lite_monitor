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

#pragma once
#include "configs_loader.h"

bool ConfigsLoader::initConfigs(const std::string& config_file_path, Parameters& params)
{
    // 校验配置文件路径是否存在
    if (!access(config_file_path.c_str(), F_OK))
    {
        spdlog::error("[ConfigsLoader::initConfigs] Config file not found: {}", config_file_path);
        return false;
    }

    // 加载配置文件参数
    if (!loadConfigs(params))
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

bool ConfigsLoader::loadConfigs(Parameters &params)
{
    // 从配置文件中加载参数
    try
    {
        YAML::Node config = YAML::LoadFile("config.yaml");
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
    }
    catch(const std::exception& e)
    {
        spdlog::error("[ConfigsLoader::loadConfigs] Failed to load config file: {}", e.what());
        return false;
    }

    return true;
}

bool ConfigsLoader::validateConfigs(const Parameters &params)
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

    // 校验监控数据保存时间配置是否合法
    if (params.retention_params.history_retention_days < -1 || params.retention_params.trend_retention_days < -1)
    {
        spdlog::error("[ConfigsLoader::validateConfigs] Invalid retention config");
        return false;
    }
}