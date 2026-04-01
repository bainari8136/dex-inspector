#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * StringIds: Represents the string identifiers section of a DEX file.
 * Each entry in this section is a 32-bit offset into the data section.
 */
class StringIds {
public:
    /**
     * Parse string IDs section from a buffer.
     * @param data Pointer to the data buffer.
     * @param offset Starting offset of the string IDs section.
     * @param count Number of string IDs to parse.
     * @return Vector of 32-bit offsets into the data section.
     */
    static std::vector<uint32_t> parse(const uint8_t* data, uint32_t offset, uint32_t count);
};
