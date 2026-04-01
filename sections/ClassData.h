#pragma once

#include <cstdint>
#include <vector>
#include "EncodedField.h"
#include "EncodedMethod.h"

/**
 * ClassData: Parsed class_data_item structure.
 * Located at the offset specified in ClassDef.classDataOffset.
 * Contains encoded lists of fields and methods for a class.
 */
struct ClassData {
    std::vector<EncodedField> staticFields;    // Static fields
    std::vector<EncodedField> instanceFields;  // Instance fields
    std::vector<EncodedMethod> directMethods;  // Direct methods (private, static, constructor)
    std::vector<EncodedMethod> virtualMethods; // Virtual methods (inherited/overridden)
};

/**
 * ClassDataParser: Parser for class_data_item structures.
 */
class ClassDataParser {
public:
    /**
     * Parse a class_data_item from a buffer.
     * @param data Pointer to the data buffer.
     * @param offset Offset to the class_data_item.
     * @return Parsed ClassData structure.
     */
    static ClassData parse(const uint8_t* data, uint32_t offset);
};
