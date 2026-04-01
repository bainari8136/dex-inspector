#include "Leb128.h"
#include <stdexcept>

uint32_t Leb128::decodeULEB128(const uint8_t* data, size_t offset, size_t& outBytesRead) {
    uint32_t result = 0;
    int shift = 0;
    size_t bytesRead = 0;

    while (true) {
        uint8_t byte = data[offset + bytesRead];
        bytesRead++;

        // Add the lower 7 bits to result
        result |= (static_cast<uint32_t>(byte & 0x7F) << shift);

        // If the high bit is not set, this is the last byte
        if ((byte & 0x80) == 0) {
            break;
        }

        shift += 7;

        // Prevent overflow (ULEB128 should not exceed 5 bytes for 32-bit values)
        if (shift >= 32) {
            throw std::runtime_error("ULEB128 overflow");
        }
    }

    outBytesRead = bytesRead;
    return result;
}

int32_t Leb128::decodeSLEB128(const uint8_t* data, size_t offset, size_t& outBytesRead) {
    int32_t result = 0;
    int shift = 0;
    size_t bytesRead = 0;
    uint8_t byte = 0;

    while (true) {
        byte = data[offset + bytesRead];
        bytesRead++;

        // Add the lower 7 bits to result
        result |= (static_cast<int32_t>(byte & 0x7F) << shift);
        shift += 7;

        // If the high bit is not set, this is the last byte
        if ((byte & 0x80) == 0) {
            break;
        }

        // Prevent overflow (SLEB128 should not exceed 5 bytes for 32-bit values)
        if (shift >= 32) {
            throw std::runtime_error("SLEB128 overflow");
        }
    }

    // Sign extend if necessary
    if (shift < 32 && (byte & 0x40)) {
        result |= (static_cast<int32_t>(-1) << shift);
    }

    outBytesRead = bytesRead;
    return result;
}

uint32_t Leb128::decodeULEB128(const std::vector<uint8_t>& data, size_t offset, size_t& outBytesRead) {
    return decodeULEB128(data.data(), offset, outBytesRead);
}

int32_t Leb128::decodeSLEB128(const std::vector<uint8_t>& data, size_t offset, size_t& outBytesRead) {
    return decodeSLEB128(data.data(), offset, outBytesRead);
}
