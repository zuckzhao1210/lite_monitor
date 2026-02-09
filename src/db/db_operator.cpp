#include "db_operator.h"

DBConnection::DBConnection(const DBConfig &db_config) : db_config_(db_config)
{
}

DBConnection::~DBConnection()
{
    disconnect();
}

bool DBConnection::connect()
{
    if (isConnected())
    {
        return true;
    }
    mysql_ = mysql_init(nullptr);
    if (!mysql_)
    {
        return false;
    }
    if (!mysql_real_connect(mysql_, db_config_.host.c_str(), db_config_.user.c_str(), db_config_.password.c_str(),
                            db_config_.database.c_str(), db_config_.port, nullptr, 0))
    {
        mysql_close(mysql_);
        mysql_ = nullptr;
        return false;
    }
    return true;
}

bool DBConnection::isConnected() const
{
    return mysql_ && mysql_ping(mysql_) == 0;
}

bool DBConnection::disconnect()
{
    if (mysql_)
    {
        mysql_close(mysql_);
        mysql_ = nullptr;
    }
    return true;
}

bool DBConnection::execute(const std::string &sql)
{
    if (!isConnected())
    {
        return false;
    }
    return mysql_query(mysql_, sql.c_str()) == 0;
}

std::shared_ptr<MYSQL_RES> DBConnection::query(const std::string &sql)
{
    if (!isConnected())
    {
        return nullptr;
    }
    if (mysql_query(mysql_, sql.c_str()) != 0)
    {
        return nullptr;
    }
    auto res = mysql_store_result(mysql_);
    auto res_ptr = std::shared_ptr<MYSQL_RES>(res, mysql_free_result);
    return res_ptr;
}

MYSQL *DBConnection::raw()
{
    return mysql_;
}

DBConnectionPool::DBConnectionPool(const DBConfig &db_config) : db_config_(db_config)
{
    // 初始化连接池
    for (int i = 0; i < db_config_.pool_size; ++i)
    {
        auto conn = new DBConnection(db_config_);
        conn->connect();
        if (conn->isConnected())
        {
            pool_.push(conn);
        }
        else
        {
            delete conn;
        }
    }
}

DBConnectionPool::~DBConnectionPool()
{
    {
    std::unique_lock<std::mutex> lock(pool_mutex_);
    shutting_down_ = true;
    }
    cv_.notify_all();

    std::unique_lock<std::mutex> lock_exit(pool_mutex_);
    cv_exit_.wait(lock_exit, [this]() { return active_connections_ == 0; });

    // 关闭所有连接
    while (!pool_.empty())
    {
        auto conn = pool_.front();
        pool_.pop();
        conn->disconnect();
        delete conn;
    }
}

std::shared_ptr<DBConnection> DBConnectionPool::getConnection()
{
    std::chrono::milliseconds timeout = std::chrono::milliseconds(db_config_.pool_timeout_ms);
    std::unique_lock<std::mutex> lock(pool_mutex_);
    if (shutting_down_)
    {
        spdlog::warn("[DBConnectionPool::getConnection] pool is shutting down");
        return nullptr;
    }

    bool success = cv_.wait_for(lock, timeout, [this]() { return !pool_.empty(); });
    if (!success)
    {
        throw std::runtime_error("[DBConnectionPool::getConnection] wait for pool timeout");
        return nullptr;
    }

    if (shutting_down_ || pool_.empty())
    {
        spdlog::warn("[DBConnectionPool::getConnection] pool is empty or pool is shutting down");
        return nullptr;
    }

    auto conn = pool_.front();
    pool_.pop();
    active_connections_++;
    return std::shared_ptr<DBConnection>(conn, [this](DBConnection *conn) {
        std::unique_lock<std::mutex> lock(pool_mutex_);
        if (!conn->isConnected())
        {
            conn->disconnect();
            delete conn;
            conn = new DBConnection(db_config_);
            conn->connect();
        }
        pool_.push(conn);
        active_connections_--;
        lock.unlock();
        cv_.notify_one();
        if (shutting_down_)
        {
            cv_exit_.notify_one();
        }
    });
}

int DBConnectionPool::getPoolSize()
{
    std::unique_lock<std::mutex> lock(pool_mutex_);
    return static_cast<int>(pool_.size());
}

DBOperator::DBOperator(const DBConfig &db_config) : db_config_(db_config)
{
    if (!initDBPool())
    {
        throw std::runtime_error("[DBOperator::DBOperator] initDBPool failed");
    }
}

DBOperator::~DBOperator()
{
    mysql_library_end();
}

DBOperator &DBOperator::getInstance(const DBConfig &db_config)
{
    static DBOperator instance(db_config);
    return instance;
}

bool DBOperator::initDBPool()
{
    if (mysql_library_init(0, nullptr, nullptr))
    {
        spdlog::error("[DBOperator::initDBPool] mysql_library_init failed");
        return false;
    }
    pool_ = std::make_shared<DBConnectionPool>(db_config_);
    return pool_->getPoolSize() > 0;
}

DBResult DBOperator::queryDB(const std::string &sql)
{
    try
    {
        auto conn = pool_->getConnection();
        if (!conn)
        {
            return DBResult();
        }
        auto res = conn->query(sql);
        return DBResult(res);
    }
    catch (const std::exception &e)
    {
        spdlog::error("[DBOperator::queryDB] exec sql: {} \n error: {}", sql, e.what());
        return DBResult();
    }
}

bool DBOperator::executeDB(const std::string &sql)
{
    if (!pool_)
    {
        spdlog::error("[DBOperator::executeDB] pool is not initialized");
        return false;
    }
    try
    {
        auto conn = pool_->getConnection();
        if (!conn)
        {
            return false;
        }
        return conn->execute(sql);
    }
    catch (const std::exception &e)
    {
        spdlog::error("[DBOperator::executeDB] exec sql: {} \n error: {}", sql, e.what());
        return false;
    }
}
