#include "DexParser.h"
#include "DexFile.h"

std::unique_ptr<DexFile> DexParser::parse(const std::string& filePath) {
    // For Phase 1, we delegate directly to DexFile::parseFile
    // In later phases, this can be extended with additional parsing logic
    return DexFile::parseFile(filePath);
}
