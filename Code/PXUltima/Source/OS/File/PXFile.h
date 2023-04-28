#ifndef PXFileINCLUDE
#define PXFileINCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include <Media/PXType.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Error/PXActionResult.h>

#if OSUnix

#include <sys/typed.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>

#elif OSWindows

#include <windows.h>
#include <direct.h>

#endif

#define FileLineBufferSize 2048

#if OSUnix
#define PathMaxSize 260
#define DriveMaxSize 3
#define DirectoryMaxSize 256
#define FileNameMaxSize 256
#define ExtensionMaxSize 256
#elif OSWindows
#define PathMaxSize 260 // _MAX_PATH
#define DriveMaxSize 3 //_MAX_DRIVE
#define DirectoryMaxSize 256//_MAX_DIR
#define FileNameMaxSize 256 //_MAX_FNAME
#define ExtensionMaxSize 256 //_MAX_EXT
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if 1

#define PXDirectoryIsDotFolder(s) (((s[0] == '.') && (s[1] == '\0')) || ((s[0] == '.') && (s[1] == '.') && (s[2] == '\0')))
#else

void PXDirectoryIsDotFolder(const char* s)
{
    return (((s[0] == '.') && (s[1] == '\0')) || ((s[0] == '.') && (s[1] == '.') && (s[2] == '\0')));
}

#endif


	typedef enum FileFormatExtension_
	{
		FileFormatInvalid,
		FileFormatUnkown,

		FileFormatA3DS,
		FileFormatAAC,
		FileFormatAVI,
		FileFormatBitMap,
		FileFormatC,
		FileFormatCSharp,
		FileFormatCSS,
		FileFormatCPP,
		FileFormatWindowsDynamicLinkedLibrary,
		FileFormatLinuxExecutableAndLinkable,
		FileFormatEML,
		FileFormatWindowsExecutable,
		FileFormatFimBox,
		FileFormatFLAC,
		FileFormatSpriteFont,
		FileFormatGIF,
		FileForPXMathTML,
		FileFormatINI,
		FileFormatJPEG,
		FileFormatJSON,
		FileFormatM4A,
		FileFormatMIDI,
		FileFormatMP3,
		FileFormatMP4,
		FileFormatMSI,
		FileFormatMTL,
		FileFormatOBJ,
		FileFormatOGG,
		FileFormatPDF,
		FileFormatPHP,
		FileFormatPLY,
		FileFormatPNG,
		FileFormatQOI,
		FileFormatSTEP,
		FileFormatSTL,
		FileFormatSVG,
		FileFormatTGA,
		FileFormatTagImage,
		FileFormatTrueTypeFont,
		FileFormatVRML,
		FileFormatWave,
		FileFormatWEBM,
		FileFormatWEBP,
		FileFormatWMA,
		FileFormatXML,
		FileFormatYAML
	}
	FileFormatExtension;

	PXPublic FileFormatExtension PXFilePathExtensionDetectTryA(const char* const filePath, const PXSize filePathSize);
	PXPublic FileFormatExtension PXFilePathExtensionDetectTryW(const wchar_t* const filePath, const PXSize filePathSize);

	PXPublic FileFormatExtension PXFileExtensionDetectTryA(const char* const extension, const PXSize extensionSize);
	PXPublic FileFormatExtension PXFileExtensionDetectTryW(const wchar_t* const extension, const PXSize extensionSize);






	typedef enum PXFileLocationMode_
	{
		PXFileLocationModeInvalid,
		PXFileLocationModeInternal, // Memory is handled internally.
		PXFileLocationModeExternal, // Memory is stored outside this object
		PXFileLocationModeMappedVirtual, // Used 'VirtalAlloc()' / 'mmap()'
		PXFileLocationModeMappedFromDisk, // Used 'FileView()' / 'fmap()'
		PXFileLocationModeCachedFromDisk, // When the file is cached into an CPrivate buffer
		PXFileLocationModeLinked // Used existing file with 'fopen()'
	}
	PXFileLocationMode;

	typedef struct PXFileDataElementType_
	{
		PXDataType Type;
		void* Adress;
	}
	PXFileDataElementType;

	typedef struct PXFile_
	{
		//---<PosisionData>---
		void* Data; // [Do not use directly] Data from where to read/write depending on the used method of linking.
		PXSize DataCursor; // [Do not use directly] Current position of the data.
		PXSize DataCursorBitOffset; // [Do not use directly] Current offset in bits of current byte
		PXSize DataSize; // [Do not use directly] The total size of the data block.
		PXSize DataAllocated; // [Do not use directly] The size of the data pace in which you can move without triggering an invalid access.
		//--------------------

		PXMemoryAccessMode AccessMode;
		PXMemoryCachingMode CachingMode;
		PXFileLocationMode LocationMode;

#if OSUnix
		FILE* ID;
		int MappingID;
#elif OSWindows
		HANDLE ID;
		HANDLE MappingID;
#endif
	}
	PXFile;








	/*
	typedef struct FilePath_
	{
		wchar_t Path[PathMaxSize];
		wchar_t Drive[DriveMaxSize];
		wchar_t Directory[DirectoryMaxSize];
		wchar_t FileName[FileNameMaxSize];
		wchar_t Extension[ExtensionMaxSize];
	}
	FilePath;*/

	//---<Utility>---------------------------------------------------------
	PXPublic PXBool PXFileDoesExistA(const char* filePath);
	PXPublic PXBool PXFileDoesExistW(const wchar_t* filePath);

	PXPublic PXActionResult PXFileRemoveA(const char* filePath);
	PXPublic PXActionResult PXFileRemoveW(const wchar_t* filePath);
	PXPublic PXActionResult PXFileRenameA(const char* oldName, const char* newName);
	PXPublic PXActionResult PXFileRenameW(const wchar_t* oldName, const wchar_t* newName);
	PXPublic PXActionResult PXFileCopyA(const char* sourceFilePath, const char* destinationFilePath);
	PXPublic PXActionResult PXFileCopyW(const wchar_t* sourceFilePath, const wchar_t* destinationFilePath);

	PXPublic void PXFilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName);
	PXPublic void PXFilePathSwapFileNameA(const PXTextASCII inputPath, PXTextASCII exportPath, const PXTextASCII fileName);
	PXPublic void PXFilePathSwapFileNameW(const PXTextUNICODE inputPath, PXTextUNICODE exportPath, const PXTextUNICODE fileName);
	PXPublic void PXFilePathSwapFileNameU(const PXTextUTF8 inputPath, PXTextUTF8 exportPath, const PXTextUTF8 fileName);

	PXPublic void PXFilePathSwapExtensionA(const PXTextASCII inputPath, PXTextASCII exportPath, const PXTextASCII fileExtension);
	PXPublic void PXFilePathSwapExtensionW(const PXTextUNICODE inputPath, PXTextUNICODE exportPath, const PXTextUNICODE fileExtension);
	PXPublic void PXFilePathSwapExtensionU(const PXTextUTF8 inputPath, PXTextUTF8 exportPath, const PXTextUTF8 fileExtension);
	//---------------------------------------------------------------------





	PXPublic PXActionResult PXFileNameA(PXFile* const pxFile, const PXTextASCII fileName, const PXSize fileNameSize, PXSize* const sizeWritten);
	PXPublic PXActionResult PXFileNameW(PXFile* const pxFile, const PXTextUNICODE fileName, const PXSize fileNameSize, PXSize* const sizeWritten);

	PXPublic PXSize PXFileDirectoryPathExtract(const wchar_t* path, const PXSize pathSize, wchar_t* const directoryPath, const PXSize directoryPathSize);

	PXPublic void PXFilePathSplittA
	(
		const char* fullPath, PXSize fullPathMaxSize,
		char* drive, PXSize driveMaxSize,
		char* directory, PXSize directoryMaxSize,
		char* fileName, PXSize fileNameMaxSize,
		char* extension, PXSize extensionMaxSize
	);

	PXPublic void PXFilePathSplittW
	(
		const wchar_t* fullPath, PXSize fullPathMaxSize,
		wchar_t* drive, PXSize driveMaxSize,
		wchar_t* directory, PXSize directoryMaxSize,
		wchar_t* fileName, PXSize fileNameMaxSize,
		wchar_t* extension, PXSize extensionMaxSize
	);

	PXPublic void PXFilePathSplittPositionW
	(
		const wchar_t* fullPath, PXSize fullPathMaxSize,
		PXSize* drivePos, PXSize driveSize,
		PXSize* directory, PXSize directorySize,
		PXSize* fileName, PXSize fileNameSize,
		PXSize* extension, PXSize extensionSize
	);

	PXPublic PXSize PXFilePathExtensionGetA(const char* filePath, const PXSize filePathSize, char* extension, const PXSize extensionSizeMax);
	PXPublic PXSize PXFilePathExtensionGetW(const wchar_t* filePath, const PXSize filePathSize, wchar_t* extension, const PXSize extensionSizeMax);






	//---------------------------------------------------------------------
	PXPublic void PXFileConstruct(PXFile* const pxFile);
	PXPublic void PXFileDestruct(PXFile* const pxFile);
	//---------------------------------------------------------------------

	//---<Convert>---------------------------------------------------------
	PXPrivate PXInt32U PXFileMemoryCachingModeConvertToID(const PXMemoryCachingMode pxMemoryCachingMode);
	//---------------------------------------------------------------------

	//---<Set>-------------------------------------------------------------
	PXPublic void PXFileBufferAllocate(PXFile* const pxFile, const PXSize dataSize);
	PXPublic void PXFileBufferExternal(PXFile* const pxFile, void* const data, const PXSize dataSize);
	//---------------------------------------------------------------------

	//---<Open>------------------------------------------------------------
	PXPublic PXActionResult PXFileOpenFromPathA(PXFile* const pxFile, const PXTextASCII filePath, const PXMemoryAccessMode pxAccessMode, const PXMemoryCachingMode pxMemoryCachingMode);
	PXPublic PXActionResult PXFileOpenFromPathW(PXFile* const pxFile, const PXTextUNICODE filePath, const PXMemoryAccessMode pxAccessMode, const PXMemoryCachingMode pxMemoryCachingMode);
	PXPublic PXActionResult PXFileOpenFromPathU(PXFile* const pxFile, const PXTextUTF8 filePath, const PXMemoryAccessMode pxAccessMode, const PXMemoryCachingMode pxMemoryCachingMode);
	//---------------------------------------------------------------------

	//---<Close>-----------------------------------------------------------
	PXPublic PXActionResult PXFileClose(PXFile* const pxFile);
	//---------------------------------------------------------------------

	//---<Mapping>---------------------------------------------------------
	PXPublic PXActionResult PXFileMapToMemoryA(PXFile* const pxFile, const PXTextASCII filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode);
	PXPublic PXActionResult PXFileMapToMemoryW(PXFile* const pxFile, const PXTextUNICODE filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode);
	PXPublic PXActionResult PXFileMapToMemoryU(PXFile* const pxFile, const PXTextUTF8 filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode);
	PXPublic PXActionResult PXFileMapToMemory(PXFile* const pxFile, const PXSize size, const PXMemoryAccessMode protectionMode);
	PXPublic PXActionResult PXFileUnmapFromMemory(PXFile* const pxFile);
	//---------------------------------------------------------------------

	//---<Parsing Utility>-----------------------------------------------------
	PXPublic PXSize PXFileRemainingSize(const PXFile* PXRestrict const dataStream);
	PXPublic PXSize PXFileRemainingSizeRelativeFromAddress(const PXFile* PXRestrict const dataStream, const void* const adress);
	PXPublic PXBool PXFileIsAtEnd(const PXFile* PXRestrict const dataStream);

	PXPublic void* PXFileCursorPosition(PXFile* const pxFile);
	PXPublic void PXFileCursorMoveTo(PXFile* const pxFile, const PXSize position);
	PXPublic void PXFileCursorToBeginning(PXFile* const pxFile);
	PXPublic PXSize PXFileCursorAdvance(PXFile* const pxFile, const PXSize steps);
	PXPublic void PXFileCursorRewind(PXFile* const pxFile, const PXSize steps);
	PXPublic void PXFileCursorToEnd(PXFile* const pxFile);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic PXSize PXFileSkipEndOfLineCharacters(PXFile* const  PXFile);
	PXPublic PXSize PXFileSkipEmptySpace(PXFile* const pxFile);

	PXPublic PXSize PXFileReadNextLineInto(PXFile* const pxFile, void* exportBuffer, const PXSize exportBufferSize);


	// Skips current block thats seperated with spaces.
	// Example : '|' is the cursor.
	// Before : |AAAA  BBBB CCC
	// After :  AAAA |BBBB CCC
	PXPublic PXSize PXFileSkipBlock(PXFile* const pxFile);
	PXPublic PXSize PXFileSkipToNextBlock(PXFile* const pxFile);
	PXPublic PXSize PXFileSkipLine(PXFile* const pxFile);


	PXPublic PXSize PXFileDataCopy(PXFile* const pxInputStream, PXFile* const pxOutputStream, const PXSize length);

	PXPublic PXSize PXFileReadTextIU8(PXFile* const pxFile, PXInt8U* const number);
	PXPublic PXSize PXFileReadTextI(PXFile* const pxFile, int* const number);


	PXPublic PXSize PXFileReadI8S(PXFile* const pxFile, PXInt8S* const value);
	PXPublic PXSize PXFileReadI8SV(PXFile* const pxFile, PXInt8S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileReadI8U(PXFile* const pxFile, PXInt8U* const value);
	PXPublic PXSize PXFileReadI8UV(PXFile* const pxFile, PXInt8U* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXFileReadI16S(PXFile* const pxFile, PXInt16S* const value);
	PXPublic PXSize PXFileReadI16SV(PXFile* const pxFile, PXInt16S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileReadI16SE(PXFile* const pxFile, PXInt16S* const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI16SVE(PXFile* const pxFile, PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI16U(PXFile* const pxFile, PXInt16U* const value);
	PXPublic PXSize PXFileReadI16UV(PXFile* const pxFile, PXInt16U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileReadI16UE(PXFile* const pxFile, PXInt16U* const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI16UVE(PXFile* const pxFile, PXInt16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

	PXPublic PXSize PXFileReadI32S(PXFile* const pxFile, PXInt32S* const value);
	PXPublic PXSize PXFileReadI32SV(PXFile* const pxFile, PXInt32S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileReadI32SE(PXFile* const pxFile, PXInt32S* const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI32SVE(PXFile* const pxFile, PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI32U(PXFile* const pxFile, PXInt32U* const value);
	PXPublic PXSize PXFileReadI32UV(PXFile* const pxFile, PXInt32U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileReadI32UE(PXFile* const pxFile, PXInt32U* const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI32UVE(PXFile* const pxFile, PXInt32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

	PXPublic PXSize PXFileReadI64S(PXFile* const pxFile, PXInt64S* const value);
	PXPublic PXSize PXFileReadI64SV(PXFile* const pxFile, PXInt64S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileReadI64SE(PXFile* const pxFile, PXInt64S* const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI64VE(PXFile* const pxFile, PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI64U(PXFile* const pxFile, PXInt64U* const value);
	PXPublic PXSize PXFileReadI64UV(PXFile* const pxFile, PXInt64U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileReadI64UE(PXFile* const pxFile, PXInt64U* const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadI64UVE(PXFile* const pxFile, PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

	PXPublic PXSize PXFileReadF(PXFile* const pxFile, float* const value);
	PXPublic PXSize PXFileReadFV(PXFile* const pxFile, float* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXFileReadD(PXFile* const pxFile, double* const value);
	PXPublic PXSize PXFileReadDV(PXFile* const pxFile, double* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXFileReadMultible(PXFile* const pxFile, PXFileDataElementType* const pxFileElementList, const PXSize pxFileElementListSize);

	PXPublic PXSize PXFileReadB(PXFile* const pxFile, void* const value, const PXSize length);

	// read Text
	PXPublic PXSize PXFileReadTextA(PXFile* const pxFile, PXTextASCII value, const PXSize length);
	PXPublic PXSize PXFileReadTextW(PXFile* const pxFile, PXTextUNICODE value, const PXSize length);
	PXPublic PXSize PXFileReadTextU(PXFile* const pxFile, PXTextUNICODE value, const PXSize length);

	PXPublic void PXFileReadUntil(PXFile* const pxFile, void* value, const PXSize length, const char character);



	// Additional check how many bytes are read.
	// Slower version of Read(), this function can't fail.
	//PXSize ReadSafe(Byte__* value, const PXSize length);

	PXPublic PXBool PXFileReadAndCompareI64U(PXFile* const pxFile, const PXInt64U value);


	PXPublic PXBool PXFileReadAndCompare(PXFile* const pxFile, const void* value, const PXSize length);
	PXPublic PXBool PXFileReadAndCompareV(PXFile* const pxFile, const void** value, const PXSize* valueElementSizeList, const PXSize valueLength);
	//unsigned char PXFileReadAndCompareC(PXFile* PXFile, const char value);
	//unsigned char PXFileReadAndCompareIU(PXFile* PXFile, const unsigned int value);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic PXSize PXFileWriteI8S(PXFile* const pxFile, const PXInt8S value);
	PXPublic PXSize PXFileWriteI8SV(PXFile* const pxFile, const PXInt8S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileWriteI8U(PXFile* const pxFile, const PXInt8U value);
	PXPublic PXSize PXFileWriteAtI8U(PXFile* const pxFile, const PXInt8U value, const PXSize index);
	PXPublic PXSize PXFileWriteI8UV(PXFile* const pxFile, const PXInt8U* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXFileWriteI16S(PXFile* const pxFile, const PXInt16S const value);
	PXPublic PXSize PXFileWriteI16SV(PXFile* const pxFile, const PXInt16S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileWriteI16SE(PXFile* const pxFile, const PXInt16S const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI16SVE(PXFile* const pxFile, const PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI16U(PXFile* const pxFile, const PXInt16U const value);
	PXPublic PXSize PXFileWriteAtI16U(PXFile* const pxFile, const PXInt16U const value, const PXSize index);
	PXPublic PXSize PXFileWriteI16UV(PXFile* const pxFile, const PXInt16U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileWriteI16UE(PXFile* const pxFile, const PXInt16U const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI16UVE(PXFile* const pxFile, const PXInt16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

	PXPublic PXSize PXFileWriteI32S(PXFile* const pxFile, const PXInt32S value);
	PXPublic PXSize PXFileWriteI32SV(PXFile* const pxFile, const PXInt32S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileWriteI32SE(PXFile* const pxFile, const PXInt32S value, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI32SVE(PXFile* const pxFile, const PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI32U(PXFile* const pxFile, const PXInt32U value);
	PXPublic PXSize PXFileWriteAtI32U(PXFile* const pxFile, const PXInt32U value, const PXSize index);
	PXPublic PXSize PXFileWriteI32UV(PXFile* const pxFile, const PXInt32U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileWriteI32UE(PXFile* const pxFile, const PXInt32U value, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteAtI32UE(PXFile* const pxFile, const PXInt32U value, const PXEndian pxEndian, const PXSize index);
	PXPublic PXSize PXFileWriteI32UVE(PXFile* const pxFile, const PXInt32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

	PXPublic PXSize PXFileWriteI64S(PXFile* const pxFile, const PXInt64S const value);
	PXPublic PXSize PXFileWriteI64SV(PXFile* const pxFile, const PXInt64S* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileWriteI64SE(PXFile* const pxFile, const PXInt64S const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI64VE(PXFile* const pxFile, const PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI64U(PXFile* const pxFile, const PXInt64U const value);
	PXPublic PXSize PXFileWriteI64UV(PXFile* const pxFile, const PXInt64U* const valueList, const PXSize valueListSize);
	PXPublic PXSize PXFileWriteI64UE(PXFile* const pxFile, const PXInt64U const value, const PXEndian pxEndian);
	PXPublic PXSize PXFileWriteI64UVE(PXFile* const pxFile, const PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian);

	PXPublic PXSize PXFileWriteF(PXFile* const pxFile, const float value);
	PXPublic PXSize PXFileWriteFV(PXFile* const pxFile, const float* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXFileWriteD(PXFile* const pxFile, const double value);
	PXPublic PXSize PXFileWriteDV(PXFile* const pxFile, const double* const valueList, const PXSize valueListSize);

	PXPublic PXSize PXFileWriteB(PXFile* const pxFile, const void* const value, const PXSize length);
	PXPublic PXSize PXFileWriteAtB(PXFile* const pxFile, const void* const data, const PXSize dataSize, const PXSize index);

	PXPublic PXSize PXFileWriteMultible(PXFile* const pxFile, const PXFileDataElementType* const PXFileElementList, const PXSize PXFileElementListSize);

	PXPublic PXSize PXFileWriteFill(PXFile* const pxFile, const unsigned char value, const PXSize length);

	// Write ASCII string \0 terminated.
	PXPublic PXSize PXFileWriteA(PXFile* const pxFile, const PXTextASCII text, PXSize textSize);

	// Write UNICODE string \0 terminated
	PXPublic PXSize PXFileWriteW(PXFile* const pxFile, const PXTextUNICODE const text, PXSize textSize);

	PXPublic PXSize PXFileWriteAF(PXFile* const pxFile, const PXTextASCII format, ...);
	PXPublic PXSize PXFileWriteWF(PXFile* const pxFile, const PXTextUNICODE format, ...);
	//-------------------------------------------------------------------------



	//---<Extra bit stuff>-----------------------------------------------------
	PXPublic PXSize PXFileSkipBitsToNextByte(PXFile* const pxFile);
	PXPublic PXSize PXFileCursorMoveBits(PXFile* const pxFile, const PXSize amountOfBits);
	PXPublic PXSize PXFileBitsAllign(PXFile* const pxFile);

	PXPublic PXSize PXFilePeekBits(PXFile* const pxFile, const PXSize amountOfBits);
	PXPublic PXSize PXFileReadBits(PXFile* const pxFile, const PXSize amountOfBits);

	PXPublic PXSize PXFileWriteBits(PXFile* const pxFile, const PXSize bitData, const PXSize amountOfBits);
	//-------------------------------------------------------------------------


	PXPublic PXSize PXFileFilePathGetA(PXFile* const pxFile, char* const filePath, const PXSize filePathMaxSize);
	PXPublic PXSize PXFileFilePathGetW(PXFile* const pxFile, wchar_t* const filePath, const PXSize filePathMaxSize);

#ifdef __cplusplus
}
#endif

#endif