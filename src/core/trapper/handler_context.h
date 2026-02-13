#ifndef HANDLER_CONTEXT_H
#define HANDLER_CONTEXT_H
#pragma once

#include <string>
#include <functional>

template<typename Msg>
class HandlerContext {
public:
    using NextFunc = std::function<void(Msg&&)>;

    HandlerContext(NextFunc next)
        : next_(std::move(next)) {}

    void fireRead(Msg&& msg) {
        if (next_)
            next_(std::forward<Msg>(msg));
    }

private:
    NextFunc next_;
};

#endif // HANDLER_CONTEXT_H
