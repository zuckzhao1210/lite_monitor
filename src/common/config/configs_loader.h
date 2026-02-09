/**
*****************************************************************************
*  @file    configs_loader.h
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

#ifndef CONFIGS_LOADER_H
#define CONFIGS_LOADER_H
#pragma once

#include <unistd.h>

#include "parameters.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

class ConfigsLoader
{
  public:
    ConfigsLoader() = default;
    ~ConfigsLoader() = default;

    /**
     * @brief 初始化参数配置
     * 
     * @param config_file_path 配置文件路径
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initConfigs(const std::string& config_file_path, Parameters& params);

  private:
    /**
     * @brief 读取配置文件参数
     *
     * @param params 配置参数结构体
     * @return true 加载成功
     * @return false 加载失败
     */
    bool loadConfigs(const std::string& config_file_path, Parameters &params);

    /**
     * @brief 校验配置参数是否合法
     *
     * @param params 配置参数结构体
     * @return true 校验成功
     * @return false 校验失败
     */
    bool validateConfigs(Parameters &params);
};

#endif