#include "common/logger.h"
#include "common/config/configs_loader.h"
#include "common/utils/path_utils.h"
#include "core/http/http_server.h"

int main(int argc, char* argv[])
{
    // 加载配置文件
    std::string config_path = PathUtils::getConfigsPath();
    // 如果命令行输入了路径，则覆盖默认值
    if (argc > 1) {
        config_path = argv[1];
    }

    ConfigsLoader loader;
    Parameters params;
    loader.initConfigs(config_path, params);

    // 初始化日志
    initLogger(params.log_params);

    // todo: 开启接收
    

    // 开启HTTP服务
    HTTPServer http_server(params.http_server_params.host, params.http_server_params.port);
    http_server.start();

    return 0;
}