#ifndef MESSAGE_C0_H
#define MESSAGE_C0_H
#pragma once

#include "message.h"

namespace lite_monitor
{
// 定义TCP消息格式
class Message_C0 : public Message
{
  public:
    Message_C0() = default;
    ~Message_C0();

    // 从数据构造消息
    Message_C0(const std::string &data)
    {
        payload_ = data;
    }

    // 拷贝构造函数
    Message_C0(const Message_C0 &other)
    {
        payload_ = other.payload_;
    }

    // 移动构造函数
    Message_C0(Message_C0 &&other) noexcept
    {
        payload_ = std::move(other.payload_);
    }

    void setTimestamp(uint64_t timestamp)
    {
        timestamp_ = timestamp;
    }
    uint64_t getTimestamp() const
    {
        return timestamp_;
    }

  private:
    void bodyDeserialize() override
    {
        memcpy(&timestamp_, body_.data(), sizeof(timestamp_));
        timestamp_ = ByteOrderUtils::netToHost64(timestamp_);
    }

    void bodySerialize() override
    {
        body_.clear();
        body_.resize(sizeof(timestamp_));
        char *ptr = &body_[0];
        uint64_t timestamp_net = ByteOrderUtils::hostToNet64(timestamp_);
        memcpy(ptr, &timestamp_net, sizeof(timestamp_net));
        setMessageLen(body_.size());
    }

  private:
    uint8_t _message_type = Message::MessageType::C0;
    uint64_t timestamp_ = 0;
};
} // namespace lite_monitor
#endif