# DEX Inspector

A comprehensive C++17 CLI tool for parsing and analyzing Android `.dex` files with full support for headers, strings, types, methods, fields, classes, and bytecode analysis.

## Features

### Phase 1: Core Parsing ✅ COMPLETE
- ✅ DEX file header parsing and display
- ✅ String identifiers section (with LEB128 string length decoding)
- ✅ Type identifiers section (with string resolution)
- ✅ `--header`, `--strings`, `--types` CLI flags

### Phase 2: References Resolution ✅ COMPLETE
- ✅ Proto IDs section (method prototypes)
- ✅ Field IDs section with full signature resolution
- ✅ Method IDs section with signature building
- ✅ `--protos`, `--fields`, `--methods` CLI flags

### Phase 3: Classes ✅ COMPLETE
- ✅ Class definitions and hierarchy
- ✅ Superclass resolution
- ✅ Source file tracking
- ✅ Access flags and metadata
- ✅ `--classes` CLI flag

### Phase 4: Advanced Analysis ✅ COMPLETE
- ✅ Bytecode parsing (code_item structures)
- ✅ Class data extraction (encoded methods/fields)
- ✅ Dalvik instruction set (200+ opcodes)
- ✅ DEX statistics and analysis
- ✅ `--bytecode`, `--statistics` CLI flags

## Building

### Prerequisites
- CMake 3.16+
- C++17 compliant compiler (MSVC 2015+, GCC 7+, Clang 5+)

### Build Instructions

```bash
cd dex-inspector
cmake -B build
cmake --build build --config Release
```

The executable will be located at `build/Release/dex-inspector.exe` (Windows) or `build/dex-inspector` (Linux/macOS).

## Usage

### All CLI Flags

```
dex-inspector <dex-file> [options]

Options:
  --header      Display DEX header information
  --strings     Display all strings from string IDs section
  --types       Display all types from type IDs section
  --protos      Display all protos from proto IDs section (Phase 2)
  --fields      Display all fields from field IDs section (Phase 2)
  --methods     Display all methods from method IDs section (Phase 2)
  --classes     Display all classes from class defs section (Phase 3)
  --bytecode    Display bytecode and instruction statistics (Phase 4)
  --statistics  Display DEX file statistics (Phase 4)
  --all         Display all available information (default)
  --help        Show help message
```

### Basic Commands

#### Display Header Information
```bash
dex-inspector input.dex --header
```

#### Display Strings
```bash
dex-inspector input.dex --strings
```

#### Display Types
```bash
dex-inspector input.dex --types
```

#### Display Methods with Signatures
```bash
dex-inspector input.dex --methods
```

#### Display Classes
```bash
dex-inspector input.dex --classes
```

#### Display Bytecode Analysis
```bash
dex-inspector input.dex --bytecode
```

Shows:
- Number of methods with bytecode
- Total bytecode size
- Sample methods with instruction counts and register usage

#### Display Statistics
```bash
dex-inspector input.dex --statistics
```

Shows:
- File size breakdown
- Section item counts
- Interface detection
- Method/field accessibility breakdown

#### Display All Information
```bash
dex-inspector input.dex --all
```

Or simply:
```bash
dex-inspector input.dex
```

### Examples

```bash
# Parse a DEX file and show only its header
dex-inspector classes.dex --header

# Display all strings in a DEX file
dex-inspector app.dex --strings

# Show full method signatures
dex-inspector app.dex --methods

# Show classes with superclass information
dex-inspector app.dex --classes

# Get comprehensive bytecode analysis
dex-inspector app.dex --bytecode

# Get DEX file statistics and metrics
dex-inspector app.dex --statistics

# Display all available information
dex-inspector app.dex --all
```

## Project Structure

```
dex-inspector/
├── CMakeLists.txt                      # Build configuration
├── main.cpp                             # CLI entry point
├── BinaryReader.h / .cpp               # Safe binary data reading utilities
├── Leb128.h / .cpp                     # ULEB128/SLEB128 decoding
├── DexFile.h / .cpp                    # Main DEX file representation
├── DexParser.h / .cpp                  # Orchestrator for parsing
├── DalvikDisassembler.h / .cpp         # Dalvik bytecode instructions (200+ opcodes)
├── DexStatistics.h / .cpp              # Statistical analysis
└── sections/
    ├── DexHeader.h                     # DEX file header structure
    ├── StringIds.h / .cpp              # String identifiers section
    ├── TypeIds.h / .cpp                # Type identifiers section
    ├── ProtoIds.h / .cpp               # Proto identifiers (Phase 2)
    ├── FieldIds.h / .cpp               # Field identifiers (Phase 2)
    ├── MethodIds.h / .cpp              # Method identifiers (Phase 2)
    ├── ClassDefs.h / .cpp              # Class definitions (Phase 3)
    ├── EncodedField.h / .cpp           # Encoded field parsing (Phase 4)
    ├── EncodedMethod.h / .cpp          # Encoded method parsing (Phase 4)
    ├── ClassData.h / .cpp              # Class data item parsing (Phase 4)
    └── CodeItem.h / .cpp               # Code item/bytecode parsing (Phase 4)
```

## Implementation Details

### Phase 1: Core Parsing

**BinaryReader**
- Safe binary data access with little-endian support
- Bounds checking on all read operations
- U8, U16, U32, U64, S32 reading

**LEB128 Decoding**
- ULEB128 (unsigned) variable-length integer decoding
- SLEB128 (signed) variable-length integer decoding
- Efficient for space-saving in DEX format

**DEX Header Parsing**
- 0x70-byte header with 35+ fields
- Magic number validation (dex\n035)
- Section offsets and sizes for all 12+ sections

**String Resolution**
- ULEB128 length prefix + UTF-8 data
- Lazy-loaded on-demand string decoding
- Index-based string lookups

**Type Resolution**
- Type index to string index mapping
- Human-readable type names (e.g., Ljava/lang/String;)

### Phase 2: References Resolution

**Proto IDs**
- Method prototype structures (shortyIdx, returnTypeIdx, parametersOffset)
- Full method signature building

**Field IDs**
- Field identification and type resolution
- Complete field signatures

**Method IDs**
- Method index to class, proto, and name mapping
- Full method signature formatting

### Phase 3: Classes

**Class Definitions**
- ClassDef items (32 bytes each)
- Superclass resolution
- Source file tracking
- Access flags (public, final, interface, etc.)
- Interface list offsets
- Annotation and class data offsets

**Class Hierarchy**
- Automatic superclass name resolution
- Default to Object for missing superclass

### Phase 4: Advanced Analysis

**Bytecode Parsing (CodeItem)**
- Instruction list extraction
- Register usage tracking
- Try/catch block information 
- Debug info offset tracking

**Class Data Parsing**
- Static and instance field extraction
- Direct and virtual method extraction
- ULEB128 field/method index delta decoding
- Access flags per field/method

**Dalvik Disassembly**
- 200+ instruction opcodes recognized
- Hex offset formatting
- Instruction names and basic formatting

**Statistics Generation**
- File size breakdown with percentages
- Section item counts
- Interface class detection
- Bytecode method counting
- Human-readable size formatting

## Testing

A test DEX file generator (`create_test_dex.py`) is included for validation:

```bash
# Generate a minimal test DEX file
python create_test_dex.py

# Parse and display all information from the test file
dex-inspector test.dex --all

# Test individual features
dex-inspector test.dex --header
dex-inspector test.dex --strings
dex-inspector test.dex --types
dex-inspector test.dex --methods
dex-inspector test.dex --classes
dex-inspector test.dex --bytecode
dex-inspector test.dex --statistics
```

### Test Coverage
- ✅ Header parsing and validation
- ✅ String decoding with LEB128
- ✅ Type resolution
- ✅ Method signature building
- ✅ Class hierarchy
- ✅ Bytecode analysis
- ✅ Statistics generation

## Error Handling

The tool includes comprehensive error handling for:
- Invalid file paths and missing files
- Corrupted DEX files and invalid headers
- Out-of-bounds reads with bounds checking
- Invalid LEB128 sequences with proper validation
- Missing or zero offsets (graceful degradation)
- Unreferenced indices (NO_INDEX = 0xFFFFFFFF)

All errors are reported with descriptive messages to help identify issues quickly.

## Implementation Highlights

### Code Quality
- **C++17 Modern Standards**: Uses smart pointers, move semantics, and STL containers
- **Bounds Checking**: Every read operation validates against buffer size
- **Error Messages**: Descriptive exceptions with context
- **Cross-Platform**: Builds on Windows (MSVC), Linux (GCC), macOS (Clang)

### Performance
- **Lazy Loading**: Strings are decoded on-demand, not pre-loaded
- **Efficient Parsing**: LEB128 streaming decoder with minimal allocations
- **Memory Safe**: RAII patterns with unique_ptr for resource management

### DEX Format Support
- **Complete Header**: All 0x70 bytes with 35+ fields
- **All Sections**: 12+ sections supported (header, strings, types, protos, fields, methods, classes, etc.)
- **Dalvik Instructions**: 200+ instruction opcodes recognized
- **Class Data**: Full encoded field/method extraction

## Requirements

- **Build System**: CMake 3.16 or later
- **Compiler**: C++17 support (MSVC 2017+, GCC 7+, Clang 5+)
- **OS**: Windows, Linux, macOS
- **Dependencies**: None (standard library only)

## Completion Status

| Phase | Component | Status |
|-------|-----------|--------|
| **1** | Header, Strings, Types | ✅ Complete |
| **2** | Protos, Fields, Methods | ✅ Complete |
| **3** | Classes & Hierarchy | ✅ Complete |
| **4** | Bytecode & Statistics | ✅ Complete |

**Total**: 4 phases fully implemented with comprehensive DEX file analysis capabilities.

## Possible Future Enhancements

While all major phases are complete, potential future additions could include:

- **Enhanced Disassembly**: More detailed instruction operand formatting
- **Dependency Graphs**: Visual representation of class dependencies
- **Decompilation Hints**: Basic source language reconstruction
- **Annotation Parsing**: Full annotation data extraction
- **Performance Metrics**: Bytecode complexity analysis
- **Format Export**: Output to JSON, XML, or other formats
- **Interactive Mode**: Real-time DEX exploration

## References

- [Android DEX File Format](https://source.android.com/docs/core/runtime/dex-format)
- [Dalvik Virtual Machine Specification](https://source.android.com/docs/core/runtime)
- [Dalvik Instruction Format](https://source.android.com/docs/core/runtime/instruction-formats)

## License

(Add your license information here)

## Contributing

Contributions are welcome! Please ensure:
- C++17 compliance
- Adds/maintains bounds checking
- Includes error handling
- Updates documentation as needed

This project is provided as-is for educational and development purposes.
