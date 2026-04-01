#include "ClassDefs.h"
#include <cstring>

std::vector<ClassDef> ClassDefs::parse(const uint8_t* data, uint32_t offset, uint32_t count) {
    std::vector<ClassDef> classDefs;
    classDefs.reserve(count);

    for (uint32_t i = 0; i < count; ++i) {
        ClassDef classDef = {};
        uint32_t itemOffset = offset + i * 32;  // 32 bytes per class def
        
        std::memcpy(&classDef.classIdx, data + itemOffset, 4);
        std::memcpy(&classDef.accessFlags, data + itemOffset + 4, 4);
        std::memcpy(&classDef.superclassIdx, data + itemOffset + 8, 4);
        std::memcpy(&classDef.interfacesOffset, data + itemOffset + 12, 4);
        std::memcpy(&classDef.sourceFileIdx, data + itemOffset + 16, 4);
        std::memcpy(&classDef.annotationsOffset, data + itemOffset + 20, 4);
        std::memcpy(&classDef.classDataOffset, data + itemOffset + 24, 4);
        std::memcpy(&classDef.staticValuesOffset, data + itemOffset + 28, 4);
        
        classDefs.push_back(classDef);
    }

    return classDefs;
}
