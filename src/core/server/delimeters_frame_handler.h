#ifndef DELIMITERS_FRAME_HANDLER_H
#define DELIMITERS_FRAME_HANDLER_H

#pragma once
#include "frame_handler.h"
#include <array>
#include <vector>
#include <stdexcept>
#include <string>

class DelimitersFrameHandler : public FrameHandler
{
  public:
    DelimitersFrameHandler(std::initializer_list<std::string> delimiters, bool discard_long_message,
                           uint16_t max_message_length);
    ~DelimitersFrameHandler();

    /**
     * @brief 处理帧
     *
     * @param frame 帧数据
     */
    std::string handle_frame(const std::string &frame) override;

    // 反转义
    std::string unescape(std::string &payload);

  private:
    // 分隔符
    std::vector<std::string> delimiters_;
};

#endif // DELIMITERS_FRAME_HANDLER_H