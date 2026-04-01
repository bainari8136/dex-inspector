#!/usr/bin/env python3
"""
Simple DEX file generator for testing the DEX Inspector.
This creates a minimal but valid DEX file with a few strings and types.
"""

import struct

def create_minimal_dex():
    """Create a minimal DEX file for testing."""
    
    # Header: 0x70 (112 bytes)
    dex_data = bytearray(0x1000)  # Start with 4KB
    
    # Magic: "dex\n"
    dex_data[0:4] = b'dex\n'
    
    # Version: "035\0"
    dex_data[4:8] = b'035\0'
    
    # SHA-1 (20 bytes) - placeholder
    for i in range(8, 28):
        dex_data[i] = 0
    
    # We'll fill these in later after calculating
    checksum_pos = 28
    filesize_pos = 32
    header_size = 0x70
    endian_tag = 0x12345678  # Little-endian
    
    # Link section
    link_size = 0
    link_offset = 0
    
    # Map offset (we'll put it right after header)
    map_offset = header_size
    
    # String data
    strings = [
        b"test",
        b"Ljava/lang/Object;",
        b"Ljava/lang/String;",
    ]
    
    # Calculate offsets
    data_offset = 0x200  # Start data section at offset 512
    string_ids_offset = header_size
    type_ids_offset = string_ids_offset + len(strings) * 4
    
    string_data_offset = data_offset
    
    # Build string data
    string_offsets = []
    current_offset = string_data_offset
    for s in strings:
        string_offsets.append(current_offset)
        # Write string: ULEB128 length + string data (null-terminated)
        length_bytes = []
        length = len(s)
        while length >= 0x80:
            length_bytes.append((length & 0x7F) | 0x80)
            length >>= 7
        length_bytes.append(length)
        
        string_data = bytes(length_bytes) + s + b'\x00'
        dex_data[current_offset:current_offset + len(string_data)] = string_data
        current_offset += len(string_data)
    
    # Write string IDs (offsets into data section)
    for i, offset in enumerate(string_offsets):
        struct.pack_into('<I', dex_data, string_ids_offset + i * 4, offset)
    
    # Write type IDs (string indices)
    type_ids_data = [1, 2]  # Types referring to strings 1 and 2
    for i, type_idx in enumerate(type_ids_data):
        struct.pack_into('<I', dex_data, type_ids_offset + i * 4, type_idx)
    
    # Calculate total file size
    file_size = current_offset
    
    # Write header fields
    struct.pack_into('<I', dex_data, filesize_pos, file_size)
    struct.pack_into('<I', dex_data, 0x24, header_size)
    struct.pack_into('<I', dex_data, 0x28, endian_tag)
    struct.pack_into('<I', dex_data, 0x2C, link_size)
    struct.pack_into('<I', dex_data, 0x30, link_offset)
    struct.pack_into('<I', dex_data, 0x34, map_offset)
    
    # String IDs
    struct.pack_into('<I', dex_data, 0x38, len(strings))  # stringIdsSize
    struct.pack_into('<I', dex_data, 0x3C, string_ids_offset)  # stringIdsOffset
    
    # Type IDs
    struct.pack_into('<I', dex_data, 0x40, len(type_ids_data))  # typeIdsSize
    struct.pack_into('<I', dex_data, 0x44, type_ids_offset)  # typeIdsOffset
    
    # Proto, Field, Method, ClassDefs (all empty for now)
    struct.pack_into('<I', dex_data, 0x48, 0)  # protoIdsSize
    struct.pack_into('<I', dex_data, 0x4C, 0)  # protoIdsOffset
    struct.pack_into('<I', dex_data, 0x50, 0)  # fieldIdsSize
    struct.pack_into('<I', dex_data, 0x54, 0)  # fieldIdsOffset
    struct.pack_into('<I', dex_data, 0x58, 0)  # methodIdsSize
    struct.pack_into('<I', dex_data, 0x5C, 0)  # methodIdsOffset
    struct.pack_into('<I', dex_data, 0x60, 0)  # classDefsSize
    struct.pack_into('<I', dex_data, 0x64, 0)  # classDefsOffset
    
    # Data section
    struct.pack_into('<I', dex_data, 0x68, current_offset - data_offset)  # dataSectionSize
    struct.pack_into('<I', dex_data, 0x6C, data_offset)  # dataSectionOffset
    
    # Zero out checksum and SHA-1
    struct.pack_into('<I', dex_data, checksum_pos, 0)
    
    # Trim to actual size
    return bytes(dex_data[:file_size])

if __name__ == '__main__':
    dex_data = create_minimal_dex()
    with open('test.dex', 'wb') as f:
        f.write(dex_data)
    print(f"Created test.dex ({len(dex_data)} bytes)")
