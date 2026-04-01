#include "BinaryReader.h"
#include <fstream>
#include <sstream>

BinaryReader::BinaryReader(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    mSize = file.tellg();
    if (mSize == 0) {
        throw std::runtime_error("File is empty: " + filePath);
    }

    mData = std::make_unique<uint8_t[]>(mSize);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(mData.get()), mSize);

    if (!file) {
        throw std::runtime_error("Failed to read file: " + filePath);
    }
}

BinaryReader::BinaryReader(const uint8_t* data, size_t size)
    : mSize(size) {
    if (!data || size == 0) {
        throw std::runtime_error("Invalid data buffer provided to BinaryReader");
    }
    mData = std::make_unique<uint8_t[]>(size);
    std::memcpy(mData.get(), data, size);
}

void BinaryReader::checkBounds(size_t offset, size_t size) const {
    if (offset > mSize || (offset + size) > mSize) {
        std::ostringstream oss;
        oss << "Out of bounds read: offset=" << offset 
            << ", size=" << size << ", total=" << mSize;
        throw std::out_of_range(oss.str());
    }
}

uint8_t BinaryReader::readU8(size_t offset) const {
    checkBounds(offset, 1);
    return mData[offset];
}

uint16_t BinaryReader::readU16(size_t offset) const {
    checkBounds(offset, 2);
    uint16_t value = static_cast<uint16_t>(mData[offset])
                   | (static_cast<uint16_t>(mData[offset + 1]) << 8);
    return value;
}

uint32_t BinaryReader::readU32(size_t offset) const {
    checkBounds(offset, 4);
    uint32_t value = static_cast<uint32_t>(mData[offset])
                   | (static_cast<uint32_t>(mData[offset + 1]) << 8)
                   | (static_cast<uint32_t>(mData[offset + 2]) << 16)
                   | (static_cast<uint32_t>(mData[offset + 3]) << 24);
    return value;
}

uint64_t BinaryReader::readU64(size_t offset) const {
    checkBounds(offset, 8);
    uint64_t value = static_cast<uint64_t>(mData[offset])
                   | (static_cast<uint64_t>(mData[offset + 1]) << 8)
                   | (static_cast<uint64_t>(mData[offset + 2]) << 16)
                   | (static_cast<uint64_t>(mData[offset + 3]) << 24)
                   | (static_cast<uint64_t>(mData[offset + 4]) << 32)
                   | (static_cast<uint64_t>(mData[offset + 5]) << 40)
                   | (static_cast<uint64_t>(mData[offset + 6]) << 48)
                   | (static_cast<uint64_t>(mData[offset + 7]) << 56);
    return value;
}

int32_t BinaryReader::readS32(size_t offset) const {
    return static_cast<int32_t>(readU32(offset));
}

std::vector<uint8_t> BinaryReader::readBytes(size_t offset, size_t size) const {
    checkBounds(offset, size);
    std::vector<uint8_t> result(mData.get() + offset, mData.get() + offset + size);
    return result;
}
