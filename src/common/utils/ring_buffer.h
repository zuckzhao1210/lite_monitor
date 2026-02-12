#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <condition_variable>
#include <mutex>
#include <optional>
#include <vector>

template <typename T> class RingBuffer
{
  public:
    explicit RingBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), head_(0), tail_(0), size_(0), stop_(false)
    {
        if (capacity_ == 0)
        {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    }

    // 生产者：支持拷贝和移动
    bool push(T &&item)
    { // 传值配合 std::move 是最通用的做法
        std::unique_lock<std::mutex> lock(mtx_);
        not_full_cv_.wait(lock, [this]() { return size_ < capacity_ || stop_; });

        if (stop_)
            return false;

        buffer_[tail_] = std::forward<T>(item);
        tail_ = (tail_ + 1) % capacity_;
        size_++;

        lock.unlock();
        not_empty_cv_.notify_one();
        return true;
    }

    // 消费者：返回 optional 以支持非阻塞/停止状态
    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        not_empty_cv_.wait(lock, [this]() { return size_ > 0 || stop_; });

        if (stop_ && size_ == 0)
            return std::nullopt;

        T item = std::move(buffer_[head_]);
        head_ = (head_ + 1) % capacity_;
        size_--;

        lock.unlock();
        not_full_cv_.notify_one();
        return item;
    }

    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        not_empty_cv_.notify_all();
        not_full_cv_.notify_all();
    }

  private:
    std::vector<T> buffer_;
    size_t capacity_;
    size_t head_;
    size_t tail_;
    size_t size_;
    bool stop_;

    mutable std::mutex mtx_;
    std::condition_variable not_empty_cv_;
    std::condition_variable not_full_cv_;
};

#endif