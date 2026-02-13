#ifndef HANDLER_H
#define HANDLER_H
#pragma once

#include <string>

template <typename IN, typename OUT>
class Handler
{
  public:
    virtual ~Handler() = default;

    virtual void channel_read() = 0;
    virtual void channel_write() = 0;
};

#endif // HANDLER_H