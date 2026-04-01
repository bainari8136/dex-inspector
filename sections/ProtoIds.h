#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * [PHASE 2] ProtoIds: Method prototype identifiers.
 * Each proto ID references a set of parameters and a return type.
 */
struct ProtoId {
    uint32_t shortyIdx;          // Index into string IDs for short-form signature
    uint32_t returnTypeIdx;      // Index into type IDs for return type
    uint32_t parametersOffset;   // Offset into data section for parameter list
};

class ProtoIds {
public:
    /**
     * Parse proto IDs section from a buffer.
     * [PHASE 2 - TO BE IMPLEMENTED]
     */
    static std::vector<ProtoId> parse(const uint8_t* data, uint32_t offset, uint32_t count);
};
