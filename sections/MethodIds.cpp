#include "MethodIds.h"
#include <cstring>

std::vector<MethodId> MethodIds::parse(const uint8_t* data, uint32_t offset, uint32_t count) {
    std::vector<MethodId> result;
    result.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        MethodId method = {};
        uint32_t itemOffset = offset + i * 8;  // 8 bytes per method ID
        
        std::memcpy(&method.classIdx, data + itemOffset, 2);
        std::memcpy(&method.protoIdx, data + itemOffset + 2, 2);
        std::memcpy(&method.nameIdx, data + itemOffset + 4, 4);
        
        result.push_back(method);
    }

    return result;
}
