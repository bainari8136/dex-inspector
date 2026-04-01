#pragma once

#include "sections/DexHeader.h"
#include "sections/ProtoIds.h"
#include "sections/MethodIds.h"
#include "sections/FieldIds.h"
#include "sections/ClassDefs.h"
#include "sections/ClassData.h"
#include "sections/CodeItem.h"
#include "BinaryReader.h"
#include <memory>
#include <vector>
#include <string>

/**
 * DexFile: Represents a loaded .dex file in memory.
 * Provides access to parsed sections and lazy resolution mechanisms.
 */
class DexFile {
public:
    /**
     * Factory method: Create a DexFile by parsing a file.
     * @param filePath Path to the .dex file.
     * @return A unique_ptr to the parsed DexFile.
     */
    static std::unique_ptr<DexFile> parseFile(const std::string& filePath);

    /**
     * Get the DEX header.
     */
    const DexHeader& getHeader() const { return mHeader; }

    /**
     * Get a string by its index in the string IDs section.
     * Uses lazy resolution via string data items.
     */
    std::string getString(uint32_t stringIdx) const;

    /**
     * Get a type name by its index in the type IDs section.
     */
    std::string getType(uint32_t typeIdx) const;

    /**
     * Get all string indices.
     */
    std::vector<uint32_t> getAllStringOffsets() const;

    /**
     * Get all type indices.
     */
    std::vector<uint32_t> getAllTypeIndices() const;

    /**
     * Get all proto IDs (Phase 2).
     */
    std::vector<ProtoId> getAllProtos() const;

    /**
     * Get all field IDs (Phase 2).
     */
    std::vector<FieldId> getAllFields() const;

    /**
     * Get all method IDs (Phase 2).
     */
    std::vector<MethodId> getAllMethods() const;

    /**
     * Get a proto by index (Phase 2).
     */
    const ProtoId& getProto(uint32_t protoIdx) const;

    /**
     * Get a field by index (Phase 2).
     */
    const FieldId& getField(uint32_t fieldIdx) const;

    /**
     * Get a method by index (Phase 2).
     */
    const MethodId& getMethod(uint32_t methodIdx) const;

    /**
     * Get all class definitions (Phase 3).
     */
    std::vector<ClassDef> getAllClasses() const;

    /**
     * Get a class definition by index (Phase 3).
     */
    const ClassDef& getClass(uint32_t classIdx) const;

    /**
     * Get superclass name for a given class index (Phase 3).
     * Returns "Ljava/lang/Object;" if superclass is NO_INDEX, or empty string if error.
     */
    std::string getSuperclass(uint32_t classIdx) const;

    /**
     * Get source file name for a given class index (Phase 3).
     */
    std::string getSourceFile(uint32_t classIdx) const;

    /**
     * Parse class data from a given offset (Phase 4).
     * Used to extract methods and fields from a class definition.
     */
    ClassData getClassData(uint32_t offset) const;

    /**
     * Parse code item from a given offset (Phase 4).
     * Used to extract bytecode from a method.
     */
    CodeItem getCodeItem(uint32_t offset) const;

    /**
     * Get raw data pointer for advanced operations.
     */
    const uint8_t* getRawData() const;

    DexFile() = default;

private:
    std::unique_ptr<BinaryReader> mReader;
    DexHeader mHeader;
    std::vector<uint32_t> mStringOffsets;  // Offsets into the data section for each string
    std::vector<uint32_t> mTypeIndices;    // String indices for each type
    std::vector<ProtoId> mProtos;          // Proto IDs (Phase 2)
    std::vector<FieldId> mFields;          // Field IDs (Phase 2)
    std::vector<MethodId> mMethods;        // Method IDs (Phase 2)
    std::vector<ClassDef> mClasses;        // Class Defs (Phase 3)

    friend class DexParser;
};
