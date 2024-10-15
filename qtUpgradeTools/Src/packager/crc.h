#ifndef QTUPGRADETOOLS_CRC_H
#define QTUPGRADETOOLS_CRC_H

#include <cstddef>
#include <cstdint>

namespace whisper
{
    uint16_t crc_16(const uint8_t *data, size_t length);

    uint16_t crc_modbus(const uint8_t *data, size_t length);
} // namespace whisper


#endif //QTUPGRADETOOLS_CRC_H
