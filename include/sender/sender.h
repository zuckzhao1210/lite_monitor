#pragma once
#ifndef LITE_MONITOR_SENDER_H
#define LITE_MONITOR_SENDER_H

#include "../nlohmann/json.hpp"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <thread>

namespace lite_monitor
{

class Sender
{
  public:
    using json = nlohmann::json;

    struct Options
    {
        size_t batchSize;
        int flushIntervalMs;
        size_t maxQueueSize;
        int maxBackoffMs;

        // 使用构造函数初始化
        Options() : batchSize(100), flushIntervalMs(1000), maxQueueSize(20000), maxBackoffMs(8000)
        {
        }
    };

    struct StatsSnapshot
    {
        uint64_t totalSent;
        uint64_t totalDropped;
        uint64_t reconnectCount;
    };

  public:
    Sender(const std::string &ip, int port, Options opt);
    ~Sender();

    void stop();                    // 优雅关闭
    void enqueue(const json &j);    // 线程安全
    StatsSnapshot getStats() const; // 运行统计

  private:
    void run();
    bool tryReconnect();
    bool connectSocket();
    void closeSocket();

    bool sendAll(const std::string &data);
    std::string buildPacket(const std::deque<json> &batch);
    void rollback(std::deque<json> &batch);

  private:
    struct Stats
    {
        std::atomic<uint64_t> totalSent{0};
        std::atomic<uint64_t> totalDropped{0};
        std::atomic<uint64_t> reconnectCount{0};
    };

  private:
    std::string m_ip;
    int m_port;
    int m_sockfd{-1};

    Options m_opt;

    std::deque<json> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    std::thread m_worker;
    std::atomic<bool> m_running{false};

    int m_currentBackoff{500};

    Stats m_stats;
};

} // namespace lite_monitor

#endif
