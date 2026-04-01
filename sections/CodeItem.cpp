#include "CodeItem.h"
#include <cstring>

CodeItem CodeItemParser::parse(const uint8_t* data, uint32_t offset) {
    CodeItem codeItem{};
    
    // First 16 bytes are fixed structure
    std::memcpy(&codeItem.registersSize, data + offset, 2);
    std::memcpy(&codeItem.insSize, data + offset + 2, 2);
    std::memcpy(&codeItem.outsSize, data + offset + 4, 2);
    std::memcpy(&codeItem.triesSize, data + offset + 6, 2);
    std::memcpy(&codeItem.debugInfoOff, data + offset + 8, 4);
    std::memcpy(&codeItem.insnsSize, data + offset + 12, 4);

    // Instructions start at offset + 16 and are 16-bit words
    uint32_t insnsOffset = offset + 16;
    codeItem.insns.resize(codeItem.insnsSize);
    for (uint32_t i = 0; i < codeItem.insnsSize; ++i) {
        std::memcpy(&codeItem.insns[i], data + insnsOffset + (i * 2), 2);
    }

    return codeItem;
}
