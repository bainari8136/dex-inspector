# DEX Inspector Development Roadmap

## Phase 1: MVP & Core Setup ✅ COMPLETE

### Completed Features
- ✅ CMake build system with C++17 support (Windows/Linux/macOS compatible)
- ✅ `BinaryReader` utility class with bounds checking and endianness handling
- ✅ ULEB128 and SLEB128 variable-length integer decoding
- ✅ DEX header parsing (0x70 bytes)
- ✅ String IDs section parsing with lazy string resolution
- ✅ Type IDs section parsing with name resolution
- ✅ CLI interface with flags: `--header`, `--strings`, `--types`, `--all`
- ✅ Test DEX file generator for validation
- ✅ Comprehensive error handling with bounds checking

### Test Results
All Phase 1 functionality has been tested and verified working:
- Header information displays correctly
- Strings are properly decoded from LEB128-encoded data
- Type names are resolved to their string representations
- CLI flags work as expected

---

## Phase 2: Resolving References (READY FOR IMPLEMENTATION)

### Objectives
Enable parsing of method and field references with full signature resolution.

### Components to Implement

#### `ProtoIds.h / .cpp`
- Parse the proto IDs section (contains method prototypes)
- Structure: `shortyIdx`, `returnTypeIdx`, `parametersOffset`
- Methods needed:
  - `parse()` - Read proto ID array
  - `getSignature()` - Build full method signature from proto data

#### `FieldIds.h / .cpp`
- Parse the field IDs section
- Structure: `classIdx`, `typeIdx`, `nameIdx`
- Methods needed:
  - `parse()` - Read field ID array
  - `toString()` - Format as "Lclass;->name:type"

#### `MethodIds.h / .cpp`
- Parse the method IDs section
- Structure: `classIdx`, `protoIdx`, `nameIdx`
- Methods needed:
  - `parse()` - Read method ID array
  - `getFullSignature()` - Build signature like "Lclass;->method(args)return"

#### `DexFile.cpp` Extensions
- Add methods:
  - `getMethod(idx)` - Resolve method signature
  - `getField(idx)` - Resolve field definition
  - `getProto(idx)` - Resolve prototype
- Update parsing to load these sections

#### `main.cpp` Extensions
- Add CLI flags: `--methods`, `--fields`, `--protos`
- Add display functions:
  - `printMethods()` - List all methods with signatures
  - `printFields()` - List all fields with types

### Data Structures to Parse
- **Proto ID item**: 12 bytes each
  - `shorty_idx`: 4 bytes (string ID)
  - `return_type_idx`: 4 bytes (type ID)
  - `parameters_off`: 4 bytes (offset to type_list)

- **Field ID item**: 8 bytes each
  - `class_idx`: 2 bytes (type ID)
  - `type_idx`: 2 bytes (type ID)
  - `name_idx`: 4 bytes (string ID)

- **Method ID item**: 8 bytes each
  - `class_idx`: 2 bytes (type ID)
  - `proto_idx`: 2 bytes (proto ID)
  - `name_idx`: 4 bytes (string ID)

### Expected Output Example
```
=== METHODS ===
[0] Ljava/lang/Object;->toString()Ljava/lang/String;
[1] Ljava/lang/Object;->equals(Ljava/lang/Object;)Z
...

=== FIELDS ===
[0] Ljava/lang/Example;->mValue:I
[1] Ljava/lang/Example;->mName:Ljava/lang/String;
...
```

### Testing Strategy
- Create extended test DEX with proto/method/field data
- Verify signature formatting matches standard Android format
- Compare output against `dexdump` tool if available

---

## Phase 3: Classes (PLANNED)

### Objectives
Parse and display class definitions, inheritance hierarchies, and interfaces.

### Components to Implement

#### `ClassDefs.h / .cpp`
- Parse ClassDef items (32 bytes each)
- Build class hierarchy starting from each class definition
- Methods needed:
  - `parse()` - Read class definitions
  - `getSuperclass()` - Resolve parent class
  - `getInterfaces()` - List implemented interfaces
  - `getAnnotations()` - Access class annotations

#### `DexFile.cpp` Extensions
- Add methods:
  - `getClass(idx)` - Get class definition
  - `getClassHierarchy(idx)` - Build inheritance chain up to Object
  - `getImplementedInterfaces(idx)` - List all interfaces

#### `main.cpp` Extensions
- Add CLI flag: `--classes`
- Add display function: `printClasses()`

### Expected Output Example
```
=== CLASSES ===
Total: 42 classes

[0] Lcom/example/MyClass;
  Superclass: Ljava/lang/Object;
  Interfaces: Ljava/lang/Runnable;, Ljava/lang/Cloneable;
  Flags: public, final
  Source File: MyClass.java
  
[1] Lcom/example/MyInterface;
  Superclass: Ljava/lang/Object;
  Is Interface: true
  Methods: 3
  Fields: 0
...
```

### Data Structures
- **ClassDef item**: 32 bytes each
  - `class_idx`: 4 bytes (type ID)
  - `access_flags`: 4 bytes
  - `superclass_idx`: 4 bytes (type ID)
  - `interfaces_off`: 4 bytes (offset to type_list)
  - `source_file_idx`: 4 bytes (string ID)
  - `annotations_off`: 4 bytes (offset)
  - `class_data_off`: 4 bytes (offset)
  - `static_values_off`: 4 bytes (offset)

---

## Phase 4: Advanced - Bytecode & Data (FUTURE)

### Objectives
Parse bytecode, provide basic disassembly, and extract detailed class information.

### Components to Implement

#### Bytecode Parser
- Parse `code_item` structures
- Decode instruction formats
- Implement basic disassembler

#### Class Data Parser
- Parse `class_data_item` structures
- Extract encoded method and field data
- Handle encoded arrays and annotations

#### Utility Features
- Build dependency graphs
- Generate summary statistics
- Find unused classes/methods

### Example CLI Flags
- `--bytecode` - Show method bytecode
- `--disassemble <method>` - Disassemble specific method
- `--statistics` - Show DEX statistics

---

## Technical Notes

### Parsing Order
DEX sections must be parsed in a specific order due to dependencies:
1. Header (defines offsets)
2. String IDs (needed for all string lookups)
3. Type IDs (needed for type lookups)
4. Proto IDs (needed for method signatures)
5. Field IDs, Method IDs (can be parallel)
6. Class Defs (references all above)

### Common Pitfalls to Avoid
- Not handling `NO_INDEX` (0xFFFFFFFF) values
- Forgetting that offsets in DEX are absolute from file start
- Missing null termination on decoded strings
- Not accounting for alignment padding in data sections

### Testing Checklist
- [ ] Build succeeds without warnings
- [ ] Parsed output matches `dexdump --display=all`
- [ ] Handles corrupted DEX files without crashing
- [ ] All bounds checks prevent buffer overruns
- [ ] Memory is properly managed (no leaks)

---

## Build & Test Commands

### Build
```bash
cd dex-inspector
cmake -B build
cmake --build build --config Release
```

### Test Phase 1
```bash
./build/Release/dex-inspector test.dex --all
```

### Compare Against Android tools
```bash
# Using dexdump (if available in Android SDK)
dexdump -d test.dex | head -100
```

---

## Notes for Future Developers

### Code Quality Guidelines
1. All public methods should include bounds checking
2. Use `std::unique_ptr` for resource management
3. Provide comprehensive error messages
4. Add inline comments for complex binary format handling

### Adding New Sections
1. Create `sections/NewSection.h` with structure definition
2. Implement `parse()` static method
3. Add access methods to `DexFile`
4. Update `main.cpp` with display logic
5. Add test case

### IDE Integration
The project includes support for:
- Visual Studio (generates .sln)
- CMake tools in VS Code
- Qt Creator
- JetBrains CLion

Simply open the project folder in your preferred IDE.

---

## Estimated Timeline

- **Phase 1**: ✅ Complete (Reference: working implementation provided)
- **Phase 2**: ~4-6 hours (straightforward parsing, similar to Phase 1)
- **Phase 3**: ~4-6 hours (more complex data structures, but logical progression)
- **Phase 4**: ~8-12 hours (bytecode analysis is more involved)

**Total Estimated Effort**: ~16-24 hours of development time
