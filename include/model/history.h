#ifndef HISTORY_H
#define HISTORY_H

#include <cstdint>

template <typename T>
class History {
public:
    History() : itemid(0), clock(0), value(T()), ns(0) {}
    ~History() = default;

    uint64_t getItemId() const { return itemid; }
    void setItemId(uint64_t id) { itemid = id; }

    uint64_t getClock() const { return clock; }
    void setClock(uint64_t c) { clock = c; }

    T getValue() const { return value; }
    void setValue(T v) { value = v; }

    int32_t getNs() const { return ns; }
    void setNs(int32_t n) { ns = n; }

private:
    uint64_t itemid;
    uint64_t clock;
    T value;
    int32_t ns;
};

#endif