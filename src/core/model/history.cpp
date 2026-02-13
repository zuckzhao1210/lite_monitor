#include "history.h"

template <typename T> History<T>::History() = default;

template <typename T> History<T>::~History() = default;

template <typename T> uint64_t History<T>::getItemId() const
{
    return itemid;
}

template <typename T> void History<T>::setItemId(uint64_t itemid)
{
    this->itemid = itemid;
}

template <typename T> uint64_t History<T>::getClock() const
{
    return clock;
}

template <typename T> void History<T>::setClock(uint64_t clock)
{
    this->clock = clock;
}

template <typename T> T History<T>::getValue() const
{
    return value;
}

template <typename T> void History<T>::setValue(T value)
{
    this->value = value;
}

template <typename T> int32_t History<T>::getNs() const
{
    return ns;
}

template <typename T> void History<T>::setNs(int32_t ns)
{
    this->ns = ns;
}