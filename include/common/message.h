#ifndef MESSAGE_H
#define MESSAGE_H
#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <string>

#include "utils/byteorder_utils.h"
#include "utils/checksum_utils.h"

namespace lite_monitor
{
// 定义TCP消息格式
class Message
{
  public:
    enum MessageType : uint8_t
    {
        C0 = 0xc0,
        C1 = 0xc1,
        D1 = 0xd1
    };

    Message() = default;

    // 从数据构造消息
    Message(const std::string &data)
    {
        payload_ = data;
    }

    // 拷贝构造函数
    Message(const Message &other)
    {
        payload_ = other.payload_;
    }

    // 移动构造函数
    Message(Message &&other) noexcept
    {
        payload_ = std::move(other.payload_);
    }

    ~Message();

    // setter / getter
    void setPayload(const std::string &data)
    {
        payload_ = data;
    }
    std::string getPayload() const
    {
        return payload_;
    }

    void setHostID(uint8_t host_id)
    {
        _host_id = host_id;
    }
    uint8_t getHostID() const
    {
        return _host_id;
    }
    void setMessageType(uint8_t message_type)
    {
        _message_type = message_type;
    }
    uint8_t getMessageType() const
    {
        return _message_type;
    }
    void setMessageLen(uint32_t message_len)
    {
        _message_len = message_len;
    }
    uint32_t getMessageLen() const
    {
        return _message_len;
    }

    /**
     * @brief 序列化数据
     */
    std::string serialize()
    {
        bodySerialize();

        payload_.clear();
        payload_.resize(HEADER_LENGTH + _message_len);

        char *ptr = &payload_[0];

        *ptr = static_cast<char>(_message_type);
        ptr += 1;

        uint16_t net_len = ByteOrderUtils::hostToNet16(_message_len);
        std::memcpy(ptr, &net_len, 2);
        ptr += 2;

        *ptr = static_cast<char>(_host_id);
        ptr += 1;

        // 消息体
        std::memcpy(ptr, body_.data(), _message_len);
        ptr += _message_len;

        // 计算CRC校验值，并且序列化
        uint16_t crc = ChecksumUtils::crc16(payload_.data(), HEADER_LENGTH + _message_len);
        uint16_t net_crc = ByteOrderUtils::hostToNet16(crc);
        std::memcpy(ptr, &net_crc, 2);
        return payload_;
    }

    /**
     * @brief 反序列化数据
     * @param data 包含消息数据的字符串
     */
    void deserialize()
    {
        if (payload_.size() < HEADER_LENGTH)
        {
            throw std::runtime_error("Invalid message data");
        }

        const char *ptr = payload_.data();
        _message_type = static_cast<uint8_t>(ptr[0]);
        ptr += 1;

        uint16_t net_len;
        std::memcpy(&net_len, ptr, 2);
        _message_len = ByteOrderUtils::netToHost16(net_len);
        ptr += 2;

        _host_id = static_cast<uint8_t>(ptr[0]);
        ptr += 1;

        body_.clear();
        body_.reserve(_message_len);
        body_.assign(ptr, _message_len);
        ptr += _message_len;
        if (_message_len != body_.size())
        {
            throw std::runtime_error("Invalid message data");
        }
        bodyDeserialize();

        // 验证 CRC
        uint16_t expected_crc;
        std::memcpy(&expected_crc, ptr, 2);
        expected_crc = ByteOrderUtils::netToHost16(expected_crc);
        if (!ChecksumUtils::verifyCRC(payload_.data(), HEADER_LENGTH + _message_len, expected_crc))
        {
            throw std::runtime_error("CRC verification failed");
        }
        else
        {
            _crc = expected_crc;
        }
    }

  protected:
    // 子类实现具体的消息体序列化逻辑
    virtual void bodySerialize() = 0;
    // 子类实现具体的消息体反序列化逻辑
    virtual void bodyDeserialize() = 0;

  public:
    // 除了消息体之外的其他数据长度
    static const size_t HEADER_LENGTH = 38;

  protected:
    // 消息类型 1字节 0xc1表示询问监控项，0xd1表示上报监控项
    uint8_t _message_type;
    // 消息长度 4字节
    uint16_t _message_len;
    // 主机 1字节
    uint8_t _host_id;

    // CRC校验 2字节
    uint16_t _crc = 0;

    std::string body_;
    std::string payload_;
};
} // namespace lite_monitor
#endif