#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <string>

class TcpServer
{
  public:
    typedef std::function<void(int, const std::string &)> MessageCallback;

    explicit TcpServer(int port) : port_(port), listen_fd_(-1), epfd_(-1)
    {
    }

    ~TcpServer()
    {
        if (listen_fd_ >= 0)
            close(listen_fd_);
        if (epfd_ >= 0)
            close(epfd_);
    }

    void setMessageCallback(MessageCallback cb)
    {
        message_cb_ = cb;
    }

    void run()
    {
        if (!init())
        {
            std::cerr << "TcpServer init failed" << std::endl;
            return;
        }

        const int MAX_EVENTS = 16;
        epoll_event events[MAX_EVENTS];

        std::cout << "TcpServer listen on port " << port_ << std::endl;

        while (true)
        {
            int n = epoll_wait(epfd_, events, MAX_EVENTS, -1);
            if (n < 0)
            {
                if (errno == EINTR)
                    continue;
                perror("epoll_wait");
                break;
            }

            for (int i = 0; i < n; ++i)
            {
                int fd = events[i].data.fd;
                if (fd == listen_fd_)
                {
                    handleAccept();
                }
                else
                {
                    handleRead(fd);
                }
            }
        }
    }

  private:
    int port_;
    int listen_fd_;
    int epfd_;

    MessageCallback message_cb_;
    std::map<int, std::string> recv_buffers_;
    std::map<int, std::string> send_buffers_;

  private:
    bool init()
    {
        listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd_ < 0)
        {
            perror("socket");
            return false;
        }

        int opt = 1;
        setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port_);

        if (bind(listen_fd_, (sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("bind");
            return false;
        }

        if (listen(listen_fd_, 16) < 0)
        {
            perror("listen");
            return false;
        }

        setNonBlocking(listen_fd_);

        epfd_ = epoll_create1(0);
        if (epfd_ < 0)
        {
            perror("epoll_create1");
            return false;
        }

        epoll_event ev;
        std::memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN;
        ev.data.fd = listen_fd_;
        epoll_ctl(epfd_, EPOLL_CTL_ADD, listen_fd_, &ev);

        return true;
    }

    static void setNonBlocking(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    void handleAccept()
    {
        while (true)
        {
            sockaddr_in cli;
            socklen_t len = sizeof(cli);
            int conn_fd = accept(listen_fd_, (sockaddr *)&cli, &len);
            if (conn_fd < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                perror("accept");
                break;
            }

            setNonBlocking(conn_fd);

            epoll_event ev;
            std::memset(&ev, 0, sizeof(ev));
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = conn_fd;
            epoll_ctl(epfd_, EPOLL_CTL_ADD, conn_fd, &ev);

            recv_buffers_[conn_fd] = std::string();
            std::cout << "New connection fd=" << conn_fd << std::endl;
        }
    }

    void handleRead(int fd)
    {
        char buf[1024];
        size_t n;
        while ((n = read(fd, buf, sizeof(buf))) > 0)
        {
            recv_buffers_[fd].append(buf, n);
        }
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return;
            closeConnection(fd);
            return;
        }
        if (n == 0)
        {
            closeConnection(fd);
            return;
        }
        processBuffer(fd);
    }

    void closeConnection(int fd)
    {
        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        recv_buffers_.erase(fd);
        std::cout << "Client closed fd=" << fd << std::endl;
    }

    void processBuffer(int fd)
    {
        std::string &buffer = recv_buffers_[fd];

        while (true)
        {
            std::size_t head = buffer.find(static_cast<char>(0xAA));
            if (head == std::string::npos)
            {
                buffer.clear();
                return;
            }

            if (head > 0)
            {
                buffer.erase(0, head);
            }

            std::size_t tail = buffer.find(static_cast<char>(0xFF), 1);
            if (tail == std::string::npos)
            {
                return;
            }

            // 提取完整帧（含 AA / FF）
            std::string frame = buffer.substr(0, tail + 1);
            buffer.erase(0, tail + 1);

            // 去掉帧头帧尾
            std::string payload = frame.substr(1, frame.size() - 2);

            // 反转义
            std::string data = unescape(payload);

            if (message_cb_)
            {
                message_cb_(fd, data);
            }
        }
    }

    static std::string unescape(const std::string &in)
    {
        std::string out;
        out.reserve(in.size());

        for (std::size_t i = 0; i < in.size(); ++i)
        {
            unsigned char c = in[i];
            if (c == 0x7D)
            {
                if (i + 1 >= in.size())
                {
                    break; // 异常转义，直接丢弃
                }
                unsigned char next = in[++i];
                if (next == 0x00)
                    out.push_back(0x7D);
                else if (next == 0x01)
                    out.push_back(0xAA);
                else if (next == 0x02)
                    out.push_back(0xFF);
            }
            else
            {
                out.push_back(c);
            }
        }
        return out;
    }
};

#endif // TCP_SERVER_H
