#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * [PHASE 3] ClassDefs: Class definitions.
 * Describes the structure and annotations of a class.
 */
struct ClassDef {
    uint32_t classIdx;           // Index into type IDs
    uint32_t accessFlags;        // Access flags (public, final, interface, etc.)
    uint32_t superclassIdx;      // Index into type IDs (NO_INDEX if none)
    uint32_t interfacesOffset;   // Offset into data section for interfaces array
    uint32_t sourceFileIdx;      // Index into string IDs for source file name
    uint32_t annotationsOffset;  // Offset into data section for annotations
    uint32_t classDataOffset;    // Offset into data section for class data
    uint32_t staticValuesOffset; // Offset into data section for static values
};

class ClassDefs {
public:
    /**
     * Parse class definitions section from a buffer.
     * [PHASE 3 - TO BE IMPLEMENTED]
     */
    static std::vector<ClassDef> parse(const uint8_t* data, uint32_t offset, uint32_t count);
};
