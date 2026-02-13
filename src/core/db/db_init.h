/**
*****************************************************************************
*  @file    db_init.h
*  @brief   数据库初始化功能
*
*  @author  zuckzhao
*  @date    2026-02-09
*  @version V0.1.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note V0.1.0   zuckzhao  2026-02-09   1.创建
*
*****************************************************************************
*/

#ifndef DB_INIT_H
#define DB_INIT_H
#pragma once
#include <string>

class DBInit
{
  public:
    DBInit(const std::string &init_sql_script_path);
    ~DBInit();

    /**
     * @brief 执行数据库初始化
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initDB();

    /**
     * @brief 执行数据库重置
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool resetDB();

  private:
    std::string init_sql_script_path_;
};

#endif