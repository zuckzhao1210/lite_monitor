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
    Item() : item_id(0), host_id(0), item_key(""), value_type(0), units(""), interval_sec(0), status(0), create_time(0) {};
    ~Item();
    
    // getters and setters
    uint64_t getItemId() const { return item_id; }
    void setItemId(uint64_t item_id) { this->item_id = item_id; }

    uint64_t getHostId() const { return host_id; }
    void setHostId(uint64_t host_id) { this->host_id = host_id; }

    std::string getItemKey() const { return item_key; }
    void setItemKey(const std::string &item_key) { this->item_key = item_key; }

    int8_t getValueType() const { return value_type; }
    void setValueType(int8_t value_type) { this->value_type = value_type; }

    std::string getUnits() const { return units; }
    void setUnits(const std::string &units) { this->units = units; }

    int getIntervalSec() const { return interval_sec; }
    void setIntervalSec(int interval_sec) { this->interval_sec = interval_sec; }

    int8_t getStatus() const { return status; }
    void setStatus(int8_t status) { this->status = status; }

    uint64_t getCreateTime() const { return create_time; }
    void setCreateTime(uint64_t create_time) { this->create_time = create_time; }

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