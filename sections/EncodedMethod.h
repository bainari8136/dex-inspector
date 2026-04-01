#pragma once

#include <cstdint>
#include <vector>

/**
 * EncodedMethod: Encoded method representation (variable length).
 * Found in class_data_item sections.
 * Uses LEB128 encoding for variable-length fields.
 */
struct EncodedMethod {
    uint32_t methodIdx;     // Index into method IDs (stored as delta from previous)
    uint32_t accessFlags;   // Access flags (public, private, static, final, abstract, native, synchronized, etc.)
    uint32_t codeOff;       // Offset to code_item in data section (0 if abstract or native)
};

/**
 * EncodedMethods: Parser for encoded method lists.
 * Methods are stored as deltas to save space.
 */
class EncodedMethods {
public:
    /**
     * Parse an encoded method list from a buffer.
     * @param data Pointer to the data buffer.
     * @param offset Starting offset (will be updated to point past the end).
     * @param count Number of methods to parse.
     * @return Vector of EncodedMethod structures with absolute indices.
     */
    static std::vector<EncodedMethod> parse(const uint8_t* data, uint32_t& offset, uint32_t count);
};
