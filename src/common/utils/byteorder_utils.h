#ifndef BYTEORDER_UTILS_H
#define BYTEORDER_UTILS_H
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <arpa/inet.h>
#include <endian.h> // Linux 下处理 64 位转换的核心库

namespace lite_monitor
{
// 字节序转换工具
class ByteOrderUtils
{
public:
    // --- 1. 字节序转换 (支持 16, 32, 64 位) ---
    static uint16_t hostToNet16(uint16_t val) { return htons(val); }
    static uint32_t hostToNet32(uint32_t val) { return htonl(val); }
    static uint64_t hostToNet64(uint64_t val) { return htobe64(val); }

    static uint16_t netToHost16(uint16_t val) { return ntohs(val); }
    static uint32_t netToHost32(uint32_t val) { return ntohl(val); }
    static uint64_t netToHost64(uint64_t val) { return be64toh(val); }
};
} // namespace lite_monitor
#endif