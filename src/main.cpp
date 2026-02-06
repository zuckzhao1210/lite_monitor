#include "common/logger.hpp"
#include "common/configs_loader.h"
#include "common/parameters.h"


int main()
{
    // 加载配置文件
    ConfigsLoader loader;
    Parameters params;
    loader.initConfigs("./configs/lite_monitor.yaml", params);

    initLogger(params.log_params);
    return 0;
}