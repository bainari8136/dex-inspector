#include "FieldIds.h"
#include <cstring>

std::vector<FieldId> FieldIds::parse(const uint8_t* data, uint32_t offset, uint32_t count) {
    std::vector<FieldId> result;
    result.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        FieldId field = {};
        uint32_t itemOffset = offset + i * 8;  // 8 bytes per field ID
        
        std::memcpy(&field.classIdx, data + itemOffset, 2);
        std::memcpy(&field.typeIdx, data + itemOffset + 2, 2);
        std::memcpy(&field.nameIdx, data + itemOffset + 4, 4);
        
        result.push_back(field);
    }

    return result;
}
