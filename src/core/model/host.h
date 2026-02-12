#ifndef HOST_H
#define HOST_H
#pragma once
#include <string>
#include <cstdint>

class Host
{
  public:
    Host() = default;
    ~Host() = default;

    // getter / setter
    uint64_t getHostId() const;
    void setHostId(uint64_t host_id);

    std::string getHostName() const;
    void setHostName(const std::string &host_name);

    std::string getIp() const;
    void setIp(const std::string &ip);

    int8_t getStatus() const;
    void setStatus(int8_t status);

    uint64_t getCreateTime() const;
    void setCreateTime(uint64_t create_time);

  private:
    uint64_t host_id;
    std::string host_name;
    std::string ip;
    int8_t status;
    uint64_t create_time;
};

#endif
