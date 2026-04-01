cmake_minimum_required(VERSION 3.16)
project(dex-inspector CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific compiler flags
if(MSVC)
    # MSVC flags
    add_compile_options(/W4)
else()
    # GCC / Clang flags
    add_compile_options(-Wall -Wextra)
endif()

# Define the executable
add_executable(dex-inspector
    main.cpp
    BinaryReader.cpp
    Leb128.cpp
    DexFile.cpp
    DexParser.cpp
    DalvikDisassembler.cpp
    DexStatistics.cpp
    sections/StringIds.cpp
    sections/TypeIds.cpp
    sections/ProtoIds.cpp
    sections/MethodIds.cpp
    sections/FieldIds.cpp
    sections/ClassDefs.cpp
    sections/EncodedField.cpp
    sections/EncodedMethod.cpp
    sections/ClassData.cpp
    sections/CodeItem.cpp
)

# Include directories
target_include_directories(dex-inspector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
