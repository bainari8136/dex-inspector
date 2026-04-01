#pragma once

#include <cstdint>
#include <vector>

/**
 * Leb128: Utilities for decoding ULEB128 (unsigned) and SLEB128 (signed) variable-length integers.
 * These are used extensively in the DEX format for compact representation of integers.
 */
class Leb128 {
public:
    /**
     * Decode an unsigned LEB128 integer starting at offset.
     * @param data Pointer to the data buffer.
     * @param offset Starting offset for reading.
     * @param outBytesRead [out] Number of bytes consumed.
     * @return The decoded unsigned 32-bit integer.
     */
    static uint32_t decodeULEB128(const uint8_t* data, size_t offset, size_t& outBytesRead);

    /**
     * Decode a signed LEB128 integer starting at offset.
     * @param data Pointer to the data buffer.
     * @param offset Starting offset for reading.
     * @param outBytesRead [out] Number of bytes consumed.
     * @return The decoded signed 32-bit integer.
     */
    static int32_t decodeSLEB128(const uint8_t* data, size_t offset, size_t& outBytesRead);

    /**
     * Decode an unsigned LEB128 integer from the provided byte vector.
     * This overload works with a vector and an iterator to the current position.
     * @param data Reference to the data vector.
     * @param offset Starting offset for reading.
     * @param outBytesRead [out] Number of bytes consumed.
     * @return The decoded unsigned 32-bit integer.
     */
    static uint32_t decodeULEB128(const std::vector<uint8_t>& data, size_t offset, size_t& outBytesRead);

    /**
     * Decode a signed LEB128 integer from the provided byte vector.
     * @param data Reference to the data vector.
     * @param offset Starting offset for reading.
     * @param outBytesRead [out] Number of bytes consumed.
     * @return The decoded signed 32-bit integer.
     */
    static int32_t decodeSLEB128(const std::vector<uint8_t>& data, size_t offset, size_t& outBytesRead);
};
