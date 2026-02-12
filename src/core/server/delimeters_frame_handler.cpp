#include "delimeters_frame_handler.h"

DelimitersFrameHandler::DelimitersFrameHandler(std::initializer_list<std::string> delimiters, bool discard_long_message,
                                               uint16_t max_message_length)
    : FrameHandler(discard_long_message, max_message_length)
{
    if (delimiters.size() != 2)
    {
        throw std::invalid_argument("delimiters size must be 2");
    }
    for (auto &delim : delimiters)
    {
        delimiters_.push_back(delim);
    }
}

DelimitersFrameHandler::~DelimitersFrameHandler()
{
}

std::string DelimitersFrameHandler::handle_frame(const std::string &frame)
{
    // 追加到缓冲区
    buffer.append(frame.c_str());

    std::size_t head = buffer.find(static_cast<char>(0xAA));
    if (head == std::string::npos)
    {
        buffer.clear();
        return;
    }

    if (head > 0)
    {
        buffer.erase(0, head);
    }

    std::size_t tail = buffer.find(static_cast<char>(0xFF), 1);
    if (tail == std::string::npos)
    {
        return;
    }

    // 提取完整帧（含 AA / FF）
    std::string frame = buffer.substr(0, tail + 1);
    buffer.erase(0, tail + 1);

    // 去掉帧头帧尾
    std::string payload = frame.substr(1, frame.size() - 2);

    // 反转义
    std::string data = unescape(payload);

    return data;
}

std::string DelimitersFrameHandler::unescape(std::string &in)
{
    std::string out;
    out.reserve(in.size());

    for (std::size_t i = 0; i < in.size(); ++i)
    {
        unsigned char c = in[i];
        if (c == 0x7D)
        {
            if (i + 1 >= in.size())
            {
                break; // 异常转义，直接丢弃
            }
            unsigned char next = in[++i];
            if (next == 0x00)
                out.push_back(0x7D);
            else if (next == 0x01)
                out.push_back(0xAA);
            else if (next == 0x02)
                out.push_back(0xFF);
        }
        else
        {
            out.push_back(c);
        }
    }
    return out;
}