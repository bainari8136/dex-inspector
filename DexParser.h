#pragma once

#include <memory>
#include <string>

class DexFile;

/**
 * DexParser: Orchestrates the parsing of a DEX file.
 * This is a high-level coordinator that delegates to section-specific parsers.
 */
class DexParser {
public:
    /**
     * Parse a DEX file from the given path.
     * @param filePath Path to the .dex file.
     * @return A unique_ptr to the parsed DexFile.
     */
    static std::unique_ptr<DexFile> parse(const std::string& filePath);
};
