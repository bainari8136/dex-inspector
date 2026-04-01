#pragma once

#include <cstdint>
#include <string>

class DexFile;

/**
 * DexStatistics: Generates statistics and analysis about a DEX file.
 */
class DexStatistics {
public:
    /**
     * Generate and print comprehensive DEX statistics.
     * @param dexFile The parsed DEX file.
     */
    static void printStatistics(const DexFile& dexFile);

private:
    /**
     * Format file size as human-readable string.
     */
    static std::string formatSize(uint32_t bytes);

    /**
     * Calculate percentage as string.
     */
    static std::string formatPercent(uint32_t part, uint32_t whole);
};
