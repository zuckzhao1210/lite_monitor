#ifndef ITEM_H
#define ITEM_H
#pragma once

#include <string>
#include <cstdint>
/**
create table items
(
    item_id      bigint                              not null
        primary key,
    host_id      bigint                              not null,
    item_key     varchar(128)                        not null,
    value_type   tinyint                             null comment '0=float 1=uint 2=string',
    units        varchar(32)                         null,
    interval_sec int                                 null,
    status       tinyint                             null,
    create_time  timestamp default CURRENT_TIMESTAMP not null,
    constraint uk_host_item
        unique (host_id, item_key)
);

create index idx_host_id
    on items (host_id);
 */

class Item
{
  public:
    Item() = default;
    ~Item() = default;
    
    // getters and setters
    uint64_t getItemId() const;
    void setItemId(uint64_t item_id);

    uint64_t getHostId() const;
    void setHostId(uint64_t host_id);

    std::string getItemKey() const;
    void setItemKey(const std::string &item_key);

    int8_t getValueType() const;
    void setValueType(int8_t value_type);

    std::string getUnits() const;
    void setUnits(const std::string &units);

    int getIntervalSec() const;
    void setIntervalSec(int interval_sec);

    int8_t getStatus() const;
    void setStatus(int8_t status);

    uint64_t getCreateTime() const;
    void setCreateTime(uint64_t create_time);

  private:
    uint64_t item_id;
    uint64_t host_id;
    std::string item_key;
    int8_t value_type;
    std::string units;
    int interval_sec;
    int8_t status;
    uint64_t create_time;
};

#endif