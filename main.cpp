#include "DexParser.h"
#include "DexFile.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>

void printHeader(const DexFile& dexFile) {
    std::cout << "\n=== DEX HEADER ===" << std::endl;
    
    const auto& header = dexFile.getHeader();
    
    // Print magic
    std::cout << "Magic: ";
    for (int i = 0; i < 4; ++i) {
        char c = static_cast<char>(header.magic[i]);
        if (c >= 32 && c < 127) {
            std::cout << c;
        } else {
            std::cout << "\\x" << std::hex << static_cast<int>(header.magic[i]) << std::dec;
        }
    }
    std::cout << std::endl;
    
    // Print version
    std::cout << "Version: ";
    for (int i = 0; i < 4; ++i) {
        char c = static_cast<char>(header.version[i]);
        if (c >= 32 && c < 127) {
            std::cout << c;
        } else {
            std::cout << "\\x" << std::hex << static_cast<int>(header.version[i]) << std::dec;
        }
    }
    std::cout << std::endl;
    
    std::cout << std::dec;
    std::cout << "Checksum: 0x" << std::hex << header.checksum << std::dec << std::endl;
    std::cout << "File Size: " << header.fileSize << " bytes" << std::endl;
    std::cout << "Header Size: " << header.headerSize << " bytes" << std::endl;
    std::cout << "Endian Tag: 0x" << std::hex << header.endianTag << std::dec << std::endl;
    
    std::cout << "\n--- Sections Offsets & Sizes ---" << std::endl;
    std::cout << "String IDs:  " << header.stringIdsSize << " items at offset 0x" 
              << std::hex << header.stringIdsOffset << std::dec << std::endl;
    std::cout << "Type IDs:    " << header.typeIdsSize << " items at offset 0x" 
              << std::hex << header.typeIdsOffset << std::dec << std::endl;
    std::cout << "Proto IDs:   " << header.protoIdsSize << " items at offset 0x" 
              << std::hex << header.protoIdsOffset << std::dec << std::endl;
    std::cout << "Field IDs:   " << header.fieldIdsSize << " items at offset 0x" 
              << std::hex << header.fieldIdsOffset << std::dec << std::endl;
    std::cout << "Method IDs:  " << header.methodIdsSize << " items at offset 0x" 
              << std::hex << header.methodIdsOffset << std::dec << std::endl;
    std::cout << "Class Defs:  " << header.classDefsSize << " items at offset 0x" 
              << std::hex << header.classDefsOffset << std::dec << std::endl;
    std::cout << "Data Section: " << header.dataSectionSize << " bytes at offset 0x" 
              << std::hex << header.dataSectionOffset << std::dec << std::endl;
}

void printStrings(const DexFile& dexFile) {
    std::cout << "\n=== STRINGS ===" << std::endl;
    
    auto offsets = dexFile.getAllStringOffsets();
    std::cout << "Total strings: " << offsets.size() << std::endl << std::endl;
    
    for (size_t i = 0; i < offsets.size(); ++i) {
        try {
            std::string str = dexFile.getString(i);
            std::cout << "[" << i << "] " << str << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[" << i << "] Error: " << e.what() << std::endl;
        }
    }
}

void printTypes(const DexFile& dexFile) {
    std::cout << "\n=== TYPES ===" << std::endl;
    
    auto indices = dexFile.getAllTypeIndices();
    std::cout << "Total types: " << indices.size() << std::endl << std::endl;
    
    for (size_t i = 0; i < indices.size(); ++i) {
        try {
            std::string typeName = dexFile.getType(i);
            std::cout << "[" << i << "] " << typeName << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[" << i << "] Error: " << e.what() << std::endl;
        }
    }
}

void printProtos(const DexFile& dexFile) {
    std::cout << "\n=== PROTOS ===" << std::endl;
    
    auto protos = dexFile.getAllProtos();
    std::cout << "Total protos: " << protos.size() << std::endl << std::endl;
    
    for (size_t i = 0; i < protos.size(); ++i) {
        try {
            const auto& proto = protos[i];
            std::string shortyStr = dexFile.getString(proto.shortyIdx);
            std::string returnType = dexFile.getType(proto.returnTypeIdx);
            std::cout << "[" << i << "] " << shortyStr << " -> " << returnType 
                      << " (params@0x" << std::hex << proto.parametersOffset << std::dec << ")" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[" << i << "] Error: " << e.what() << std::endl;
        }
    }
}

void printFields(const DexFile& dexFile) {
    std::cout << "\n=== FIELDS ===" << std::endl;
    
    auto fields = dexFile.getAllFields();
    std::cout << "Total fields: " << fields.size() << std::endl << std::endl;
    
    for (size_t i = 0; i < fields.size(); ++i) {
        try {
            const auto& field = fields[i];
            std::string className = dexFile.getType(field.classIdx);
            std::string fieldType = dexFile.getType(field.typeIdx);
            std::string fieldName = dexFile.getString(field.nameIdx);
            std::cout << "[" << i << "] " << className << "->" << fieldName << ":" << fieldType << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[" << i << "] Error: " << e.what() << std::endl;
        }
    }
}

void printMethods(const DexFile& dexFile) {
    std::cout << "\n=== METHODS ===" << std::endl;
    
    auto methods = dexFile.getAllMethods();
    std::cout << "Total methods: " << methods.size() << std::endl << std::endl;
    
    for (size_t i = 0; i < methods.size(); ++i) {
        try {
            const auto& method = methods[i];
            std::string className = dexFile.getType(method.classIdx);
            const auto& proto = dexFile.getProto(method.protoIdx);
            std::string methodName = dexFile.getString(method.nameIdx);
            std::string returnType = dexFile.getType(proto.returnTypeIdx);
            std::string shortyStr = dexFile.getString(proto.shortyIdx);
            std::cout << "[" << i << "] " << className << "->" << methodName << shortyStr << ":" << returnType << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[" << i << "] Error: " << e.what() << std::endl;
        }
    }
}

void printUsage(const char* programName) {
    std::cout << "DEX Inspector - Parse and display DEX file contents\n" << std::endl;
    std::cout << "Usage: " << programName << " <dex-file> [options]\n" << std::endl;
    std::cout << "Options:\n"
              << "  --header    Display DEX header information\n"
              << "  --strings   Display all strings from string IDs section\n"
              << "  --types     Display all types from type IDs section\n"
              << "  --protos    Display all protos from proto IDs section (Phase 2)\n"
              << "  --fields    Display all fields from field IDs section (Phase 2)\n"
              << "  --methods   Display all methods from method IDs section (Phase 2)\n"
              << "  --all       Display all available information (default)\n"
              << "  --help      Show this help message\n" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            printUsage(argv[0]);
            return 1;
        }

        std::string dexPath = argv[1];

        // Check for help flag
        for (int i = 2; i < argc; ++i) {
            if (std::strcmp(argv[i], "--help") == 0) {
                printUsage(argv[0]);
                return 0;
            }
        }

        // Parse the DEX file
        std::cout << "Loading DEX file: " << dexPath << std::endl;
        auto dexFile = DexParser::parse(dexPath);

        // If no specific flags, show all by default
        bool showAll = (argc == 2);
        bool showHeader = showAll || false;
        bool showStrings = showAll || false;
        bool showTypes = showAll || false;
        bool showProtos = showAll || false;
        bool showFields = showAll || false;
        bool showMethods = showAll || false;

        // Parse CLI flags
        for (int i = 2; i < argc; ++i) {
            if (std::strcmp(argv[i], "--header") == 0) {
                showHeader = true;
            } else if (std::strcmp(argv[i], "--strings") == 0) {
                showStrings = true;
            } else if (std::strcmp(argv[i], "--types") == 0) {
                showTypes = true;
            } else if (std::strcmp(argv[i], "--protos") == 0) {
                showProtos = true;
            } else if (std::strcmp(argv[i], "--fields") == 0) {
                showFields = true;
            } else if (std::strcmp(argv[i], "--methods") == 0) {
                showMethods = true;
            } else if (std::strcmp(argv[i], "--all") == 0) {
                showHeader = true;
                showStrings = true;
                showTypes = true;
                showProtos = true;
                showFields = true;
                showMethods = true;
            } else {
                std::cerr << "Unknown option: " << argv[i] << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        }

        // If specific flags are given, set showAll to false and use individual flags
        if (argc > 2 && !showAll) {
            showAll = false;
        }

        // Display requested information
        if (showHeader || showAll) {
            printHeader(*dexFile);
        }
        if (showStrings || showAll) {
            printStrings(*dexFile);
        }
        if (showTypes || showAll) {
            printTypes(*dexFile);
        }
        if (showProtos || showAll) {
            printProtos(*dexFile);
        }
        if (showFields || showAll) {
            printFields(*dexFile);
        }
        if (showMethods || showAll) {
            printMethods(*dexFile);
        }

        std::cout << "\n=== PARSING SUCCESSFUL ===" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
