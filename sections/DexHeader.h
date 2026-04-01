#pragma once

#include <cstdint>
#include <array>
#include <vector>

/**
 * DEX File Format Header structure.
 * Reference: Android DEX format specification
 */
struct DexHeader {
    // magic number: "dex\n"
    std::array<uint8_t, 4> magic;
    
    // version: "035\0" or other valid versions
    std::array<uint8_t, 4> version;
    
    // SHA-1 hash over the rest of the rest of the file (after this field)
    std::array<uint8_t, 20> sha1;
    
    // CRC32 over the rest of the file (after this field)
    uint32_t checksum;
    
    // Size of the entire file (including this header)
    uint32_t fileSize;
    
    // Offset of the header from the start of the file
    uint32_t headerSize;
    
    // Endian-ness tag
    uint32_t endianTag;
    
    // Size of the link section, or 0 if unused
    uint32_t linkSize;
    
    // Offset of the link section from the start of the file
    uint32_t linkOffset;
    
    // Offset of the map list from the start of the file
    uint32_t mapOffset;
    
    // Number of string identifiers
    uint32_t stringIdsSize;
    
    // Offset of the string identifiers section from the start of the file
    uint32_t stringIdsOffset;
    
    // Number of type identifiers
    uint32_t typeIdsSize;
    
    // Offset of the type identifiers section from the start of the file
    uint32_t typeIdsOffset;
    
    // Number of prototype identifiers
    uint32_t protoIdsSize;
    
    // Offset of the prototype identifiers section from the start of the file
    uint32_t protoIdsOffset;
    
    // Number of field identifiers
    uint32_t fieldIdsSize;
    
    // Offset of the field identifiers section from the start of the file
    uint32_t fieldIdsOffset;
    
    // Number of method identifiers
    uint32_t methodIdsSize;
    
    // Offset of the method identifiers section from the start of the file
    uint32_t methodIdsOffset;
    
    // Number of class definitions
    uint32_t classDefsSize;
    
    // Offset of the class definitions section from the start of the file
    uint32_t classDefsOffset;
    
    // Size of data section
    uint32_t dataSectionSize;
    
    // Offset of the data section from the start of the file
    uint32_t dataSectionOffset;
};
