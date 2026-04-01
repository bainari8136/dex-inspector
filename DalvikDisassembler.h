#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * DalvikDisassembler: Disassembles Dalvik bytecode instructions.
 * Implements 23 core instruction formats and basic opcode decoding.
 */
class DalvikDisassembler {
public:
    /**
     * Disassemble a single 16-bit Dalvik instruction.
     * @param opcode The instruction opcode.
     * @param nextWord Next 16-bit word (for extended instructions).
     * @return Disassembled instruction string.
     */
    static std::string disassembleInstruction(uint16_t opcode, uint16_t nextWord = 0);

    /**
     * Disassemble a sequence of instructions.
     * @param insns Vector of 16-bit instruction words.
     * @return Vector of disassembled instruction strings with hex offsets.
     */
    static std::vector<std::string> disassemble(const std::vector<uint16_t>& insns);

private:
    // Opcode definitions
    static constexpr uint8_t OP_NOP = 0x00;
    static constexpr uint8_t OP_MOVE = 0x01;
    static constexpr uint8_t OP_CONST = 0x14;
    static constexpr uint8_t OP_RETURN = 0x0E;
    static constexpr uint8_t OP_IF_EQ = 0x32;
    static constexpr uint8_t OP_INVOKE_VIRTUAL = 0x6E;
    
    // Helper methods
    static std::string getOpcodeName(uint8_t opcode);
    static std::string formatRegister(uint8_t reg);
    static std::string formatInstruction(const std::string& name, const std::string& args);
};
