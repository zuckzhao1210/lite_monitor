/**
*****************************************************************************
*  @file    logger.cpp
*  @brief   日志设置
*
*  @author  zuckzhao
*  @date    2026-02-06
*  @version V0.1.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note V0.1.0   zuckzhao    2018-2-15   1.创建
*
*****************************************************************************
*/

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "spdlog/sinks/daily_file_sink.h"    // 按日期滚动
#include "spdlog/sinks/rotating_file_sink.h" // 按大小滚动
#include "spdlog/sinks/stdout_color_sinks.h" // 控制台彩色输出
#include "spdlog/spdlog.h"
#include "parameters.h"

#include <memory>
#include <vector>
#include <iostream>

void initLogger(const LogParams &params)
{
    std::string log_dir = params.log_dir;
    std::string log_level = params.log_level;
    int max_log_file_size = params.max_log_file_size * 1024 * 1024;
    int max_log_file_num = params.max_log_file_num;
    try
    {
        // 1. 转换全局日志级别
        spdlog::level::level_enum global_level = spdlog::level::from_str(log_level);

        // 2. 创建 Sinks

        // --- 控制台 Sink ---
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(global_level);

        // --- Info Sink: 存放所有等级 (只要 >= global_level) ---
        auto info_sink =
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_dir + "/lite_monitor.log", max_log_file_size, max_log_file_num);
        info_sink->set_level(global_level);

        // --- Warn Sink: 存放 Warn 等级 ---
        auto warn_sink =
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_dir + "/lite_monitor.warn", max_log_file_size, max_log_file_num);
        warn_sink->set_level(spdlog::level::warn);

        // --- Error Sink: 只存放 Error ---
        auto error_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_dir + "/lite_monitor.error",
                                                                                 max_log_file_size, max_log_file_num);
        error_sink->set_level(spdlog::level::err);

        // 3. 组合 Logger
        std::vector<spdlog::sink_ptr> sinks{console_sink, info_sink, warn_sink, error_sink};
        auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());

        // 4. 应用配置
        logger->set_level(global_level);
        spdlog::set_default_logger(logger);

        // 设置格式
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        // 自动刷新设置
        spdlog::flush_on(spdlog::level::warn);
        spdlog::flush_every(std::chrono::seconds(3));

        spdlog::info("[initLogger] dir={}, level={}, max_size={}MB, max_files={}", log_dir, log_level,
                     params.max_log_file_size, params.max_log_file_num);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

#endif
