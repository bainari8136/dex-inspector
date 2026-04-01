#include "ProtoIds.h"
#include <cstring>

std::vector<ProtoId> ProtoIds::parse(const uint8_t* data, uint32_t offset, uint32_t count) {
    std::vector<ProtoId> result;
    result.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        ProtoId proto = {};
        uint32_t itemOffset = offset + i * 12;  // 12 bytes per proto ID
        
        std::memcpy(&proto.shortyIdx, data + itemOffset, 4);
        std::memcpy(&proto.returnTypeIdx, data + itemOffset + 4, 4);
        std::memcpy(&proto.parametersOffset, data + itemOffset + 8, 4);
        
        result.push_back(proto);
    }

    return result;
}
