#ifndef FRAME_HANDLER_H
#define FRAME_HANDLER_H

#pragma once
#include <string>

class FrameHandler
{
  public:
    FrameHandler(bool discard_long_message, uint16_t max_message_length)
        : discard_long_message(discard_long_message), max_message_length(max_message_length)
    {
    }

    virtual ~FrameHandler() = default;

    virtual std::string handle_frame(const std::string &frame) = 0;

  protected:
    // 是否超长抛弃报文
    bool discard_long_message;
    // 最大缓冲报文长度，如果超过这个长度，会根据discard_long_message判断是否抛弃
    uint16_t max_message_length;
    // 缓冲区
    std::string buffer;
};

#endif // FRAME_HANDLER_H
