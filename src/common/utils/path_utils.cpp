#include "path_utils.h"

const std::string PathUtils::CONFIGS_PATH = "../etc/lite_monitor.yaml";
const std::string PathUtils::INIT_SQL_SCRIPT_PATH = "../scripts/init.sql";
const std::string PathUtils::PARTITION_SQL_SCRIPT_PATH = "../scripts/partition.sql";

std::string PathUtils::getModuleDir()
{
    char buffer[PATH_MAX];
    // Linux 特有：通过 /proc/self/exe 获取自身路径
    ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);

    if (count == -1)
    {
        throw std::runtime_error("Failed to read /proc/self/exe");
    }

    std::string full_path(buffer, count);

    // 找到最后一个斜杠，截取目录部分
    size_t last_slash = full_path.find_last_of("/");
    if (last_slash == std::string::npos)
    {
        return ".";
    }

    return full_path.substr(0, last_slash);
}

/**
 * @brief 基于执行文件目录拼接相对路径
 * @param relative_path 相对路径，如 "../etc/config.yaml"
 */
std::string PathUtils::joinAppPath(const std::string &relative_path)
{
    return getModuleDir() + "/" + relative_path;
}

/**
 * @brief 基于执行文件目录拼接SQL脚本路径
 * @return SQL脚本路径，如 "../scripts/init.sql"
 */
std::string PathUtils::getInitSqlScriptPath()
{
    return getModuleDir() + "/" + INIT_SQL_SCRIPT_PATH;
}

/**
 * @brief 获取配置文件路径
 * @return 配置文件路径，如 "../etc/lite_monitor.yaml"
 */
std::string PathUtils::getConfigsPath()
{
    return getModuleDir() + "/" + CONFIGS_PATH;
}