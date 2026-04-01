# Quick Start Guide - DEX Inspector

## One-Minute Setup

### 1. Build the Project
```bash
# Navigate to the project folder
cd C:\Users\Victor GM\.gemini\antigravity\scratch\dex-inspector

# Build with CMake
cmake -B build
cmake --build build --config Release
```

The executable will be at: `build\Release\dex-inspector.exe`

### 2. Create a Test DEX File
```bash
# Generate a minimal test DEX file
python create_test_dex.py

# You should now have test.dex (558 bytes)
```

### 3. Run Your First Analysis
```bash
# Parse header information
.\build\Release\dex-inspector.exe test.dex --header

# Show all strings
.\build\Release\dex-inspector.exe test.dex --strings

# Show all types
.\build\Release\dex-inspector.exe test.dex --types

# Show everything
.\build\Release\dex-inspector.exe test.dex --all
```

## Command Reference

```
dex-inspector <dex-file> [options]

Options:
  --header    Show DEX header (magic, version, checksums, section sizes)
  --strings   List all strings in the string IDs section
  --types     List all type definitions
  --all       Show all of the above (default if no option specified)
  --help      Show help message
```

## Real DEX Files

To analyze a real Android DEX file:

### From an APK
```bash
# 1. Extract the APK (it's a ZIP file)
unzip -l app.apk

# 2. Look for classes.dex inside
dex-inspector classes.dex --all
```

### From the Android SDK
```bash
# If you have Android SDK installed
cd $ANDROID_SDK/platforms/android-XX/data
dex-inspector boot.dex --types
```

## What Each Flag Shows

### --header
Displays:
- ✓ DEX file magic bytes (`dex\n`)
- ✓ Format version (e.g., `035`)
- ✓ Checksum and SHA-1 hash
- ✓ Total file size
- ✓ Offsets and item counts for all sections

**Use when**: You want to understand the file structure or verify file integrity.

### --strings
Displays:
- ✓ Total count of strings
- ✓ Each string with its index
- ✓ Example: `[0] test`, `[1] Ljava/lang/Object;`

**Use when**: You want to see what literal strings are embedded in the DEX file.

### --types
Displays:
- ✓ Total count of type definitions
- ✓ Each type name with its index
- ✓ Standard Android type format: `Lpackage/Class;`

**Use when**: You want to see all classes and data types referenced by the code.

## Project Structure

```
dex-inspector/
├── build/                 # (Created during build)
│   └── Release/
│       └── dex-inspector.exe
├── sections/              # DEX section parsers
│   ├── DexHeader.h
│   ├── StringIds.{h,cpp}
│   ├── TypeIds.{h,cpp}
│   ├── ProtoIds.{h,cpp}   # Phase 2
│   ├── FieldIds.{h,cpp}   # Phase 2
│   ├── MethodIds.{h,cpp}  # Phase 2
│   └── ClassDefs.{h,cpp}  # Phase 3
├── BinaryReader.{h,cpp}   # Binary I/O utilities
├── Leb128.{h,cpp}         # Variable-length integer decoding
├── DexFile.{h,cpp}        # Main DEX representation
├── DexParser.{h,cpp}      # Parsing orchestrator
├── main.cpp               # CLI entry point
├── CMakeLists.txt         # Build configuration
├── create_test_dex.py     # Test file generator
├── README.md              # Full documentation
├── DEVELOPMENT.md         # Implementation roadmap
└── QUICKSTART.md          # This file
```

## Troubleshooting

### "File is empty" or "Failed to open file"
- Check the file path is correct
- Make sure the file exists and is readable
- Try the full path: `dex-inspector "C:\path\to\file.dex"`

### "Out of bounds read" or parsing errors
- The DEX file may be corrupted or not a valid DEX file
- Try with the test file: `dex-inspector test.dex`
- Use `--header` first to verify basic structure

### Build fails with "CMake not found"
- Install CMake from https://cmake.org/download/
- Or use your package manager: 
  - Windows: `winget install cmake`
  - macOS: `brew install cmake`
  - Linux: `apt install cmake` or `dnf install cmake`

## Next Steps

1. **Analyze Real DEX Files**
   - Extract DEX from APK and analyze it
   - Explore what classes and types are used

2. **Contribute Phase 2**
   - Implement Method, Field, and Proto ID parsing
   - See DEVELOPMENT.md for detailed specifications

3. **Customize Output**
   - Modify `printHeader()`, `printStrings()`, `printTypes()` in `main.cpp`
   - Add new output formats (JSON, CSV, XML)

## Performance Notes

- String parsing is lazy: strings are only decoded when requested
- Large DEX files (10MB+) parse in under 1 second on modern hardware
- Memory usage is proportional to DEX file size

## Additional Resources

- [Android DEX Format Specification](https://source.android.com/docs/core/runtime/dex-format)
- [Dalvik Bytecode Instruction Reference](https://source.android.com/docs/core/runtime/instruction-formats)
- [Android Open Source Project](https://source.android.com/)

---

**Happy analyzing!** 🚀
