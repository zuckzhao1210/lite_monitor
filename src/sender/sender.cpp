#include "sender/sender.h"

#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <errno.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

namespace lite_monitor
{

Sender::Sender(const std::string &ip, int port, Options opt)
    : m_ip(ip),
      m_port(port),
      m_opt(opt),
      m_running(true)
{
    m_worker = std::thread(&Sender::run, this);
}

Sender::~Sender()
{
    stop();
}

void Sender::stop()
{
    m_running = false;
    m_cv.notify_all();

    if (m_worker.joinable())
        m_worker.join();

    closeSocket();
}

void Sender::enqueue(const json &j)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_queue.size() >= m_opt.maxQueueSize)
    {
        m_queue.pop_front();
        m_stats.totalDropped++;
    }

    m_queue.push_back(j);
    m_cv.notify_one();
}

Sender::StatsSnapshot Sender::getStats() const
{
    return {
        m_stats.totalSent.load(),
        m_stats.totalDropped.load(),
        m_stats.reconnectCount.load()};
}

void Sender::run()
{
    auto lastFlush = std::chrono::steady_clock::now();

    while (m_running || !m_queue.empty())
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cv.wait_for(lock,
                      std::chrono::milliseconds(m_opt.flushIntervalMs),
                      [&] { return !m_queue.empty() || !m_running; });

        if (m_queue.empty())
            continue;

        auto now = std::chrono::steady_clock::now();
        bool timeReached =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - lastFlush)
                .count() >= m_opt.flushIntervalMs;

        if (m_queue.size() >= m_opt.batchSize || timeReached || !m_running)
        {
            std::deque<json> batch;

            size_t count = std::min(m_opt.batchSize, m_queue.size());

            for (size_t i = 0; i < count; ++i)
            {
                batch.push_back(std::move(m_queue.front()));
                m_queue.pop_front();
            }

            lock.unlock();

            if (m_sockfd < 0 && !tryReconnect())
            {
                rollback(batch);
                continue;
            }

            std::string packet = buildPacket(batch);

            if (!sendAll(packet))
            {
                rollback(batch);
                closeSocket();
                tryReconnect();
            }
            else
            {
                m_stats.totalSent += batch.size();
                lastFlush = std::chrono::steady_clock::now();
                m_currentBackoff = 500;  // 成功后重置退避
            }
        }
    }
}

bool Sender::tryReconnect()
{
    if (connectSocket())
        return true;

    m_stats.reconnectCount++;
    std::this_thread::sleep_for(std::chrono::milliseconds(m_currentBackoff));
    m_currentBackoff = std::min(m_currentBackoff * 2, m_opt.maxBackoffMs);
    return false;
}

bool Sender::connectSocket()
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0)
        return false;

    int opt = 1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);

    if (::inet_pton(AF_INET, m_ip.c_str(), &addr.sin_addr) <= 0)
    {
        closeSocket();
        return false;
    }

    if (::connect(m_sockfd, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        closeSocket();
        return false;
    }

    return true;
}

std::string Sender::buildPacket(const std::deque<json> &batch)
{
    json root;
    root["type"] = "data";
    root["items"] = batch;

    std::string body = root.dump();

    uint32_t len = htonl(static_cast<uint32_t>(body.size()));

    std::string packet(sizeof(uint32_t) + body.size(), '\0');

    std::memcpy(&packet[0], &len, sizeof(uint32_t));
    std::memcpy(&packet[0] + sizeof(uint32_t), body.data(), body.size());

    return packet;
}

bool Sender::sendAll(const std::string &data)
{
    ssize_t total = 0;
    ssize_t size = data.size();

    while (total < size)
    {
        ssize_t n = ::send(m_sockfd,
                           data.data() + total,
                           size - total,
                           MSG_NOSIGNAL);

        if (n < 0)
        {
            if (errno == EINTR)
                continue;
            return false;
        }

        if (n == 0)
            return false;

        total += n;
    }

    return true;
}

void Sender::rollback(std::deque<json> &batch)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto it = batch.rbegin(); it != batch.rend(); ++it)
    {
        m_queue.push_front(std::move(*it));
    }
}

void Sender::closeSocket()
{
    if (m_sockfd >= 0)
    {
        ::close(m_sockfd);
        m_sockfd = -1;
    }
}

} // namespace lite_monitor
