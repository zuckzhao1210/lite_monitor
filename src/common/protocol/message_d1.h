#ifndef MESSAGE_D1_H
#define MESSAGE_D1_H
#pragma once

#include "message.h"
#include <string>

namespace lite_monitor
{
// 定义TCP消息格式
class Message_D1 : public Message
{
  public:
    Message_D1() = default;

    // 从数据构造消息
    Message_D1(const std::string &data)
    {
        payload_ = data;
    }

    // 拷贝构造函数
    Message_D1(const Message_D1 &other)
    {
        payload_ = other.payload_;
    }

    // 移动构造函数
    Message_D1(Message_D1 &&other) noexcept
    {
        payload_ = std::move(other.payload_);
    }

    ~Message_D1();

    void setMonitorItem(const std::string &monitor_item)
    {
        std::memcpy(_monitor_item, monitor_item.data(), 64);
    }
    void setMonitorItemType(uint8_t monitor_item_type)
    {
        _monitor_item_type = monitor_item_type;
    }
    void setMonitorItemValue(uint64_t monitor_item_value)
    {
        _monitor_item_value = monitor_item_value;
    }
    void setTimestamp(uint64_t timestamp)
    {
        _timestamp = timestamp;
    }

    std::string getMonitorItem() const
    {
        return std::string(_monitor_item, 64);
    }
    uint8_t getMonitorItemType() const
    {
        return _monitor_item_type;
    }
    uint64_t getMonitorItemValue() const
    {
        return _monitor_item_value;
    }
    uint64_t getTimestamp() const
    {
        return _timestamp;
    }

  private:
    void bodyDeserialize() override
    {
        const char *ptr = body_.data();
        std::memcpy(_monitor_item, ptr, 32);
        ptr += 32;
        std::memcpy(&_monitor_item_type, ptr, 1);
        ptr += 1;
        std::memcpy(&_monitor_item_value, ptr, 4);
        _monitor_item_value = ByteOrderUtils::netToHost32(_monitor_item_value);
        ptr += 4;
        std::memcpy(&_timestamp, ptr, 8);
        _timestamp = ByteOrderUtils::netToHost64(_timestamp);
    }

    void bodySerialize() override
    {
        char *ptr = &body_[0];
        std::memcpy(ptr, _monitor_item, 32);
        ptr += 32;
        std::memcpy(ptr, &_monitor_item_type, 1);
        ptr += 1;
        uint32_t monitor_item_value_net = ByteOrderUtils::hostToNet32(_monitor_item_value);
        std::memcpy(ptr, &monitor_item_value_net, 4);
        ptr += 4;
        uint64_t timestamp_net = ByteOrderUtils::hostToNet64(_timestamp);
        std::memcpy(ptr, &timestamp_net, 8);
        setMessageLen(body_.size());
    }

  private:
    // 消息类型 1字节
    uint8_t _message_type = Message::MessageType::D1;
    // 监控项 32字节
    char _monitor_item[32] = {0};
    // 监控项值类型 1.int 2.float 1字节
    uint8_t _monitor_item_type;
    // 监控项值 4字节
    uint32_t _monitor_item_value;
    // 时间戳 8字节
    uint64_t _timestamp;
};
} // namespace lite_monitor
#endif