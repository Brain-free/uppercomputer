#include "packager.h"
#include "crc.h"
#include "qDebug.h"

namespace whisper {

    std::vector<uint8_t> packager::pack(uint8_t channel, uint8_t flags, const payload &payload) {
        auto length = static_cast<uint16_t>(payload.size());

        std::vector<uint8_t> buffer(length + 10, 0);

        size_t widx = 0;

        // head
        std::memcpy(buffer.data() + widx, "<<", 2);
        widx += 2;

        // channel
        std::memcpy(buffer.data() + widx, &channel, 1);
        widx += 1;

        // flags
        std::memcpy(buffer.data() + widx, &flags, 1);
        widx += 1;

        // length
        std::memcpy(buffer.data() + widx, &length, 2);
        widx += 2;

        // payload
        std::memcpy(buffer.data() + widx, payload.data(), length);
        widx += length;

        // checksum
        uint16_t checksum = crc_16(payload.data(), payload.size());
        std::memcpy(buffer.data() + widx, &checksum, 2);
        widx += 2;

        // tail
        std::memcpy(buffer.data() + widx, ">>", 2);
        return buffer;
    }

    constexpr int S_HEAD_L = 0;
    constexpr int S_HEAD_H = S_HEAD_L + 1;
    constexpr int S_CHANNEL = S_HEAD_H + 1;
    constexpr int S_FLAGS = S_CHANNEL + 1;
    constexpr int S_LENGTH_L = S_FLAGS + 1;
    constexpr int S_LENGTH_H = S_LENGTH_L + 1;
    constexpr int S_PAYLOAD = S_LENGTH_H + 1;
    constexpr int S_CHECKSUM_L = S_PAYLOAD + 1;
    constexpr int S_CHECKSUM_H = S_CHECKSUM_L + 1;
    constexpr int S_TAIL_L = S_CHECKSUM_H + 1;
    constexpr int S_TAIL_H = S_TAIL_L + 1;

    void packager::unpack(const void *buffer, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            uint8_t ch = reinterpret_cast<const uint8_t *>(buffer)[i];
            switch (state_) {
                case S_HEAD_L:
                    state_ = (ch == '<') ? S_HEAD_H : S_HEAD_L;
                    break;
                case S_HEAD_H:
                    state_ = (ch == '<') ? S_CHANNEL : S_HEAD_L;
                    break;

                case S_CHANNEL:
                    channel_ = ch;
                    state_ = S_FLAGS;
                    break;

                case S_FLAGS:
                    flags_ = ch;
                    state_ = S_LENGTH_L;
                    break;

                case S_LENGTH_L:
                    length_ = ch;
                    payload_.clear();
                    state_ = S_LENGTH_H;
                    break;
                case S_LENGTH_H:
                    length_ = (ch << 8) | length_;
                    state_ = S_PAYLOAD;
                    break;

                case S_PAYLOAD:
                    if (payload_.size() < length_) payload_.write(ch);

                    if (payload_.size() == length_) state_ = S_CHECKSUM_L;
                    break;

                case S_CHECKSUM_L:
                    checksum_ = ch;
                    state_ = S_CHECKSUM_H;
                    break;
                case S_CHECKSUM_H:
                    checksum_ = (ch << 8) | checksum_;
                    state_ = S_TAIL_L;
                    break;

                case S_TAIL_L:
                    state_ = (ch == '>') ? S_TAIL_H : S_HEAD_L;
                    break;

                case S_TAIL_H:
                    state_ = S_HEAD_L;

//                    if (ch == '>' && (checksum_ == crc_16(payload_.data(), payload_.size()))) {
////                        onready(channel_, flags_, payload_);
//                        break;
//                    }
                    if (ch == '>' ) {
//                        onready(channel_, flags_, payload_);
                        break;
                    }
                    break;
                default:
                    break;
            }
            // clang-format on
        }
    }
} // namespace whisper