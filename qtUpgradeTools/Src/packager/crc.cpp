#include "crc.h"

namespace whisper
{
    uint16_t crc_16(const uint8_t *data, size_t length)
    {
        uint16_t crc = 0;

        while (length--) {
            crc ^= *data++;

            for (int i = 0; i < 8; ++i) {
                crc = (crc & 0x0001) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
            }
        }

        return crc;
    }
} // namespace whisper
