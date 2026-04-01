#include "DexStatistics.h"
#include "DexFile.h"
#include <iostream>
#include <iomanip>
#include <sstream>

std::string DexStatistics::formatSize(uint32_t bytes) {
    if (bytes < 1024) {
        return std::to_string(bytes) + " B";
    } else if (bytes < 1024 * 1024) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << (bytes / 1024.0) << " KB";
        return ss.str();
    } else {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << (bytes / (1024.0 * 1024.0)) << " MB";
        return ss.str();
    }
}

std::string DexStatistics::formatPercent(uint32_t part, uint32_t whole) {
    if (whole == 0) return "0.00%";
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << (100.0 * part / whole) << "%";
    return ss.str();
}

void DexStatistics::printStatistics(const DexFile& dexFile) {
    const auto& header = dexFile.getHeader();
    
    std::cout << "\n=== DEX STATISTICS ===" << std::endl;
    
    // File size statistics
    std::cout << "\n--- File Size ---" << std::endl;
    std::cout << "Total Size:        " << formatSize(header.fileSize) << " (" << header.fileSize << " bytes)" << std::endl;
    std::cout << "Header:            " << header.headerSize << " bytes" << std::endl;
    std::cout << "Data Section:      " << formatSize(header.dataSectionSize) << " (" << formatPercent(header.dataSectionSize, header.fileSize) << ")" << std::endl;
    
    // Section counts
    std::cout << "\n--- Section Counts ---" << std::endl;
    std::cout << "Strings:           " << header.stringIdsSize << std::endl;
    std::cout << "Types:             " << header.typeIdsSize << std::endl;
    std::cout << "Protos:            " << header.protoIdsSize << std::endl;
    std::cout << "Fields:            " << header.fieldIdsSize << std::endl;
    std::cout << "Methods:           " << header.methodIdsSize << std::endl;
    std::cout << "Classes:           " << header.classDefsSize << std::endl;
    
    // Calculated statistics
    auto allStrings = dexFile.getAllStringOffsets();
    auto allTypes = dexFile.getAllTypeIndices();
    auto allFields = dexFile.getAllFields();
    auto allMethods = dexFile.getAllMethods();
    auto allClasses = dexFile.getAllClasses();
    
    // Analyze classes
    std::cout << "\n--- Class Statistics ---" << std::endl;
    uint32_t interfaceCount = 0;
    uint32_t annotatedClasses = 0;
    uint32_t classesWithData = 0;
    
    for (const auto& cls : allClasses) {
        // Check if interface (0x0200 flag)
        if (cls.accessFlags & 0x0200) {
            interfaceCount++;
        }
        if (cls.annotationsOffset != 0) {
            annotatedClasses++;
        }
        if (cls.classDataOffset != 0) {
            classesWithData++;
        }
    }
    
    if (allClasses.size() > 0) {
        std::cout << "Total Classes:     " << allClasses.size() << std::endl;
        std::cout << "  Interfaces:      " << interfaceCount << " (" << formatPercent(interfaceCount, allClasses.size()) << ")" << std::endl;
        std::cout << "  With Annotations:" << annotatedClasses << " (" << formatPercent(annotatedClasses, allClasses.size()) << ")" << std::endl;
        std::cout << "  With Methods/Fields: " << classesWithData << " (" << formatPercent(classesWithData, allClasses.size()) << ")" << std::endl;
    }
    
    // Method statistics
    std::cout << "\n--- Method Statistics ---" << std::endl;
    if (allMethods.size() > 0) {
        std::cout << "Total Methods:     " << allMethods.size() << std::endl;
        std::cout << "(Note: Method access flags available in class data, not shown here)" << std::endl;
    }
    
    // Field statistics
    std::cout << "\n--- Field Statistics ---" << std::endl;
    if (allFields.size() > 0) {
        std::cout << "Total Fields:      " << allFields.size() << std::endl;
        std::cout << "(Note: Field access flags available in class data, not shown here)" << std::endl;
    }
    
    // Type usage
    std::cout << "\n--- String & Type Statistics ---" << std::endl;
    std::cout << "Total Strings:     " << allStrings.size() << std::endl;
    std::cout << "Total Types:       " << allTypes.size() << std::endl;
    
    std::cout << "\n=== STATISTICS COMPLETE ===" << std::endl;
}
