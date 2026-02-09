
#ifndef PATH_UTILS_H
#define PATH_UTILS_H
#pragma once

#include <string>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <stdexcept>

class PathUtils {
public:

    static const std::string CONFIGS_PATH;
    static const std::string INIT_SQL_SCRIPT_PATH;
    static const std::string PARTITION_SQL_SCRIPT_PATH;
    
    /**
     * @brief 获取当前运行的可执行文件所在的绝对目录
     * @return 目录路径，末尾不带斜杠 (例如: /home/zhaozhe/project/output/bin)
     */
    static std::string getModuleDir();

    /**
     * @brief 基于执行文件目录拼接相对路径
     * @param relative_path 相对路径，如 "../etc/config.yaml"
     */
    static std::string joinAppPath(const std::string& relative_path);
    /**
     * @brief 基于执行文件目录拼接SQL脚本路径
     * @return SQL脚本路径，如 "../scripts/init.sql"
     */
    static std::string getInitSqlScriptPath();

    /**
     * @brief 获取配置文件路径
     * @return 配置文件路径，如 "../etc/lite_monitor.yaml"
     */
    static std::string getConfigsPath();
};

#endif