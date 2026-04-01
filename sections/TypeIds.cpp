#include "TypeIds.h"
#include <cstring>

std::vector<uint32_t> TypeIds::parse(const uint8_t* data, uint32_t offset, uint32_t count) {
    std::vector<uint32_t> result;
    result.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t typeIndex = 0;
        std::memcpy(&typeIndex, data + offset + i * 4, 4);
        result.push_back(typeIndex);
    }

    return result;
}
