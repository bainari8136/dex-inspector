#include "StringIds.h"
#include <cstring>

std::vector<uint32_t> StringIds::parse(const uint8_t* data, uint32_t offset, uint32_t count) {
    std::vector<uint32_t> result;
    result.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t stringOffset = 0;
        std::memcpy(&stringOffset, data + offset + i * 4, 4);
        result.push_back(stringOffset);
    }

    return result;
}
