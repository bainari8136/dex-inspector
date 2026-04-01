#pragma once

#include <cstdint>
#include <vector>

/**
 * CodeItem: Parsed code_item structure.
 * Located at the offset specified in EncodedMethod.codeOff.
 * Contains method bytecode and metadata.
 */
struct CodeItem {
    uint16_t registersSize;   // Number of registers used by this method
    uint16_t insSize;         // Number of words of incoming arguments to the method
    uint16_t outsSize;        // Number of words of outgoing argument space required by method
    uint16_t triesSize;       // Number of try_item blocks
    uint32_t debugInfoOff;    // Offset to debug_info_item (0 if none)
    uint32_t insnsSize;       // Size of instruction list in 16-bit code units
    std::vector<uint16_t> insns;  // Bytecode instructions
};

/**
 * CodeItemParser: Parser for code_item structures.
 */
class CodeItemParser {
public:
    /**
     * Parse a code_item from a buffer.
     * @param data Pointer to the data buffer.
     * @param offset Offset to the code_item.
     * @return Parsed CodeItem structure.
     */
    static CodeItem parse(const uint8_t* data, uint32_t offset);
};
