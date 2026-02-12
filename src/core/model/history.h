#ifndef HISTORY_H
#define HISTORY_H
#pragma once

#include <cstdint>
/*
 -- auto-generated definition
create table history_uint
(
    itemid bigint unsigned           not null,
    clock  int             default 0 not null,
    value  bigint unsigned default 0 not null,
    ns     int             default 0 not null,
    primary key (itemid, clock, ns)
)
    partition by range (clock);
 */
template <typename T>
class History
{
  public:
    History() = default;
    ~History() = default;

    // getters and setters
    uint64_t getItemId() const;
    void setItemId(uint64_t itemid);

    uint64_t getClock() const;
    void setClock(uint64_t clock);

    T getValue() const;
    void setValue(T value);

    int32_t getNs() const;
    void setNs(int32_t ns);

  private:
    uint64_t itemid;
    uint64_t clock;
    T value;
    int32_t ns;
};
#endif // HISTORY_H