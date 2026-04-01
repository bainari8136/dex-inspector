#include "EncodedField.h"
#include "../Leb128.h"

std::vector<EncodedField> EncodedFields::parse(const uint8_t* data, uint32_t& offset, uint32_t count) {
    std::vector<EncodedField> fields;
    fields.reserve(count);

    uint32_t currentFieldIdx = 0;

    for (uint32_t i = 0; i < count; ++i) {
        size_t bytesRead = 0;
        
        // Field index delta (LEB128)
        uint32_t fieldIdxDelta = Leb128::decodeULEB128(data, offset, bytesRead);
        offset += bytesRead;
        
        // Access flags (LEB128)
        uint32_t accessFlags = Leb128::decodeULEB128(data, offset, bytesRead);
        offset += bytesRead;

        // Convert delta to absolute index
        currentFieldIdx += fieldIdxDelta;

        EncodedField field{};
        field.fieldIdx = currentFieldIdx;
        field.accessFlags = accessFlags;

        fields.push_back(field);
    }

    return fields;
}
