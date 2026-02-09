#ifndef MESSAGE_H
#define MESSAGE_H
#pragma once

#include <string>

namespace lite_monitor
{
// 定义TCP消息格式
class Message
{
public:
    Message() = default;
    
    // 从数据构造消息
    Message(const std::string &data) {
            
    }

    // 拷贝构造函数
    Message(const Message &other) {
        
    }

    // 移动构造函数
    Message(Message &&other) noexcept {
        data_ = std::move(other.data_);
    }

    ~Message();

    std::string getData() const {
        
    }

    /**
     * @brief 计算CRC校验值
     * @return uint16_t CRC校验值
     */
    uint16_t calculateCRC() {
        uint16_t crc = 0;
        for (int i = 0; i < _message_len; i++) {
            crc += data_[i];
        }
        return crc;
    }

private:
    char data_[1024];
    // 头标识
    static const char HEAD_IDENTIFIER = 0xAA;
    // 消息类型 1字节
    uint8_t _message_type;
    // 消息长度 4字节
    int _message_len;
    // 主机 1字节
    uint8_t _host_id;
    // 监控项 64字节
    char _monitor_item[64] = {0};
    // 监控项值类型 1.int 2.float 1字节
    uint8_t _monitor_item_type;
    // 监控项值 8字节
    uint64_t _monitor_item_value;
    // CRC校验
    uint16_t _crc = 0;
    // 尾标识
    static const char TAIL_IDENTIFIER = 0xFF;
};
}
#endif