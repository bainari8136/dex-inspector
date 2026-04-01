#include "DexFile.h"
#include "BinaryReader.h"
#include "Leb128.h"
#include "sections/ProtoIds.h"
#include "sections/MethodIds.h"
#include "sections/FieldIds.h"
#include <sstream>

std::unique_ptr<DexFile> DexFile::parseFile(const std::string& filePath) {
    auto dexFile = std::make_unique<DexFile>();
    dexFile->mReader = std::make_unique<BinaryReader>(filePath);

    // Parse header (first 0x70 bytes)
    const uint8_t* data = dexFile->mReader->getRawData();
    
    // Copy magic
    for (int i = 0; i < 4; ++i) {
        dexFile->mHeader.magic[i] = data[i];
    }
    
    // Copy version
    for (int i = 0; i < 4; ++i) {
        dexFile->mHeader.version[i] = data[4 + i];
    }
    
    // Copy SHA-1
    for (int i = 0; i < 20; ++i) {
        dexFile->mHeader.sha1[i] = data[8 + i];
    }
    
    dexFile->mHeader.checksum = dexFile->mReader->readU32(0x1C);
    dexFile->mHeader.fileSize = dexFile->mReader->readU32(0x20);
    dexFile->mHeader.headerSize = dexFile->mReader->readU32(0x24);
    dexFile->mHeader.endianTag = dexFile->mReader->readU32(0x28);
    dexFile->mHeader.linkSize = dexFile->mReader->readU32(0x2C);
    dexFile->mHeader.linkOffset = dexFile->mReader->readU32(0x30);
    dexFile->mHeader.mapOffset = dexFile->mReader->readU32(0x34);
    dexFile->mHeader.stringIdsSize = dexFile->mReader->readU32(0x38);
    dexFile->mHeader.stringIdsOffset = dexFile->mReader->readU32(0x3C);
    dexFile->mHeader.typeIdsSize = dexFile->mReader->readU32(0x40);
    dexFile->mHeader.typeIdsOffset = dexFile->mReader->readU32(0x44);
    dexFile->mHeader.protoIdsSize = dexFile->mReader->readU32(0x48);
    dexFile->mHeader.protoIdsOffset = dexFile->mReader->readU32(0x4C);
    dexFile->mHeader.fieldIdsSize = dexFile->mReader->readU32(0x50);
    dexFile->mHeader.fieldIdsOffset = dexFile->mReader->readU32(0x54);
    dexFile->mHeader.methodIdsSize = dexFile->mReader->readU32(0x58);
    dexFile->mHeader.methodIdsOffset = dexFile->mReader->readU32(0x5C);
    dexFile->mHeader.classDefsSize = dexFile->mReader->readU32(0x60);
    dexFile->mHeader.classDefsOffset = dexFile->mReader->readU32(0x64);
    dexFile->mHeader.dataSectionSize = dexFile->mReader->readU32(0x68);
    dexFile->mHeader.dataSectionOffset = dexFile->mReader->readU32(0x6C);

    // Load string offsets (string IDs section: array of 32-bit offsets into data section)
    dexFile->mStringOffsets.resize(dexFile->mHeader.stringIdsSize);
    for (uint32_t i = 0; i < dexFile->mHeader.stringIdsSize; ++i) {
        dexFile->mStringOffsets[i] = dexFile->mReader->readU32(
            dexFile->mHeader.stringIdsOffset + i * 4);
    }

    // Load type indices (type IDs section: array of 32-bit string indices)
    dexFile->mTypeIndices.resize(dexFile->mHeader.typeIdsSize);
    for (uint32_t i = 0; i < dexFile->mHeader.typeIdsSize; ++i) {
        dexFile->mTypeIndices[i] = dexFile->mReader->readU32(
            dexFile->mHeader.typeIdsOffset + i * 4);
    }

    // Load proto IDs (Phase 2)
    dexFile->mProtos = ProtoIds::parse(data, dexFile->mHeader.protoIdsOffset, dexFile->mHeader.protoIdsSize);

    // Load field IDs (Phase 2)
    dexFile->mFields = FieldIds::parse(data, dexFile->mHeader.fieldIdsOffset, dexFile->mHeader.fieldIdsSize);

    // Load method IDs (Phase 2)
    dexFile->mMethods = MethodIds::parse(data, dexFile->mHeader.methodIdsOffset, dexFile->mHeader.methodIdsSize);

    // Load class defs (Phase 3)
    dexFile->mClasses = ClassDefs::parse(data, dexFile->mHeader.classDefsOffset, dexFile->mHeader.classDefsSize);

    return dexFile;
}

std::string DexFile::getString(uint32_t stringIdx) const {
    if (stringIdx >= mStringOffsets.size()) {
        throw std::out_of_range("String index out of range");
    }

    uint32_t offset = mStringOffsets[stringIdx];

    // At the offset, there's a ULEB128-encoded string length, followed by the string data.
    size_t bytesRead = 0;
    uint32_t stringLength = Leb128::decodeULEB128(mReader->getRawData(), offset, bytesRead);

    // Read the string data
    offset += bytesRead;
    std::vector<uint8_t> stringBytes = mReader->readBytes(offset, stringLength);

    // Convert to string (null-terminated)
    std::string result(reinterpret_cast<const char*>(stringBytes.data()), stringLength);
    return result;
}

std::string DexFile::getType(uint32_t typeIdx) const {
    if (typeIdx >= mTypeIndices.size()) {
        throw std::out_of_range("Type index out of range");
    }

    uint32_t stringIdx = mTypeIndices[typeIdx];
    return getString(stringIdx);
}

std::vector<uint32_t> DexFile::getAllStringOffsets() const {
    return mStringOffsets;
}

std::vector<uint32_t> DexFile::getAllTypeIndices() const {
    return mTypeIndices;
}

std::vector<ProtoId> DexFile::getAllProtos() const {
    return mProtos;
}

std::vector<FieldId> DexFile::getAllFields() const {
    return mFields;
}

std::vector<MethodId> DexFile::getAllMethods() const {
    return mMethods;
}

const ProtoId& DexFile::getProto(uint32_t protoIdx) const {
    if (protoIdx >= mProtos.size()) {
        throw std::out_of_range("Proto index out of range");
    }
    return mProtos[protoIdx];
}

const FieldId& DexFile::getField(uint32_t fieldIdx) const {
    if (fieldIdx >= mFields.size()) {
        throw std::out_of_range("Field index out of range");
    }
    return mFields[fieldIdx];
}

const MethodId& DexFile::getMethod(uint32_t methodIdx) const {
    if (methodIdx >= mMethods.size()) {
        throw std::out_of_range("Method index out of range");
    }
    return mMethods[methodIdx];
}

std::vector<ClassDef> DexFile::getAllClasses() const {
    return mClasses;
}

const ClassDef& DexFile::getClass(uint32_t classIdx) const {
    if (classIdx >= mClasses.size()) {
        throw std::out_of_range("Class index out of range");
    }
    return mClasses[classIdx];
}

std::string DexFile::getSuperclass(uint32_t classIdx) const {
    if (classIdx >= mClasses.size()) {
        throw std::out_of_range("Class index out of range");
    }

    uint32_t superclassTypeIdx = mClasses[classIdx].superclassIdx;
    
    // NO_INDEX is 0xFFFFFFFF
    if (superclassTypeIdx == 0xFFFFFFFF) {
        return "Ljava/lang/Object;";
    }

    try {
        return getType(superclassTypeIdx);
    } catch (const std::exception&) {
        return "";
    }
}

std::string DexFile::getSourceFile(uint32_t classIdx) const {
    if (classIdx >= mClasses.size()) {
        throw std::out_of_range("Class index out of range");
    }

    uint32_t sourceFileStringIdx = mClasses[classIdx].sourceFileIdx;
    
    // NO_INDEX is 0xFFFFFFFF
    if (sourceFileStringIdx == 0xFFFFFFFF) {
        return "";
    }

    try {
        return getString(sourceFileStringIdx);
    } catch (const std::exception&) {
        return "";
    }
}

ClassData DexFile::getClassData(uint32_t offset) const {
    if (offset == 0) {
        throw std::out_of_range("Class data offset is zero");
    }
    return ClassDataParser::parse(mReader->getRawData(), offset);
}

CodeItem DexFile::getCodeItem(uint32_t offset) const {
    if (offset == 0) {
        throw std::out_of_range("Code item offset is zero");
    }
    return CodeItemParser::parse(mReader->getRawData(), offset);
}

const uint8_t* DexFile::getRawData() const {
    return mReader->getRawData();
}
