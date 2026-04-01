#pragma once

#include <cstring>
#include <cstdint>
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * BinaryReader: A utility class for safely reading binary data with bounds checking.
 * Handles little-endian format and supports offset-based reading.
 */
class BinaryReader {
public:
    /**
     * Constructor: Load binary data from a file.
     * @param filePath Path to the binary file to read.
     */
    explicit BinaryReader(const std::string& filePath);

    /**
     * Constructor: Initialize from a memory buffer.
     * @param data Pointer to the data buffer.
     * @param size Size of the data buffer in bytes.
     */
    BinaryReader(const uint8_t* data, size_t size);

    // Movable, non-copyable
    BinaryReader(BinaryReader&&) = default;
    BinaryReader& operator=(BinaryReader&&) = default;
    BinaryReader(const BinaryReader&) = delete;
    BinaryReader& operator=(const BinaryReader&) = delete;

    /**
     * Read a uint8_t at the given offset.
     */
    uint8_t readU8(size_t offset) const;

    /**
     * Read a uint16_t (little-endian) at the given offset.
     */
    uint16_t readU16(size_t offset) const;

    /**
     * Read a uint32_t (little-endian) at the given offset.
     */
    uint32_t readU32(size_t offset) const;

    /**
     * Read a uint64_t (little-endian) at the given offset.
     */
    uint64_t readU64(size_t offset) const;

    /**
     * Read a signed int32_t (little-endian) at the given offset.
     */
    int32_t readS32(size_t offset) const;

    /**
     * Read raw bytes from the given offset.
     * @param offset Starting offset.
     * @param size Number of bytes to read.
     * @return Vector containing the read bytes.
     */
    std::vector<uint8_t> readBytes(size_t offset, size_t size) const;

    /**
     * Get the total size of the data buffer.
     */
    size_t getSize() const { return mSize; }

    /**
     * Get a pointer to the underlying data (for direct access if needed).
     */
    const uint8_t* getRawData() const { return mData.get(); }

private:
    std::unique_ptr<uint8_t[]> mData;
    size_t mSize;

    /**
     * Helper: Check if the requested range is within bounds.
     */
    void checkBounds(size_t offset, size_t size) const;
};
