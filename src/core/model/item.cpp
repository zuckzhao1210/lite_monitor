#include "item.h"

Item::Item() = default;
Item::~Item() = default;

uint64_t Item::getItemId() const
{
    return item_id;
}
void Item::setItemId(uint64_t item_id)
{
    this->item_id = item_id;
}

uint64_t Item::getHostId() const
{
    return host_id;
}
void Item::setHostId(uint64_t host_id)
{
    this->host_id = host_id;
}

std::string Item::getItemKey() const
{
    return item_key;
}
void Item::setItemKey(const std::string &item_key)
{
    this->item_key = item_key;
}

int8_t Item::getValueType() const
{
    return value_type;
}
void Item::setValueType(int8_t value_type)
{
    this->value_type = value_type;
}

std::string Item::getUnits() const
{
    return units;
}
void Item::setUnits(const std::string &units)
{
    this->units = units;
}

int Item::getIntervalSec() const
{
    return interval_sec;
}
void Item::setIntervalSec(int interval_sec)
{
    this->interval_sec = interval_sec;
}

int8_t Item::getStatus() const
{
    return status;
}
void Item::setStatus(int8_t status)
{
    this->status = status;
}

uint64_t Item::getCreateTime() const
{
    return create_time;
}
void Item::setCreateTime(uint64_t create_time)
{
    this->create_time = create_time;
}
