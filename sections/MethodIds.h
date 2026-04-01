#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * [PHASE 2] MethodIds: Method identifiers.
 * Each method ID refers to a type, proto, and name.
 */
struct MethodId {
    uint16_t classIdx;        // Index into type IDs
    uint16_t protoIdx;        // Index into proto IDs
    uint32_t nameIdx;         // Index into string IDs
};

class MethodIds {
public:
    /**
     * Parse method IDs section from a buffer.
     * [PHASE 2 - TO BE IMPLEMENTED]
     */
    static std::vector<MethodId> parse(const uint8_t* data, uint32_t offset, uint32_t count);
};
