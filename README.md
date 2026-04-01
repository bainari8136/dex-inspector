# DEX Inspector

A modern C++17 CLI tool for parsing and displaying the internal components of Android `.dex` files in a clear, human-readable format.

## Features

### Currently Implemented (Phase 1)
- ✅ DEX file header parsing and display
- ✅ String identifiers section (with LEB128 string length decoding)
- ✅ Type identifiers section (with string resolution)
- ✅ CLI flags for selective output

### Planned
- Phase 2: Proto, Method, and Field IDs with full signature resolution
- Phase 3: Class definitions with inheritance and interface display
- Phase 4: Bytecode parsing and disassembly

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

### Basic Commands

#### Display Header Information
```bash
dex-inspector input.dex --header
```

Displays:
- Magic number and version
- Checksum and file size
- Offsets and sizes of all sections

#### Display Strings
```bash
dex-inspector input.dex --strings
```

Shows all strings defined in the DEX file, indexed and formatted for clarity.

#### Display Types
```bash
dex-inspector input.dex --types
```

Shows all type definitions, resolved to their string names (e.g., `Ljava/lang/Object;`).

#### Display All Information
```bash
dex-inspector input.dex --all
```

Or simply:
```bash
dex-inspector input.dex
```

#### Show Help
```bash
dex-inspector --help
```

### Examples

```bash
# Parse a DEX file and show only its header
dex-inspector classes.dex --header

# Display all strings in a DEX file
dex-inspector app.dex --strings

# Show types and nothing else
dex-inspector app.dex --types

# Display all available information
dex-inspector app.dex --all
```

## Project Structure

```
dex-inspector/
├── CMakeLists.txt                 # Build configuration
├── main.cpp                        # CLI entry point
├── BinaryReader.h / .cpp          # Safe binary data reading utilities
├── Leb128.h / .cpp                # ULEB128/SLEB128 decoding
├── DexFile.h / .cpp               # Main DEX file representation
├── DexParser.h / .cpp             # Orchestrator for parsing
└── sections/
    ├── DexHeader.h                # DEX file header structure
    ├── StringIds.h / .cpp         # String identifiers section
    └── TypeIds.h / .cpp           # Type identifiers section
```

## Implementation Details

### BinaryReader
Provides safe binary data access with:
- Little-endian integer reading (U8, U16, U32, U64, S32)
- Bounds checking on all operations
- Raw byte array access

### LEB128 Decoding
Implements ULEB128 (unsigned) and SLEB128 (signed) variable-length integer decoding as specified in the DEX format documentation.

### DEX Header Parsing
Reads the 0x70-byte DEX header containing:
- Magic number validation
- Section offsets and sizes
- File metadata (checksums, timestamps)

### String Resolution
- Parses string ID offsets from the string IDs section
- Resolves strings by decoding ULEB128 length followed by UTF-8 data
- Lazy loading: strings are decoded on-demand for efficiency

### Type Resolution
- Maps type indices to string indices
- Provides human-readable type names (e.g., `Ljava/lang/String;`)

## Testing

A test DEX file generator (`create_test_dex.py`) is included for validation:

```bash
python create_test_dex.py  # Creates test.dex
dex-inspector test.dex --all  # Parse the test file
```

## Error Handling

The tool includes comprehensive error handling for:
- Invalid file paths
- Corrupted DEX files
- Out-of-bounds reads
- Invalid LEB128 sequences

All errors are reported with descriptive messages to help identify issues.

## Future Enhancements

### Phase 2: Method and Field Resolution
- Parse ProtoIds (method prototypes)
- Resolve method signatures (class, name, parameters, return type)
- Parse FieldIds and resolve field types

### Phase 3: Class Definitions
- Parse ClassDefs section
- Display class hierarchies (inheritance)
- Show implemented interfaces

### Phase 4: Advanced Analysis
- Parse class data items for method/field details
- Basic bytecode disassembly
- Dependency analysis

## References

- [Android DEX File Format](https://source.android.com/docs/core/runtime/dex-format)
- [Dalvik Virtual Machine Specification](https://source.android.com/docs/core/runtime)

## License

This project is provided as-is for educational and development purposes.
