/**
*****************************************************************************
*  @file    db_operator.h
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

#ifndef DB_OPERATOR_H
#define DB_OPERATOR_H
#pragma once
#include "db_config.h"
#include "db_result.hpp"
#include "mysql/mysql.h"
#include "spdlog/spdlog.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

class DBConnection
{
  public:
    DBConnection(const DBConfig &db_config);
    ~DBConnection();

    // 禁止拷贝构造
    DBConnection(const DBConnection &) = delete;
    DBConnection &operator=(const DBConnection &) = delete;

    bool connect();
    bool isConnected() const;
    bool disconnect();

    bool execute(const std::string &sql);
    std::shared_ptr<MYSQL_RES> query(const std::string &sql);

    MYSQL *raw();

  private:
    DBConfig db_config_;
    MYSQL *mysql_ = nullptr;
};

class DBConnectionPool
{
  public:
    explicit DBConnectionPool(const DBConfig &db_config);
    ~DBConnectionPool();

    std::shared_ptr<DBConnection> getConnection();

    int getPoolSize();

  private:
    std::queue<DBConnection *> pool_;
    DBConfig db_config_;
    std::mutex pool_mutex_;
    std::condition_variable cv_;

    std::atomic<bool> shutting_down_{false};
    std::atomic<int> active_connections_{0};
    std::condition_variable cv_exit_;
};

class DBOperator
{
  public:
    ~DBOperator();

    // 单例模式
    DBOperator(const DBOperator &) = delete;
    DBOperator &operator=(const DBOperator &) = delete;

    static DBOperator &getInstance(const DBConfig &db_config);

    /**
     * @brief 执行数据库查询
     * @param sql 查询语句
     * @return true 查询成功
     * @return false 查询失败
     */
    DBResult queryDB(const std::string &sql);

    /**
     * @brief 执行数据库插入、更新、删除操作
     * @param sql 操作语句
     * @return true 操作成功
     * @return false 操作失败
     */
    bool executeDB(const std::string &sql);

  private:
    explicit DBOperator(const DBConfig &db_config);

    /**
     * @brief 初始化数据库连接池
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initDBPool();

    DBConfig db_config_;
    std::shared_ptr<DBConnectionPool> pool_;
};

#endif