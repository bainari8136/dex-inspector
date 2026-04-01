#include "DalvikDisassembler.h"
#include <sstream>
#include <iomanip>

std::string DalvikDisassembler::formatRegister(uint8_t reg) {
    if (reg == 0xFF) return "NO_REG";
    std::stringstream ss;
    ss << "v" << static_cast<int>(reg);
    return ss.str();
}

std::string DalvikDisassembler::formatInstruction(const std::string& name, const std::string& args) {
    if (args.empty()) {
        return name;
    }
    return name + " " + args;
}

std::string DalvikDisassembler::getOpcodeName(uint8_t opcode) {
    // Implement 23 core instruction opcodes
    switch (opcode) {
        case 0x00: return "nop";
        case 0x01: return "move";
        case 0x02: return "move/from16";
        case 0x03: return "move/16";
        case 0x04: return "move-wide";
        case 0x05: return "move-wide/from16";
        case 0x06: return "move-wide/16";
        case 0x07: return "move-object";
        case 0x08: return "move-object/from16";
        case 0x09: return "move-object/16";
        case 0x0A: return "move-result";
        case 0x0B: return "move-result-wide";
        case 0x0C: return "move-result-object";
        case 0x0D: return "move-exception";
        case 0x0E: return "return-void";
        case 0x0F: return "return";
        case 0x10: return "return-wide";
        case 0x11: return "return-object";
        case 0x12: return "const/4";
        case 0x13: return "const/16";
        case 0x14: return "const";
        case 0x15: return "const/high16";
        case 0x16: return "const-wide/16";
        case 0x17: return "const-wide/32";
        case 0x18: return "const-wide";
        case 0x19: return "const-wide/high16";
        case 0x1A: return "const-string";
        case 0x1B: return "const-string/jumbo";
        case 0x1C: return "const-class";
        case 0x1D: return "monitor-enter";
        case 0x1E: return "monitor-exit";
        case 0x1F: return "check-cast";
        case 0x20: return "instance-of";
        case 0x21: return "array-length";
        case 0x22: return "new-instance";
        case 0x23: return "new-array";
        case 0x24: return "filled-new-array";
        case 0x25: return "filled-new-array/range";
        case 0x26: return "fill-array-data";
        case 0x27: return "throw";
        case 0x28: return "goto";
        case 0x29: return "goto/16";
        case 0x2A: return "goto/32";
        case 0x2B: return "packed-switch";
        case 0x2C: return "sparse-switch";
        case 0x2D: return "cmpl-float";
        case 0x2E: return "cmpg-float";
        case 0x2F: return "cmpl-double";
        case 0x30: return "cmpg-double";
        case 0x31: return "cmp-long";
        case 0x32: return "if-eq";
        case 0x33: return "if-ne";
        case 0x34: return "if-lt";
        case 0x35: return "if-ge";
        case 0x36: return "if-gt";
        case 0x37: return "if-le";
        case 0x38: return "if-eqz";
        case 0x39: return "if-nez";
        case 0x3A: return "if-ltz";
        case 0x3B: return "if-gez";
        case 0x3C: return "if-gtz";
        case 0x3D: return "if-lez";
        case 0x44: return "aget";
        case 0x45: return "aget-wide";
        case 0x46: return "aget-object";
        case 0x47: return "aget-boolean";
        case 0x48: return "aget-byte";
        case 0x49: return "aget-char";
        case 0x4A: return "aget-short";
        case 0x4B: return "aput";
        case 0x4C: return "aput-wide";
        case 0x4D: return "aput-object";
        case 0x4E: return "aput-boolean";
        case 0x4F: return "aput-byte";
        case 0x50: return "aput-char";
        case 0x51: return "aput-short";
        case 0x52: return "iget";
        case 0x53: return "iget-wide";
        case 0x54: return "iget-object";
        case 0x55: return "iget-boolean";
        case 0x56: return "iget-byte";
        case 0x57: return "iget-char";
        case 0x58: return "iget-short";
        case 0x59: return "iput";
        case 0x5A: return "iput-wide";
        case 0x5B: return "iput-object";
        case 0x5C: return "iput-boolean";
        case 0x5D: return "iput-byte";
        case 0x5E: return "iput-char";
        case 0x5F: return "iput-short";
        case 0x60: return "sget";
        case 0x61: return "sget-wide";
        case 0x62: return "sget-object";
        case 0x63: return "sget-boolean";
        case 0x64: return "sget-byte";
        case 0x65: return "sget-char";
        case 0x66: return "sget-short";
        case 0x67: return "sput";
        case 0x68: return "sput-wide";
        case 0x69: return "sput-object";
        case 0x6A: return "sput-boolean";
        case 0x6B: return "sput-byte";
        case 0x6C: return "sput-char";
        case 0x6D: return "sput-short";
        case 0x6E: return "invoke-virtual";
        case 0x6F: return "invoke-super";
        case 0x70: return "invoke-direct";
        case 0x71: return "invoke-static";
        case 0x72: return "invoke-interface";
        case 0x74: return "invoke-virtual/range";
        case 0x75: return "invoke-super/range";
        case 0x76: return "invoke-direct/range";
        case 0x77: return "invoke-static/range";
        case 0x78: return "invoke-interface/range";
        case 0x7B: return "neg-int";
        case 0x7C: return "not-int";
        case 0x7D: return "neg-long";
        case 0x7E: return "not-long";
        case 0x7F: return "neg-float";
        case 0x80: return "neg-double";
        case 0x81: return "int-to-long";
        case 0x82: return "int-to-float";
        case 0x83: return "int-to-double";
        case 0x84: return "long-to-int";
        case 0x85: return "long-to-float";
        case 0x86: return "long-to-double";
        case 0x87: return "float-to-int";
        case 0x88: return "float-to-long";
        case 0x89: return "float-to-double";
        case 0x8A: return "double-to-int";
        case 0x8B: return "double-to-long";
        case 0x8C: return "double-to-float";
        case 0x8D: return "int-to-byte";
        case 0x8E: return "int-to-char";
        case 0x8F: return "int-to-short";
        case 0x90: return "add-int";
        case 0x91: return "sub-int";
        case 0x92: return "mul-int";
        case 0x93: return "div-int";
        case 0x94: return "rem-int";
        case 0x95: return "and-int";
        case 0x96: return "or-int";
        case 0x97: return "xor-int";
        case 0x98: return "shl-int";
        case 0x99: return "shr-int";
        case 0x9A: return "ushr-int";
        case 0x9B: return "add-long";
        case 0x9C: return "sub-long";
        case 0x9D: return "mul-long";
        case 0x9E: return "div-long";
        case 0x9F: return "rem-long";
        case 0xA0: return "and-long";
        case 0xA1: return "or-long";
        case 0xA2: return "xor-long";
        case 0xA3: return "shl-long";
        case 0xA4: return "shr-long";
        case 0xA5: return "ushr-long";
        case 0xA6: return "add-float";
        case 0xA7: return "sub-float";
        case 0xA8: return "mul-float";
        case 0xA9: return "div-float";
        case 0xAA: return "rem-float";
        case 0xAB: return "add-double";
        case 0xAC: return "sub-double";
        case 0xAD: return "mul-double";
        case 0xAE: return "div-double";
        case 0xAF: return "rem-double";
        case 0xB0: return "add-int/2addr";
        case 0xB1: return "sub-int/2addr";
        case 0xB2: return "mul-int/2addr";
        case 0xB3: return "div-int/2addr";
        case 0xB4: return "rem-int/2addr";
        case 0xB5: return "and-int/2addr";
        case 0xB6: return "or-int/2addr";
        case 0xB7: return "xor-int/2addr";
        case 0xB8: return "shl-int/2addr";
        case 0xB9: return "shr-int/2addr";
        case 0xBA: return "ushr-int/2addr";
        case 0xBB: return "add-long/2addr";
        case 0xBC: return "sub-long/2addr";
        case 0xBD: return "mul-long/2addr";
        case 0xBE: return "div-long/2addr";
        case 0xBF: return "rem-long/2addr";
        case 0xC0: return "and-long/2addr";
        case 0xC1: return "or-long/2addr";
        case 0xC2: return "xor-long/2addr";
        case 0xC3: return "shl-long/2addr";
        case 0xC4: return "shr-long/2addr";
        case 0xC5: return "ushr-long/2addr";
        case 0xC6: return "add-float/2addr";
        case 0xC7: return "sub-float/2addr";
        case 0xC8: return "mul-float/2addr";
        case 0xC9: return "div-float/2addr";
        case 0xCA: return "rem-float/2addr";
        case 0xCB: return "add-double/2addr";
        case 0xCC: return "sub-double/2addr";
        case 0xCD: return "mul-double/2addr";
        case 0xCE: return "div-double/2addr";
        case 0xCF: return "rem-double/2addr";
        default: return "unknown";
    }
}

std::string DalvikDisassembler::disassembleInstruction(uint16_t opcode, uint16_t nextWord) {
    uint8_t op = opcode & 0xFF;
    std::string name = getOpcodeName(op);
    
    // For now, just return the opcode name with hex representation
    std::stringstream ss;
    ss << name << " (0x" << std::hex << std::setfill('0') << std::setw(4) << opcode << ")";
    return ss.str();
}

std::vector<std::string> DalvikDisassembler::disassemble(const std::vector<uint16_t>& insns) {
    std::vector<std::string> result;
    
    for (size_t i = 0; i < insns.size(); ++i) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(4) << (i * 2) << ": ";
        ss << disassembleInstruction(insns[i]);
        result.push_back(ss.str());
    }
    
    return result;
}
