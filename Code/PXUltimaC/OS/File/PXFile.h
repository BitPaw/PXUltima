#ifndef PXFileINCLUDE
#define PXFileINCLUDE

//---------------------------------------------------------
// Settings
//---------------------------------------------------------
#define FileLineBufferSize 2048
#define PXFileMappingAllow 1
#define PXFileDebugOutput 0
//---------------------------------------------------------

#define PXFileCursorPositionInvalid (void*)-1

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include <Media/PXResource.h>

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Time/PXTime.h>

#if OSUnix

#include <sys/types.h>
//#include <sys/typed.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>

#elif OSWindows

#include <windows.h>
#include <direct.h>

#endif


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


#if 1
#define PXDirectoryIsRootFolder(s) ((s[0] == '.') && (s[1] == '\0'))
#define PXDirectoryIsCurrentFolder(s) (s[0] == '.') && (s[1] == '.') && (s[2] == '\0'))
#define PXDirectoryIsDotFolder(s) (PXDirectoryIsRootFolder(s) || (PXDirectoryIsCurrentFolder(s))
#else

void PXDirectoryIsDotFolder(const char* s)
{
	return (((s[0] == '.') && (s[1] == '\0')) || ((s[0] == '.') && (s[1] == '.') && (s[2] == '\0')));
}

#endif



typedef struct PXFileOpenFromPathInfo_
{
	PXText Text;

	PXSize FileSize;

	PXMemoryAccessMode AccessMode;
	PXMemoryCachingMode MemoryCachingMode;
	PXBool AllowMapping;
	PXBool CreateIfNotExist;
	PXBool AllowOverrideOnCreate;
}
PXFileOpenFromPathInfo;


typedef struct PXFileDataElementType_
{
	void* Adress;
	PXInt32U Type;
}
PXFileDataElementType;


#define PXFileOpenFromPathInfoMakeLoadOneshot(adress, path) \
	(adress)->Text = path; \
	(adress)->FileSize = 0; \
	(adress)->AccessMode = PXMemoryAccessModeReadOnly; \
	(adress)->MemoryCachingMode = PXMemoryCachingModeSequential; \
	(adress)->AllowMapping = PXTrue; \
	(adress)->CreateIfNotExist = PXFalse; \
	(adress)->AllowOverrideOnCreate = PXFalse;



/*
typedef struct FilePath_
{
	wchar_t Path[PXPathSizeMax];
	wchar_t Drive[DriveMaxSize];
	wchar_t Directory[DirectoryMaxSize];
	wchar_t FileName[FileNameMaxSize];
	wchar_t Extension[ExtensionMaxSize];
}
FilePath;*/

PXPublic PXFileFormat PXAPI PXFilePathExtensionDetectTry(const PXText* const filePath);


PXPublic void PXAPI PXFileDataElementTypeInfo
(
	PXFileDataElementType* const pxFileDataElementType,
	PXText* const dataType,
	PXText* const dataContent
);


//---<Utility>---------------------------------------------------------
PXPublic PXBool PXAPI PXFileDoesExist(const PXText* const filePath);
PXPublic PXActionResult PXAPI PXFileRemove(const PXText* const filePath);
PXPublic PXActionResult PXAPI PXFileRename(const PXText* const oldName, const PXText* const newName);

PXPublic PXActionResult PXAPI PXFileCopy(const PXText* const sourceFilePath, const PXText* const destinationFilePath, const PXBool overrideIfExists);

//PXPublic void PXFilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName);



PXPublic void PXAPI PXFilePathRelativeFromFile(const PXFile* const pxFile, const PXText* const targetPath, PXText* const resultPath);


PXPublic void PXAPI PXFilePathSwapFileName(const PXText* const inputPath, PXText* const exportPath, const PXText* const fileName);
PXPublic void PXAPI PXFilePathSwapExtension(const PXText* const inputPath, PXText* const exportPath);

//---------------------------------------------------------------------


PXPublic PXActionResult PXAPI PXFileName(const PXFile* const pxFile, PXText* const fileName);
PXPublic PXBool PXAPI PXFileDirectoryPathExtract(const PXFile* const path, PXFile* const directoryPath);


typedef struct PXFilePathStructure_
{
	PXText Drive;
	PXText Directory;
	PXText FileName;
	PXText Extension;
}
PXFilePathStructure;

PXPublic PXActionResult PXAPI PXFilePathSplitt(const PXText* const fullPath, PXFilePathStructure* const pxFilePathStructure);
PXPublic PXActionResult PXAPI PXFilePathCombine(const PXText* const fullPath, PXFilePathStructure* const pxFilePathStructure);




PXPublic PXSize PXAPI PXFilePathExtensionGet(const PXText* const filePath, PXText* const extension);




PXPublic PXBool PXAPI PXFileCanDirectAccess(const PXFile* const pxFile);

//---------------------------------------------------------------------
PXPublic void PXAPI PXFileConstruct(PXFile* const pxFile);
PXPublic void PXAPI PXFileDestruct(PXFile* const pxFile);
//---------------------------------------------------------------------

//---<Convert>---------------------------------------------------------
PXPrivate PXInt32U PXAPI PXFileMemoryCachingModeConvertToID(const PXMemoryCachingMode pxMemoryCachingMode);
//---------------------------------------------------------------------

//---<Set>-------------------------------------------------------------
PXPublic void PXAPI PXFileBufferAllocate(PXFile* const pxFile, const PXSize dataSize);
PXPublic void PXAPI PXFileBufferExternal(PXFile* const pxFile, void* const data, const PXSize dataSize);
//---------------------------------------------------------------------

//---<Open>------------------------------------------------------------
PXPublic PXActionResult PXAPI PXFileOpenFromPath(PXFile* const pxFile, const PXFileOpenFromPathInfo* const pxFileOpenFromPathInfo);
PXPublic PXActionResult PXAPI PXFileOpenTemporal(PXFile* const pxFile, const PXSize expectedFileSize);
//---------------------------------------------------------------------

//---<Close>-----------------------------------------------------------
PXPublic PXActionResult PXAPI PXFileClose(PXFile* const pxFile);
//---------------------------------------------------------------------

//---<Mapping>---------------------------------------------------------
PXPublic PXActionResult PXAPI PXFileMapToMemory(PXFile* const pxFile, const PXSize size, const PXMemoryAccessMode protectionMode);
PXPublic PXActionResult PXAPI PXFileUnmapFromMemory(PXFile* const pxFile);
//---------------------------------------------------------------------

//---<Parsing Utility>-----------------------------------------------------
PXPublic PXSize PXAPI PXFileRemainingSize(const PXFile* const pxFile);
PXPublic PXSize PXAPI PXFileRemainingSizeRelativeFromAddress(const PXFile* const pxFile, const void* const adress);
PXPublic PXBool PXAPI PXFileIsAtEnd(const PXFile* const pxFile);

// Seeks over the file, does not change the cursor.
// Returns offset of end.
PXPublic PXSize PXAPI PXFileFindEndOfText(PXFile* const pxFile);

PXPublic void* PXAPI PXFileCursorPosition(PXFile* const pxFile);
PXPublic void PXAPI PXFileCursorMoveTo(PXFile* const pxFile, const PXSize position);
PXPublic void PXAPI PXFileCursorToBeginning(PXFile* const pxFile);
PXPublic PXSize PXAPI PXFileCursorPeek(PXFile* const pxFile, void* const data, const PXSize steps);
PXPublic PXSize PXAPI PXFileCursorAdvance(PXFile* const pxFile, const PXSize steps);
PXPublic void PXAPI PXFileCursorRewind(PXFile* const pxFile, const PXSize steps);
PXPublic void PXAPI PXFileCursorToEnd(PXFile* const pxFile);
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic PXSize PXAPI PXFileSkipEndOfLineCharacters(PXFile* const  PXFile);
PXPublic PXSize PXAPI PXFileSkipEmptySpace(PXFile* const pxFile);

PXPublic PXSize PXAPI PXFileReadNextLineInto(PXFile* const pxFile, void* exportBuffer, const PXSize exportBufferSize);


// Skips current block thats seperated with spaces.
// Example : '|' is the cursor.
// Before : |AAAA  BBBB CCC
// After :  AAAA |BBBB CCC
PXPublic PXSize PXAPI PXFileSkipBlock(PXFile* const pxFile);
PXPublic PXSize PXAPI PXFileSkipToNextBlock(PXFile* const pxFile);
PXPublic PXSize PXAPI PXFileSkipLine(PXFile* const pxFile);

// Set the current position as the end of this file.
// Use this after you have written to a file to declare the end at this exact posiition.
// Curent position will be set to 0 and the max position will be the current position.
PXPublic PXSize PXAPI PXFileCursorPositionTerminate(PXFile* const pxFile);

PXPublic PXSize PXAPI PXFileDataCopy(PXFile* const pxInputStream, PXFile* const pxOutputStream, const PXSize length);

PXPublic PXSize PXAPI PXFileReadTextIU8(PXFile* const pxFile, PXInt8U* const number);
PXPublic PXSize PXAPI PXFileReadTextI(PXFile* const pxFile, int* const number);

PXPublic PXSize PXAPI PXFileReadIXXE(PXFile* const pxFile, void* const valueAdress, const PXSize valueSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadIXXVE(PXFile* const pxFile, void** const valueList, const PXSize valueListSize, const PXSize valueSizeSingle, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadI8S(PXFile* const pxFile, PXInt8S* const value);
PXPublic PXSize PXAPI PXFileReadI8SV(PXFile* const pxFile, PXInt8S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI8U(PXFile* const pxFile, PXInt8U* const value);
PXPublic PXSize PXAPI PXFileReadI8UV(PXFile* const pxFile, PXInt8U* const valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileReadI16S(PXFile* const pxFile, PXInt16S* const value);
PXPublic PXSize PXAPI PXFileReadI16SV(PXFile* const pxFile, PXInt16S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI16SE(PXFile* const pxFile, PXInt16S* const value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI16SVE(PXFile* const pxFile, PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI16U(PXFile* const pxFile, PXInt16U* const value);
PXPublic PXSize PXAPI PXFileReadI16UV(PXFile* const pxFile, PXInt16U* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI16UE(PXFile* const pxFile, PXInt16U* const value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI16UVE(PXFile* const pxFile, PXInt16U** const valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadI32S(PXFile* const pxFile, PXInt32S* const value);
PXPublic PXSize PXAPI PXFileReadI32SV(PXFile* const pxFile, PXInt32S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI32SE(PXFile* const pxFile, PXInt32S* const value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI32SVE(PXFile* const pxFile, PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI32U(PXFile* const pxFile, PXInt32U* const value);
PXPublic PXSize PXAPI PXFileReadI32UV(PXFile* const pxFile, PXInt32U* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI32UE(PXFile* const pxFile, PXInt32U* const value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI32UVE(PXFile* const pxFile, PXInt32U** const valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadI64S(PXFile* const pxFile, PXInt64S* const value);
PXPublic PXSize PXAPI PXFileReadI64SV(PXFile* const pxFile, PXInt64S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI64SE(PXFile* const pxFile, PXInt64S* const value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI64VE(PXFile* const pxFile, PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI64U(PXFile* const pxFile, PXInt64U* const value);
PXPublic PXSize PXAPI PXFileReadI64UV(PXFile* const pxFile, PXInt64U* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileReadI64UE(PXFile* const pxFile, PXInt64U* const value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileReadI64UVE(PXFile* const pxFile, PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileReadF(PXFile* const pxFile, float* const value);
PXPublic PXSize PXAPI PXFileReadFV(PXFile* const pxFile, float* const valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileReadD(PXFile* const pxFile, double* const value);
PXPublic PXSize PXAPI PXFileReadDV(PXFile* const pxFile, double* const valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileReadMultible(PXFile* const pxFile, const PXFileDataElementType* const pxFileElementList, const PXSize pxFileElementListFullSize);

typedef PXSize(PXAPI* PXFileIOMultibleFunction)(PXFile* const pxFile, void* const value, const PXSize length);

PXPublic PXSize PXAPI PXFileIOMultible(PXFile* const pxFile, const PXFileDataElementType* const pxFileElementList, const PXSize pxFileElementListFullSize, PXFileIOMultibleFunction pxFileIOMultibleFunction);

// Read x amounts of bytes and write into a buffer.
// If there is no output buffer given, the cursor will just skip forward.
PXPublic PXSize PXAPI PXFileReadB(PXFile* const pxFile, void* const value, const PXSize length);

// read Text
PXPublic PXSize PXAPI PXFileReadTextA(PXFile* const pxFile, PXTextASCII value, const PXSize length);
PXPublic PXSize PXAPI PXFileReadTextW(PXFile* const pxFile, PXTextUNICODE value, const PXSize length);
PXPublic PXSize PXAPI PXFileReadTextU(PXFile* const pxFile, PXTextUNICODE value, const PXSize length);

PXPublic void PXAPI PXFileReadUntil(PXFile* const pxFile, void* value, const PXSize length, const char character);


// This function swaps the first and secound byte around
// Some formats are designed that way
// Takes in the file and will open up a new fake-one and swaps all things
PXPublic PXSize PXAPI PXFileByteSwap(PXFile* const pxFileTarget, PXFile* const pxFileSource);


// Additional check how many bytes are read.
// Slower version of Read(), this function can't fail.
//PXSize ReadSafe(Byte__* value, const PXSize length);

PXPublic PXBool PXAPI PXFileReadAndCompareI64U(PXFile* const pxFile, const PXInt64U value);


PXPublic PXBool PXAPI PXFileReadAndCompare(PXFile* const pxFile, const void* value, const PXSize length);
PXPublic PXBool PXAPI PXFileReadAndCompareV(PXFile* const pxFile, const void** const value, const PXSize* const valueElementSizeList, const PXSize valueLength);
//unsigned char PXFileReadAndCompareC(PXFile* PXFile, const char value);
//unsigned char PXFileReadAndCompareIU(PXFile* PXFile, const unsigned int value);

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic PXSize PXAPI PXFileWriteI8S(PXFile* const pxFile, const PXInt8S value);
PXPublic PXSize PXAPI PXFileWriteI8SV(PXFile* const pxFile, const PXInt8S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI8U(PXFile* const pxFile, const PXInt8U value);
PXPublic PXSize PXAPI PXFileWriteAtI8U(PXFile* const pxFile, const PXInt8U value, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI8UV(PXFile* const pxFile, const PXInt8U* const valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileWriteI16S(PXFile* const pxFile, const PXInt16S value);
PXPublic PXSize PXAPI PXFileWriteI16SV(PXFile* const pxFile, const PXInt16S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI16SE(PXFile* const pxFile, const PXInt16S value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI16SVE(PXFile* const pxFile, const PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI16U(PXFile* const pxFile, const PXInt16U value);
PXPublic PXSize PXAPI PXFileWriteAtI16U(PXFile* const pxFile, const PXInt16U value, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI16UV(PXFile* const pxFile, const PXInt16U* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI16UE(PXFile* const pxFile, const PXInt16U value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI16UVE(PXFile* const pxFile, const PXInt16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileWriteI32S(PXFile* const pxFile, const PXInt32S value);
PXPublic PXSize PXAPI PXFileWriteI32SV(PXFile* const pxFile, const PXInt32S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI32SE(PXFile* const pxFile, const PXInt32S value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI32SVE(PXFile* const pxFile, const PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI32U(PXFile* const pxFile, const PXInt32U value);
PXPublic PXSize PXAPI PXFileWriteAtI32U(PXFile* const pxFile, const PXInt32U value, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI32UV(PXFile* const pxFile, const PXInt32U* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI32UE(PXFile* const pxFile, const PXInt32U value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteAtI32UE(PXFile* const pxFile, const PXInt32U value, const PXEndian pxEndian, const PXSize index);
PXPublic PXSize PXAPI PXFileWriteI32UVE(PXFile* const pxFile, const PXInt32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileWriteI64S(PXFile* const pxFile, const PXInt64S value);
PXPublic PXSize PXAPI PXFileWriteI64SV(PXFile* const pxFile, const PXInt64S* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI64SE(PXFile* const pxFile, const PXInt64S value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI64VE(PXFile* const pxFile, const PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI64U(PXFile* const pxFile, const PXInt64U value);
PXPublic PXSize PXAPI PXFileWriteI64UV(PXFile* const pxFile, const PXInt64U* const valueList, const PXSize valueListSize);
PXPublic PXSize PXAPI PXFileWriteI64UE(PXFile* const pxFile, const PXInt64U value, const PXEndian pxEndian);
PXPublic PXSize PXAPI PXFileWriteI64UVE(PXFile* const pxFile, const PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

PXPublic PXSize PXAPI PXFileWriteF(PXFile* const pxFile, const float value);
PXPublic PXSize PXAPI PXFileWriteFV(PXFile* const pxFile, const float* const valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileWriteD(PXFile* const pxFile, const double value);
PXPublic PXSize PXAPI PXFileWriteDV(PXFile* const pxFile, const double* const valueList, const PXSize valueListSize);

PXPublic PXSize PXAPI PXFileWriteB(PXFile* const pxFile, const void* const value, const PXSize length);
PXPublic PXSize PXAPI PXFileWriteAtB(PXFile* const pxFile, const void* const data, const PXSize dataSize, const PXSize index);

PXPublic PXSize PXAPI PXFileWriteMultible(PXFile* const pxFile, const PXFileDataElementType* const pxFileDataElementTypeList, const PXSize pxFileElementListFullSize);

PXPublic PXSize PXAPI PXFileWriteFill(PXFile* const pxFile, const PXByte value, const PXSize length);

PXPublic PXSize PXAPI PXFileWriteNewLine(PXFile* const pxFile);

PXPublic PXSize PXAPI PXFileWriteC(PXFile* const pxFile, const char character);

// Write ASCII string \0 terminated.
PXPublic PXSize PXAPI PXFileWriteA(PXFile* const pxFile, const PXTextASCII text, PXSize textSize);

// Write UNICODE string \0 terminated
PXPublic PXSize PXAPI PXFileWriteW(PXFile* const pxFile, const PXTextUNICODE text, PXSize textSize);

PXPublic PXSize PXAPI PXFileWriteAF(PXFile* const pxFile, const PXTextASCII format, ...);
PXPublic PXSize PXAPI PXFileWriteWF(PXFile* const pxFile, const PXTextUNICODE format, ...);
//-------------------------------------------------------------------------



//---<Extra bit stuff>-----------------------------------------------------
PXPublic PXSize PXAPI PXFileSkipBitsToNextByte(PXFile* const pxFile);
PXPublic PXSize PXAPI PXFileCursorMoveBits(PXFile* const pxFile, const PXSize amountOfBits);
PXPublic PXSize PXAPI PXFileBitsAllign(PXFile* const pxFile);

PXPublic PXSize PXAPI PXFilePeekBits(PXFile* const pxFile, const PXSize amountOfBits);
PXPublic PXSize PXAPI PXFileReadBits(PXFile* const pxFile, const PXSize amountOfBits);

PXPublic PXSize PXAPI PXFileWriteBits(PXFile* const pxFile, const PXSize bitData, const PXSize amountOfBits);
//-------------------------------------------------------------------------

PXPublic PXActionResult PXAPI PXFileTimeGet
(
	const PXFile* const pxFile,
	PXTime* const pxTimeCreation,
	PXTime* const pxTimeAccessLast,
	PXTime* const pxTimeWriteLast
);

PXPublic PXActionResult PXAPI PXFilePathSet(PXFile* const pxFile, const PXText* const filePath);
PXPublic PXActionResult PXAPI PXFilePathGet(const PXFile* const pxFile, PXText* const filePath);
PXPublic PXActionResult PXAPI PXFIlePathGetLong(PXText* const pxTextInput, PXText* const pxTextOutput);

#endif