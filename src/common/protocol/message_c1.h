#ifndef MESSAGE_C1_H
#define MESSAGE_C1_H
#pragma once

#include "message.h"
#include <string>

namespace lite_monitor
{
// 定义TCP消息格式
class Message_C1 : public Message
{
  public:
    Message_C1() = default;
    ~Message_C1();

    // 从数据构造消息
    Message_C1(const std::string &data)
    {
        payload_ = data;
    }

    // 拷贝构造函数
    Message_C1(const Message_C1 &other)
    {
        payload_ = other.payload_;
    }

    // 移动构造函数
    Message_C1(Message_C1 &&other) noexcept
    {
        payload_ = std::move(other.payload_);
    }

  private:
    void bodyDeserialize() override
    {
        const char *ptr = body_.data();
        std::memcpy(_monitor_item, ptr, 64);
        ptr += 64;
        std::memcpy(&_monitor_item_type, ptr, 1);
        ptr += 1;
        std::memcpy(&_timestamp, ptr, 8);
        _timestamp = ByteOrderUtils::netToHost64(_timestamp);
    }

    void bodySerialize() override
    {
        char *ptr = &body_[0];
        std::memcpy(ptr, _monitor_item, 64);
        ptr += 64;
        std::memcpy(ptr, &_monitor_item_type, 1);
        setMessageLen(body_.size());
        ptr += 1;
        uint64_t timestamp_net = ByteOrderUtils::hostToNet64(_timestamp);
        std::memcpy(ptr, &timestamp_net, 8);
        ptr += 8;
    }

  private:
    uint8_t _message_type = Message::MessageType::C1;
    // 监控项 32字节
    char _monitor_item[32] = {0};
    // 监控项值类型 1.int 2.float 1字节
    uint8_t _monitor_item_type;
    // 时间戳
    uint64_t _timestamp;
};
} // namespace lite_monitor
#endif