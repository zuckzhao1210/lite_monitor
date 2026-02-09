/**
*****************************************************************************
*  @file    parameters.h
*  @brief   系统参数定义
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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

struct DatabaseParams
{
    std::string host;
    int port;
    std::string user;
    std::string password;
    std::string db_name;
};

struct RetentionParams
{
    int history_retention_days;
    int trend_retention_days;
};

struct LogParams
{
    std::string log_dir;
    int max_log_file_size;
    int max_log_file_num;
    std::string log_level;
};

struct HTTPServerParams
{
    std::string host;
    int port;
};

struct TCPParams
{
    std::string host;
    int port;
};

struct OtherParams
{
};

struct Parameters
{
    DatabaseParams db_params;
    RetentionParams retention_params;
    LogParams log_params;
    HTTPServerParams http_server_params;
    TCPParams tcp_server_params;
    OtherParams other_params;
};

#endif