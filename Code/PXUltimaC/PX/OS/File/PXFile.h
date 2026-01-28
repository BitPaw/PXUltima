#pragma once

#ifndef PXFileIncluded
#define PXFileIncluded

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Media/PXText.h>
#include <PX/Container/Buffer/PXBuffer.h>

#if OSUnix
#define PXPathSizeMax 260
#define DriveMaxSize 3
#define DirectoryMaxSize 256
#define FileNameMaxSize 256
#define ExtensionMaxSize 256
#elif OSWindows
#define PXPathSizeMax 260 // _MAX_PATH
#define DriveMaxSize 3 //_MAX_DRIVE
#define DirectoryMaxSize 256//_MAX_DIR
#define FileNameMaxSize 256 //_MAX_FNAME
#define ExtensionMaxSize 256 //_MAX_EXT
#endif


typedef enum PXFileElementInfoType_
{
    PXFileElementInfoTypeInvalid,
    PXFileElementInfoTypeUnkown, // DT_UNKNOWN

    PXFileElementInfoTypeFile, // DT_REG
    PXFileElementInfoTypeDictionary, // DT_DIR

    PXFileElementInfoTypeNamedPipeFIFO, // DT_FIFO
    PXFileElementInfoTypeLink, // DT_LNK
    PXFileElementInfoTypeSocket, // DT_SOCK

    PXFileElementInfoTypeDeviceCharacter, // DT_CHR
    PXFileElementInfoTypeDeviceBlock, // DT_BLK

    PXFileElementInfoTypeDictionaryRoot, // '.'
    PXFileElementInfoTypeDictionaryParent // '..'
}
PXFileElementInfoType;


typedef enum PXFileLocationMode_
{
    PXFileLocationModeInvalid,
    PXFileLocationModeInternal, // Memory is handled internally.
    PXFileLocationModeExternal, // Memory is stored outside this object
    PXFileLocationModeMappedVirtual, // Used 'VirtalAlloc()' / 'mmap()'
    PXFileLocationModeMappedFromDisk, // Used 'FileView()' / 'fmap()'
    PXFileLocationModeDirectCached, // Read & Write operations are cached into a buffer first.
    PXFileLocationModeDirectUncached // Read & Write operations are directly put into
}
PXFileLocationMode;

PXPublic const char* PXFileLocationModeToString(const PXFileLocationMode pxFileLocationMode);




// Permentant data that is and will be stored on disk.
typedef struct PXFileEntry_
{
    PXI32U ID;

    PXText FilePath;

    PXSize Size;
    PXI8U Depth;

    PXFileElementInfoType Type;
}
PXFileEntry;


#define PXFileIOInfoFileMask                (0b0001111)
#define PXFileIOInfoFilePhysical            (1<<0) // Is file a physical file on a disk?
#define PXFileIOInfoFileVirtual             (1<<1) // Is this file virtual, that is not stored permanently?
#define PXFileIOInfoFileTemp                (1<<2) // Is this file a temp-file that destroyed on close?
#define PXFileIOInfoFileMemory              (1<<3) // is this file in memory only?

#define PXFileIOInfoAllowMapping            (1<<4)
#define PXFileIOInfoCreateIfNotExist        (1<<5)
#define PXFileIOInfoAllowOverrideOnCreate   (1<<6)

// File IO info, how a file needs to be created or opened
typedef struct PXFileOpenInfo_
{
    PXText FilePath;  // Path to the file we want to use

    PXAccessMode AccessMode;
    PXMemoryCachingMode MemoryCachingMode;
    PXI32U FlagList;

    // if we write a file, we can preallocate a size. If not this value is ignored
    PXSize FileSizeRequest;

    PXBuffer Data;
}
PXFileOpenInfo;


typedef struct PXFile_ PXFile;


PXPublic PXBool PXAPI PXDirectoryIsRootFolder(const char* directory);
PXPublic PXBool PXAPI PXDirectoryIsCurrentFolder(const char* directory);
PXPublic PXBool PXAPI PXDirectoryIsDotFolder(const char* directory);


PXPublic void PXAPI PXTypeEntryInfo
(
    PXTypeEntry PXREF pxFileDataElementType,
    PXText PXREF dataType,
    PXText PXREF dataContent
);


//---<Utility>---------------------------------------------------------
PXPublic PXFile* PXAPI PXFileCreate(void);
PXPublic PXBool PXAPI PXFileRelese(PXFile PXREF pxFile);

PXPublic PXBuffer* PXAPI PXFileBufferGET(PXFile PXREF pxFile);


PXPublic PXBool PXAPI PXFileDoesExist(const PXText PXREF pxText);
PXPublic PXBool PXAPI PXFileDoesExistA(const char PXREF filePath);
PXPublic PXResult PXAPI PXFileRemove(const PXText PXREF filePath);
PXPublic PXResult PXAPI PXFileRename(const PXText PXREF oldName, const PXText PXREF newName);

PXPublic PXResult PXAPI PXFileCopy(const PXText PXREF sourceFilePath, const PXText PXREF destinationFilePath, const PXBool overrideIfExists);

//PXPublic void PXFilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName);


PXPublic void PXAPI PXFilePathRelativeFromFileA
(
    const char PXREF pathCurrent,
    const PXSize patHCurrentSize,
    const char PXREF pathTarget,
    const PXSize pathTargetSize,
    char PXREF pathResult,
    PXSize PXREF pathResultSize
);

PXPublic PXResult PXAPI PXFilePathRelativeFromFile(const PXFile PXREF pxFile, const PXText PXREF targetPath, PXText PXREF resultPath);


PXPublic PXResult PXAPI PXFilePathSwapFileName(const PXText PXREF inputPath, PXText PXREF exportPath, const PXText PXREF fileName);
PXPublic void PXAPI PXFilePathSwapExtension(const PXText PXREF inputPath, PXText PXREF exportPath);

//---------------------------------------------------------------------

// Check is there is data to be read. 
// Use when we check if a file is loaded and not at the end
PXPublic PXBool PXAPI PXFileDataAvailable(const PXFile PXREF pxFile);


PXPublic PXResult PXAPI PXFileName(const PXFile PXREF pxFile, PXText PXREF fileName);
PXPublic PXResult PXAPI PXFileNameSet(const PXFile PXREF pxFile, PXText PXREF fileName);


PXPublic PXBool PXAPI PXFileDirectoryPathExtract(const PXText PXREF path, PXText PXREF directoryPath);


typedef struct PXFilePathStructure_
{
    PXText Drive;
    PXText Directory;
    PXText FileName;
    PXText Extension;
}
PXFilePathStructure;

PXPublic PXResult PXAPI PXFilePathSplitt(const PXText PXREF fullPath, PXFilePathStructure PXREF pxFilePathStructure);
PXPublic PXResult PXAPI PXFilePathCombine(PXText PXREF fullPath, PXFilePathStructure PXREF pxFilePathStructure);



PXPublic PXBool PXAPI PXFilePathExtensionCompare(const PXText PXREF filePath, PXText PXREF extension);
PXPublic PXSize PXAPI PXFilePathExtensionGet(const PXText PXREF filePath, PXText PXREF extension);




PXPublic PXBool PXAPI PXFileCanDirectAccess(const PXFile PXREF pxFile);

//---<Binding>---------------------------------------------------------
// Searches and tryes to read a binding eqiviland to the key. 
// Expected format is "Key=Value".
// Given type will be parsed, converted and written into given adress.
PXPublic PXBool PXAPI PXFileKeyValueFetch(PXFile PXREF pxFile, PXTypeBinding PXREF pxTypeBinding);
//---------------------------------------------------------------------

//---<Convert>---------------------------------------------------------
PXPrivate PXI32U PXAPI PXFileMemoryCachingModeConvertToID(const PXMemoryCachingMode pxMemoryCachingMode);
//---------------------------------------------------------------------

//---<I/O>-------------------------------------------------------------
PXPublic PXResult PXAPI PXFileOpen(PXFile PXREF pxFile, PXFileOpenInfo PXREF pxFileIOInfo);
PXPublic PXResult PXAPI PXFileClose(PXFile PXREF pxFile);
//---------------------------------------------------------------------

//---<Mapping>---------------------------------------------------------
PXPublic PXResult PXAPI PXFileMapToMemory(PXFile PXREF pxFile, const PXSize size, const PXAccessMode protectionMode);
PXPublic PXResult PXAPI PXFileUnmapFromMemory(PXFile PXREF pxFile);
//---------------------------------------------------------------------

//---<Parsing Utility>-----------------------------------------------------
PXPublic PXSize PXAPI PXFileAllocatedSize(const PXFile PXREF pxFile);
PXPublic PXSize PXAPI PXFileRemainingSize(const PXFile PXREF pxFile);
PXPublic PXSize PXAPI PXFileRemainingSizeRelativeFromAddress(const PXFile PXREF pxFile, const void PXREF adress);
PXPublic PXBool PXAPI PXFileIsAtEnd(const PXFile PXREF pxFile);

// Check if we have enough size, if not allocate space
PXPublic PXBool PXAPI PXFileAssureFreeSize(PXFile PXREF pxFile, const PXSize amount);


// Seeks over the file, does not change the cursor.
// Returns offset of end.
PXPublic PXSize PXAPI PXFileFindEndOfText(PXFile PXREF pxFile);

PXPublic void* PXAPI PXFileDataAtCursor(PXFile PXREF pxFile);
PXPublic void* PXAPI PXFileDataAtCursorWithOffset(PXFile PXREF pxFile, const PXSize offset);

PXPublic PXSize PXAPI PXFileDataPosition(PXFile PXREF pxFile);

PXPublic PXSize PXAPI PXFileSizeToRead(PXFile PXREF pxFile);

PXPublic PXSize PXAPI PXFileOperationsRead(PXFile PXREF pxFile);
PXPublic PXSize PXAPI PXFileOperationsWrite(PXFile PXREF pxFile);

PXPublic PXSize PXAPI PXFileCursorOffsetGetBit(PXFile PXREF pxFile);
PXPublic void PXAPI PXFileCursorOffsetSetBit(PXFile PXREF pxFile, const PXSize offsetBit);

PXPublic void PXAPI PXFileCursorMoveTo(PXFile PXREF pxFile, const PXSize position);
PXPublic void PXAPI PXFileCursorToBeginning(PXFile PXREF pxFile);
PXPublic PXSize PXAPI PXFileCursorPeek(PXFile PXREF pxFile, void PXREF data, const PXSize steps);
PXPublic PXSize PXAPI PXFileCursorAdvance(PXFile PXREF pxFile, const PXSize steps);
PXPublic PXBool PXAPI PXFileCursorIsAt(PXFile PXREF pxFile, const PXSize position);
PXPublic PXSize PXAPI PXFileCursorRewind(PXFile PXREF pxFile, const PXSize steps);
PXPublic PXSize PXAPI PXFileCursorOffset(PXFile PXREF pxFile, const PXI32S offset);
PXPublic void PXAPI PXFileCursorToEnd(PXFile PXREF pxFile);
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic PXSize PXAPI PXFileSkipEndOfLineCharacters(PXFile PXREF  PXFile);
PXPublic PXSize PXAPI PXFileSkipEmptySpace(PXFile PXREF pxFile);

PXPublic PXSize PXAPI PXFileReadNextLineInto(PXFile PXREF pxFile, void* exportBuffer, const PXSize exportBufferSize);


// Skips current block thats seperated with spaces.
// Example : '|' is the cursor.
// Before : |AAAA  BBBB CCC
// After :  AAAA |BBBB CCC
PXPublic PXSize PXAPI PXFileSkipBlock(PXFile PXREF pxFile);
PXPublic PXSize PXAPI PXFileSkipToNextBlock(PXFile PXREF pxFile);
PXPublic PXSize PXAPI PXFileSkipLine(PXFile PXREF pxFile);

// Set the current position as the end of this file.
// Use this after you have written to a file to declare the end at this exact posiition.
// Curent position will be set to 0 and the max position will be the current position.
PXPublic PXSize PXAPI PXFileCursorPositionTerminate(PXFile PXREF pxFile);

PXPublic PXSize PXAPI PXFileDataCopy(PXFile PXREF pxInputStream, PXFile PXREF pxOutputStream, const PXSize length);

PXPublic PXSize PXAPI PXFileReadTextIU8(PXFile PXREF pxFile, PXI8U PXREF number);
PXPublic PXSize PXAPI PXFileReadTextI(PXFile PXREF pxFile, int PXREF number);

PXPublic PXSize PXAPI PXFileReadType(PXFile PXREF pxFile, void PXREF valueAdress, const PXI32U type);

PXPublic PXSize PXAPI PXFileReadIXXE(PXFile PXREF pxFile, void PXREF valueAdress, const PXSize valueSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadIXXVE(PXFile PXREF pxFile, void* PXREF valueList, const PXSize valueListSize, const PXSize valueSizeSingle, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadI8S(PXFile PXREF pxFile, PXI8S PXREF value);
PXPublic PXSize PXAPI PXFileReadI8SV(PXFile PXREF pxFile, PXI8S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI8U(PXFile PXREF pxFile, PXI8U PXREF value);
PXPublic PXSize PXAPI PXFileReadI8UV(PXFile PXREF pxFile, PXI8U PXREF valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileReadI16S(PXFile PXREF pxFile, PXI16S PXREF value);
PXPublic PXSize PXAPI PXFileReadI16SV(PXFile PXREF pxFile, PXI16S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI16SE(PXFile PXREF pxFile, PXI16S PXREF value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI16SVE(PXFile PXREF pxFile, PXI16S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI16U(PXFile PXREF pxFile, PXI16U PXREF value);
PXPublic PXSize PXAPI PXFileReadI16UV(PXFile PXREF pxFile, PXI16U PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI16UE(PXFile PXREF pxFile, PXI16U PXREF value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI16UVE(PXFile PXREF pxFile, PXI16U* PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadI32S(PXFile PXREF pxFile, PXI32S PXREF value);
PXPublic PXSize PXAPI PXFileReadI32SV(PXFile PXREF pxFile, PXI32S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI32SE(PXFile PXREF pxFile, PXI32S PXREF value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI32SVE(PXFile PXREF pxFile, PXI32S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI32U(PXFile PXREF pxFile, PXI32U PXREF value);
PXPublic PXSize PXAPI PXFileReadI32UV(PXFile PXREF pxFile, PXI32U PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI32UE(PXFile PXREF pxFile, PXI32U PXREF value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI32UVE(PXFile PXREF pxFile, PXI32U* PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadI64S(PXFile PXREF pxFile, PXI64S PXREF value);
PXPublic PXSize PXAPI PXFileReadI64SV(PXFile PXREF pxFile, PXI64S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI64SE(PXFile PXREF pxFile, PXI64S PXREF value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI64VE(PXFile PXREF pxFile, PXI64S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI64U(PXFile PXREF pxFile, PXI64U PXREF value);
PXPublic PXSize PXAPI PXFileReadI64UV(PXFile PXREF pxFile, PXI64U PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI64UE(PXFile PXREF pxFile, PXI64U PXREF value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI64UVE(PXFile PXREF pxFile, PXI64U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadF(PXFile PXREF pxFile, PXF32 PXREF value);
PXPublic PXSize PXAPI PXFileReadFV(PXFile PXREF pxFile, PXF32 PXREF valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileReadD(PXFile PXREF pxFile, double PXREF value);
PXPublic PXSize PXAPI PXFileReadDV(PXFile PXREF pxFile, double PXREF valueList, const PXSize valueListSize);


#define PXFileBindingIOMask 0b0011
#define PXFileBindingRead   0b0000
#define PXFileBindingWrite  0b0001

#define PXFileBindingAllignToByte  0b0100

// Read or write a defined binding.
// Requires a list of types and a struct of the data that is to be used.
// The data list and struct have to be the same!
PXPublic PXSize PXAPI PXFileBinding(PXFile PXREF pxFile, void PXREF dataStruct, const PXI32U* listOfTypes, const PXSize listOfTypesAmount, const PXI8U flags);

PXPublic PXSize PXAPI PXFileDataWidthCalculate(PXFile PXREF pxFile, const PXI32U type);

// Read a batch of values. Improved fetch speed
PXPublic PXSize PXAPI PXFileReadMultible(PXFile PXREF pxFile, const PXTypeEntry PXREF pxFileElementList, const PXSize pxFileElementListFullSize);

typedef PXSize(PXAPI* PXFileIOMultibleFunction)(PXFile PXREF pxFile, void PXREF value, const PXSize length);

PXPublic PXSize PXAPI PXFileIOMultible(PXFile PXREF pxFile, const PXTypeEntry PXREF pxFileElementList, const PXSize pxFileElementListFullSize, PXFileIOMultibleFunction pxFileIOMultibleFunction);

// Read x amounts of bytes and write into a buffer.
// If there is no output buffer given, the cursor will just skip forward.
PXPublic PXSize PXAPI PXFileReadB(PXFile PXREF pxFile, void PXREF value, const PXSize length);

// read Text
PXPublic PXSize PXAPI PXFileReadA(PXFile PXREF pxFile, PXASCII* value, const PXSize length);
PXPublic PXSize PXAPI PXFileReadW(PXFile PXREF pxFile, PXUNICODE* value, const PXSize length);
PXPublic PXSize PXAPI PXFileTextU(PXFile PXREF pxFile, PXUTF8* value, const PXSize length);

PXPublic void PXAPI PXFileReadUntil(PXFile PXREF pxFile, void* value, const PXSize length, const char character);


// This function swaps the first and secound byte around
// Some formats are designed that way
// Takes in the file and will open up a new fake-one and swaps all things
PXPublic PXSize PXAPI PXFileByteSwap(PXFile PXREF pxFileTarget, PXFile PXREF pxFileSource);


// Additional check how many bytes are read.
// Slower version of Read(), this function can't fail.
//PXSize ReadSafe(Byte__* value, const PXSize length);

PXPublic PXBool PXAPI PXFileReadAndCompareI64U(PXFile PXREF pxFile, const PXI64U value);


PXPublic PXBool PXAPI PXFileReadAndCompare(PXFile PXREF pxFile, const void* value, const PXSize length);
PXPublic PXBool PXAPI PXFileReadAndCompareV(PXFile PXREF pxFile, const void* PXREF value, const PXSize PXREF valueElementSizeList, const PXSize valueLength);
//unsigned char PXFileReadAndCompareC(PXFile* PXFile, const char value);
//unsigned char PXFileReadAndCompareIU(PXFile* PXFile, const unsigned int value);

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic PXSize PXAPI PXFileWriteI8S(PXFile PXREF pxFile, const PXI8S value);
PXPublic PXSize PXAPI PXFileWriteI8SV(PXFile PXREF pxFile, const PXI8S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI8U(PXFile PXREF pxFile, const PXI8U value);
PXPublic PXSize PXAPI PXFileWriteAtI8U(PXFile PXREF pxFile, const PXI8U value, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI8UV(PXFile PXREF pxFile, const PXI8U PXREF valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileWriteI16S(PXFile PXREF pxFile, const PXI16S value);
PXPublic PXSize PXAPI PXFileWriteI16SV(PXFile PXREF pxFile, const PXI16S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI16SE(PXFile PXREF pxFile, const PXI16S value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI16SVE(PXFile PXREF pxFile, const PXI16S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI16U(PXFile PXREF pxFile, const PXI16U value);
PXPublic PXSize PXAPI PXFileWriteAtI16U(PXFile PXREF pxFile, const PXI16U value, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI16UV(PXFile PXREF pxFile, const PXI16U PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI16UE(PXFile PXREF pxFile, const PXI16U value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI16UVE(PXFile PXREF pxFile, const PXI16U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileWriteI32S(PXFile PXREF pxFile, const PXI32S value);
PXPublic PXSize PXAPI PXFileWriteI32SV(PXFile PXREF pxFile, const PXI32S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI32SE(PXFile PXREF pxFile, const PXI32S value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI32SVE(PXFile PXREF pxFile, const PXI32S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI32U(PXFile PXREF pxFile, const PXI32U value);
PXPublic PXSize PXAPI PXFileWriteAtI32U(PXFile PXREF pxFile, const PXI32U value, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI32UV(PXFile PXREF pxFile, const PXI32U PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI32UE(PXFile PXREF pxFile, const PXI32U value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteAtI32UE(PXFile PXREF pxFile, const PXI32U value, const PXEndian pxEndian, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI32UVE(PXFile PXREF pxFile, const PXI32U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileWriteI64S(PXFile PXREF pxFile, const PXI64S value);
PXPublic PXSize PXAPI PXFileWriteI64SV(PXFile PXREF pxFile, const PXI64S PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI64SE(PXFile PXREF pxFile, const PXI64S value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI64VE(PXFile PXREF pxFile, const PXI64S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI64U(PXFile PXREF pxFile, const PXI64U value);
PXPublic PXSize PXAPI PXFileWriteI64UV(PXFile PXREF pxFile, const PXI64U PXREF valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI64UE(PXFile PXREF pxFile, const PXI64U value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI64UVE(PXFile PXREF pxFile, const PXI64U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileWriteF(PXFile PXREF pxFile, const PXF32 value);
PXPublic PXSize PXAPI PXFileWriteFV(PXFile PXREF pxFile, const PXF32 PXREF valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileWriteD(PXFile PXREF pxFile, const double value);
PXPublic PXSize PXAPI PXFileWriteDV(PXFile PXREF pxFile, const double PXREF valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileWriteInternMove(PXFile PXREF pxFile, const PXI16S offset, const PXSize length);

PXPublic PXSize PXAPI PXFileWriteB(PXFile PXREF pxFile, const void PXREF value, const PXSize length);
PXPublic PXSize PXAPI PXFileWriteAtB(PXFile PXREF pxFile, const void PXREF data, const PXSize dataSize, const PXSize index);

PXPublic PXSize PXAPI PXFileWriteMultible(PXFile PXREF pxFile, const PXTypeEntry PXREF pxFileDataElementTypeList, const PXSize pxFileElementListFullSize);

PXPublic PXSize PXAPI PXFileWriteFill(PXFile PXREF pxFile, const PXByte value, const PXSize length);

PXPublic PXSize PXAPI PXFileWriteNewLine(PXFile PXREF pxFile);

PXPublic PXSize PXAPI PXFileWriteC(PXFile PXREF pxFile, const char character);

PXPublic PXSize PXAPI PXFileWriteText(PXFile PXREF pxFile, PXText PXREF pxText);

// Write ASCII string \0 terminated.
PXPublic PXSize PXAPI PXFileWriteA(PXFile PXREF pxFile, const char PXREF text, const PXSize textSize);

// Write UNICODE string \0 terminated
PXPublic PXSize PXAPI PXFileWriteW(PXFile PXREF pxFile, const wchar_t PXREF text, const PXSize textSize);

PXPublic PXSize PXAPI PXFileWriteAF(PXFile PXREF pxFile, const char PXREF format, ...);
PXPublic PXSize PXAPI PXFileWriteWF(PXFile PXREF pxFile, const wchar_t PXREF format, ...);

// If a file is mapped, we can store 
PXPublic PXResult PXAPI PXFileStoreOnDiskA(PXFile PXREF pxFile, const char* fileName);

//-------------------------------------------------------------------------


PXPublic PXResult PXAPI PXFileMapToMemoryEE(PXFile PXREF pxFile, const PXSize requestedSize, const PXAccessMode pxAccessMode, const PXBool prefetch);



//---<Extra bit stuff>-----------------------------------------------------
PXPublic PXSize PXAPI PXFileSkipBitsToNextByte(PXFile PXREF pxFile);
PXPublic PXSize PXAPI PXFileCursorMoveBits(PXFile PXREF pxFile, const PXSize amountOfBits);
PXPublic PXSize PXAPI PXFileBitsAllign(PXFile PXREF pxFile);

PXPublic PXSize PXAPI PXFilePeekBits(PXFile PXREF pxFile, const PXSize amountOfBits);
PXPublic PXSize PXAPI PXFileReadBits(PXFile PXREF pxFile, const PXSize amountOfBits);

PXPublic PXSize PXAPI PXFileWriteBits(PXFile PXREF pxFile, const PXSize bitData, const PXSize amountOfBits);
//-------------------------------------------------------------------------

PXPublic PXResult PXAPI PXFileTimeGet
(
    const PXFile PXREF pxFile,
    PXTime PXREF pxTimeCreation,
    PXTime PXREF pxTimeAccessLast,
    PXTime PXREF pxTimeWriteLast
);

PXPublic PXResult PXAPI PXFilePath(const PXFile PXREF pxFile, PXText PXREF filePath, const PXBool doWrite);
PXPublic PXResult PXAPI PXFilePathA(PXFile PXREF pxFile, char PXREF filePath, const PXSize filePathSize, PXSize PXREF sizeWritten);
PXPublic PXResult PXAPI PXFilePathLong(PXText PXREF pxTextInput, PXText PXREF pxTextOutput);


PXPublic PXResult PXAPI PXFileNameViaHandle(const PXFile PXREF pxFile, PXText PXREF pxText);
PXPublic void PXAPI PXFileEndianessSet(PXFile PXREF pxFile, const PXEndian pxEndian);
PXPublic void PXAPI PXFileBitFormatOfDataSet(PXFile PXREF pxFile, const PXBitFormat pxBitFormat);

// Percentage
PXPublic PXSize PXAPI PXFileDataUtilized(const PXFile PXREF pxFile);

#endif
