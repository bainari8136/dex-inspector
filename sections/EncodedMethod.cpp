#include "EncodedMethod.h"
#include "../Leb128.h"

std::vector<EncodedMethod> EncodedMethods::parse(const uint8_t* data, uint32_t& offset, uint32_t count) {
    std::vector<EncodedMethod> methods;
    methods.reserve(count);

    uint32_t currentMethodIdx = 0;

    for (uint32_t i = 0; i < count; ++i) {
        size_t bytesRead = 0;
        
        // Method index delta (LEB128)
        uint32_t methodIdxDelta = Leb128::decodeULEB128(data, offset, bytesRead);
        offset += bytesRead;
        
        // Access flags (LEB128)
        uint32_t accessFlags = Leb128::decodeULEB128(data, offset, bytesRead);
        offset += bytesRead;
        
        // Code offset (LEB128)
        uint32_t codeOff = Leb128::decodeULEB128(data, offset, bytesRead);
        offset += bytesRead;

        // Convert delta to absolute index
        currentMethodIdx += methodIdxDelta;

        EncodedMethod method{};
        method.methodIdx = currentMethodIdx;
        method.accessFlags = accessFlags;
        method.codeOff = codeOff;

        methods.push_back(method);
    }

    return methods;
}
