#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * [PHASE 2] FieldIds: Field identifiers.
 * Each field ID refers to a type, class, and name.
 */
struct FieldId {
    uint16_t classIdx;        // Index into type IDs
    uint16_t typeIdx;         // Index into type IDs
    uint32_t nameIdx;         // Index into string IDs
};

class FieldIds {
public:
    /**
     * Parse field IDs section from a buffer.
     * [PHASE 2 - TO BE IMPLEMENTED]
     */
    static std::vector<FieldId> parse(const uint8_t* data, uint32_t offset, uint32_t count);
};
