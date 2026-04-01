#include "ClassData.h"
#include "../Leb128.h"

ClassData ClassDataParser::parse(const uint8_t* data, uint32_t offset) {
    ClassData classData{};
    
    size_t bytesRead = 0;

    // Static fields size (LEB128)
    uint32_t staticFieldsSize = Leb128::decodeULEB128(data, offset, bytesRead);
    offset += bytesRead;

    // Instance fields size (LEB128)
    uint32_t instanceFieldsSize = Leb128::decodeULEB128(data, offset, bytesRead);
    offset += bytesRead;

    // Direct methods size (LEB128)
    uint32_t directMethodsSize = Leb128::decodeULEB128(data, offset, bytesRead);
    offset += bytesRead;

    // Virtual methods size (LEB128)
    uint32_t virtualMethodsSize = Leb128::decodeULEB128(data, offset, bytesRead);
    offset += bytesRead;

    // Parse static fields
    classData.staticFields = EncodedFields::parse(data, offset, staticFieldsSize);

    // Parse instance fields
    classData.instanceFields = EncodedFields::parse(data, offset, instanceFieldsSize);

    // Parse direct methods
    classData.directMethods = EncodedMethods::parse(data, offset, directMethodsSize);

    // Parse virtual methods
    classData.virtualMethods = EncodedMethods::parse(data, offset, virtualMethodsSize);

    return classData;
}
