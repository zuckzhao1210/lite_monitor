#include <cstdint>
#include <string>
#include <vector>

class ChecksumUtils {
public:
    /**
     * @brief CRC16-Modbus 计算函数
     * @param data 数据指针
     * @param len 数据长度
     * @return uint16_t 校验值
     */
    static uint16_t crc16(const uint8_t* data, size_t len) {
        uint16_t crc = 0xFFFF;
        for (size_t i = 0; i < len; ++i) {
            crc ^= data[i];
            for (int j = 0; j < 8; ++j) {
                if (crc & 0x0001) {
                    crc = (crc >> 1) ^ 0xA001; // 0xA001 是多项式反转值
                } else {
                    crc >>= 1;
                }
            }
        }
        return crc;
    }

    // 针对 std::string 的重载，方便调用
    static uint16_t crc16(const std::string& data) {
        return crc16(reinterpret_cast<const uint8_t*>(data.data()), data.size());
    }

    // 针对 char* 的重载，方便调用
    static uint16_t crc16(const char* data, size_t len) {
        return crc16(reinterpret_cast<const uint8_t*>(data), len);
    }

    /**
     * @brief CRC16-Modbus 验证函数
     * @param data 数据指针
     * @param len 数据长度
     * @param expected_crc 期望的校验值
     * @return bool 是否验证通过
     */
    static bool verifyCRC(const char* data, size_t len, uint16_t expected_crc) {
        return crc16(reinterpret_cast<const uint8_t*>(data), len) == expected_crc;
    }

    // 针对 std::string 的重载，方便调用
    static bool verifyCRC(const std::string& data, uint16_t expected_crc) {
        return verifyCRC(data.data(), data.size(), expected_crc);
    }
};