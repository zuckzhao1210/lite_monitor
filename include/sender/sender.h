#ifndef SENDER_H
#define SENDER_H
#pragma once
#include <string>
#include <sys/socket.h>

namespace lite_monitor
{
class Sender
{
public:
    Sender(const std::string &ip, int port);
    ~Sender();
    
    bool send(const std::string &data);
    bool heartbeat();
    bool connect();
    bool isConnected() const;

private:
    std::string m_ip;
    int m_port;
    int sockfd_;
};
}
#endif
