#pragma once

#include <cstdint>
#include <vector>

/**
 * EncodedField: Encoded field representation (variable length).
 * Found in class_data_item sections.
 * Uses LEB128 encoding for variable-length fields.
 */
struct EncodedField {
    uint32_t fieldIdx;      // Index into field IDs (stored as delta from previous)
    uint32_t accessFlags;   // Access flags (public, private, static, final, volatile, transient, etc.)
};

/**
 * EncodedFields: Parser for encoded field lists.
 * Fields are stored as deltas to save space.
 */
class EncodedFields {
public:
    /**
     * Parse an encoded field list from a buffer.
     * @param data Pointer to the data buffer.
     * @param offset Starting offset (will be updated to point past the end).
     * @param count Number of fields to parse.
     * @return Vector of EncodedField structures with absolute indices.
     */
    static std::vector<EncodedField> parse(const uint8_t* data, uint32_t& offset, uint32_t count);
};
