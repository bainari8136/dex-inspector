#include "DexParser.h"
#include "DexFile.h"
#include "DalvikDisassembler.h"
#include "DexStatistics.h"
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

void printClasses(const DexFile& dexFile) {
    std::cout << "\n=== CLASSES ===" << std::endl;
    
    auto classes = dexFile.getAllClasses();
    std::cout << "Total classes: " << classes.size() << std::endl << std::endl;
    
    for (size_t i = 0; i < classes.size(); ++i) {
        try {
            const auto& classDef = classes[i];
            std::string className = dexFile.getType(classDef.classIdx);
            std::string superclass = dexFile.getSuperclass(i);
            std::string sourceFile = dexFile.getSourceFile(i);
            
            std::cout << "[" << i << "] " << className << std::endl;
            std::cout << "    Superclass: " << superclass << std::endl;
            std::cout << "    Source File: " << (sourceFile.empty() ? "(none)" : sourceFile) << std::endl;
            std::cout << "    Access Flags: 0x" << std::hex << classDef.accessFlags << std::dec << std::endl;
            std::cout << "    Interfaces: " << (classDef.interfacesOffset == 0 ? "(none)" : "(present)") << std::endl;
            std::cout << "    Annotations: " << (classDef.annotationsOffset == 0 ? "(none)" : "(present)") << std::endl;
            std::cout << "    Class Data: " << (classDef.classDataOffset == 0 ? "(none)" : "(present)") << std::endl;
            std::cout << "    Static Values: " << (classDef.staticValuesOffset == 0 ? "(none)" : "(present)") << std::endl;
            std::cout << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[" << i << "] Error: " << e.what() << std::endl;
        }
    }
}

void printBytecode(const DexFile& dexFile) {
    std::cout << "\n=== BYTECODE ===" << std::endl;
    
    auto classes = dexFile.getAllClasses();
    auto methods = dexFile.getAllMethods();
    
    std::cout << "Scanning for bytecode..." << std::endl;
    uint32_t bytecodeMethodCount = 0;
    uint32_t totalBytecodeSize = 0;

    for (size_t classIdx = 0; classIdx < classes.size(); ++classIdx) {
        try {
            const auto& classDef = classes[classIdx];
            
            // Skip if no class data
            if (classDef.classDataOffset == 0) {
                continue;
            }
            
            ClassData classData = dexFile.getClassData(classDef.classDataOffset);
            
            // Process all methods (direct and virtual)
            std::vector<EncodedMethod> allClassMethods = classData.directMethods;
            allClassMethods.insert(allClassMethods.end(), classData.virtualMethods.begin(), classData.virtualMethods.end());
            
            for (const auto& encodedMethod : allClassMethods) {
                if (encodedMethod.codeOff == 0) {
                    continue;  // Abstract or native method
                }
                
                bytecodeMethodCount++;
                try {
                    CodeItem codeItem = dexFile.getCodeItem(encodedMethod.codeOff);
                    totalBytecodeSize += codeItem.insnsSize * 2;  // insnsSize is in 16-bit words
                    
                    // Show sample of this method
                    if (bytecodeMethodCount <= 5) {  // Show first 5 methods only
                        if (encodedMethod.methodIdx < methods.size()) {
                            std::string methodName = dexFile.getString(methods[encodedMethod.methodIdx].nameIdx);
                            std::cout << "\nMethod [" << bytecodeMethodCount << "]: " << methodName << std::endl;
                            std::cout << "  Instructions: " << codeItem.insnsSize << " words (" << (codeItem.insnsSize * 2) << " bytes)" << std::endl;
                            std::cout << "  Registers: " << codeItem.registersSize << std::endl;
                            std::cout << "  Args: " << codeItem.insSize << std::endl;
                            std::cout << "  Outs: " << codeItem.outsSize << std::endl;
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing code item: " << e.what() << std::endl;
                }
            }
        } catch (const std::exception& e) {
            // Skip class if error
            continue;
        }
    }
    
    std::cout << "\n--- Bytecode Summary ---" << std::endl;
    std::cout << "Methods with bytecode: " << bytecodeMethodCount << std::endl;
    std::cout << "Total bytecode size: " << totalBytecodeSize << " bytes (" << (totalBytecodeSize / 1024.0) << " KB)" << std::endl;
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
              << "  --classes   Display all classes from class defs section (Phase 3)\n"
              << "  --bytecode  Display bytecode and instruction statistics (Phase 4)\n"
              << "  --statistics Display DEX file statistics (Phase 4)\n"
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
        bool showClasses = showAll || false;
        bool showBytecode = false;
        bool showStatistics = false;

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
            } else if (std::strcmp(argv[i], "--classes") == 0) {
                showClasses = true;
            } else if (std::strcmp(argv[i], "--bytecode") == 0) {
                showBytecode = true;
            } else if (std::strcmp(argv[i], "--statistics") == 0) {
                showStatistics = true;
            } else if (std::strcmp(argv[i], "--all") == 0) {
                showHeader = true;
                showStrings = true;
                showTypes = true;
                showProtos = true;
                showFields = true;
                showMethods = true;
                showClasses = true;
                showBytecode = true;
                showStatistics = true;
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
        if (showClasses || showAll) {
            printClasses(*dexFile);
        }
        if (showBytecode) {
            printBytecode(*dexFile);
        }
        if (showStatistics) {
            DexStatistics::printStatistics(*dexFile);
        }

        std::cout << "\n=== PARSING SUCCESSFUL ===" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
