#ifndef QTUPGRADETOOLS_PACKAGER_H
#define QTUPGRADETOOLS_PACKAGER_H

#include <functional>
#include <string>
#include <vector>
#include "payload.h"

namespace whisper{

struct packager {
public:
    std::vector<uint8_t> pack(uint8_t channel, uint8_t flags, const payload& data);

    void unpack(const void *buffer, size_t size) ;

    std::function<void(uint8_t channel, uint8_t flags, payload& payload)> onready{};

    std::function<void(const std::string& message)> onerror{};

    uint8_t get_channel() const { return channel_; }
    uint8_t get_flags() const { return flags_; }
    uint16_t get_length() const { return length_; }

    [[nodiscard]] std::vector<uint8_t> get_payload_() const {
        return std::vector<uint8_t>(payload_.data(), payload_.data() + payload_.size());
    }

private:
    // state machine
    uint8_t state_{};

    //
    uint8_t  channel_{};
    uint8_t  flags_{};
    uint16_t length_{};
    payload  payload_{};
    uint16_t checksum_{};
};
}


#endif //QTUPGRADETOOLS_PACKAGER_H
