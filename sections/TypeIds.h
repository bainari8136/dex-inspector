#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * TypeIds: Represents the type identifiers section of a DEX file.
 * Each entry in this section is a 32-bit index into the string IDs section.
 */
class TypeIds {
public:
    /**
     * Parse type IDs section from a buffer.
     * @param data Pointer to the data buffer.
     * @param offset Starting offset of the type IDs section.
     * @param count Number of type IDs to parse.
     * @return Vector of 32-bit string indices.
     */
    static std::vector<uint32_t> parse(const uint8_t* data, uint32_t offset, uint32_t count);
};
