#include "host.h"

Host::Host() = default;
Host::~Host() = default;

uint64_t Host::getHostId() const
{
    return host_id;
}
void Host::setHostId(uint64_t host_id)
{
    this->host_id = host_id;
}

std::string Host::getHostName() const
{
    return host_name;
}
void Host::setHostName(const std::string &host_name)
{
    this->host_name = host_name;
}

std::string Host::getIp() const
{
    return ip;
}
void Host::setIp(const std::string &ip)
{
    this->ip = ip;
}

int8_t Host::getStatus() const
{
    return status;
}
void Host::setStatus(int8_t status)
{
    this->status = status;
}

uint64_t Host::getCreateTime() const
{
    return create_time;
}
void Host::setCreateTime(uint64_t create_time)
{
    this->create_time = create_time;
}
