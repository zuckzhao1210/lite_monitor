#ifndef HOST_H
#define HOST_H
#pragma once
#include <cstdint>
#include <string>

class Host
{
  public:
    Host() : host_id(0), host_name(""), ip(""), status(0), create_time(0) {}
    ~Host() = default;

    // getter / setter
    uint64_t getHostId() const { return host_id; }
    void setHostId(uint64_t host_id) { this->host_id = host_id; }

    std::string getHostName() const { return host_name; }
    void setHostName(const std::string &host_name) { this->host_name = host_name; }

    std::string getIp() const { return ip; }
    void setIp(const std::string &ip) { this->ip = ip; }

    int8_t getStatus() const { return status; }
    void setStatus(int8_t status) { this->status = status; }

    uint64_t getCreateTime() const { return create_time; }
    void setCreateTime(uint64_t create_time) { this->create_time = create_time; }

  private:
    uint64_t host_id;
    std::string host_name;
    std::string ip;
    int8_t status;
    uint64_t create_time;
};

#endif
