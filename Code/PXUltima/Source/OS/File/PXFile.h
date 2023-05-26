#ifndef PXFileINCLUDE
#define PXFileINCLUDE


#define PXFileUSE OSDeviceToUse == OSDeviceDestop
#if PXFileUSE


#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Error/PXActionResult.h>


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
		FileFormatHTML,
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

	PXPublic FileFormatExtension PXFilePathExtensionDetectTry(const PXText* const filePath);






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
	PXPublic PXBool PXFileDoesExist(const PXText* const filePath);
	PXPublic PXActionResult PXFileRemove(const PXText* const filePath);
	PXPublic PXActionResult PXFileRename(const PXText* const oldName, const PXText* const newName);

	PXPublic PXActionResult PXFileCopy(const PXText* const sourceFilePath, const PXText* const destinationFilePath, const PXBool overrideIfExists);

	//PXPublic void PXFilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName);



	PXPublic void PXFilePathSwapFileName(const PXText* const inputPath, PXText* const exportPath, const PXText* const fileName);
	PXPublic void PXFilePathSwapExtension(const PXText* const inputPath, PXText* const exportPath);

	//---------------------------------------------------------------------


	PXPublic PXActionResult PXFileName(const PXFile* const pxFile, PXText* const fileName);
	PXPublic PXBool PXFileDirectoryPathExtract(const PXFile* const path, PXFile* const directoryPath);


	PXPublic void PXFilePathSplitt
	(
		const PXText* const fullPath,
		PXText* const drive,
		PXText* const directory,
		PXText* const fileName,
		PXText* const extension
	);

	PXPublic void PXFilePathSplittPositionW
	(
		const wchar_t* fullPath, PXSize fullPathMaxSize,
		PXSize* drivePos, PXSize driveSize,
		PXSize* directory, PXSize directorySize,
		PXSize* fileName, PXSize fileNameSize,
		PXSize* extension, PXSize extensionSize
	);

	PXPublic PXSize PXFilePathExtensionGet(const PXText* const filePath, const PXText* const extension);






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
	PXPublic PXActionResult PXFileOpenFromPath(PXFile* const pxFile, const PXFileOpenFromPathInfo* const pxFileOpenFromPathInfo);
	//---------------------------------------------------------------------

	//---<Close>-----------------------------------------------------------
	PXPublic PXActionResult PXFileClose(PXFile* const pxFile);
	//---------------------------------------------------------------------

	//---<Mapping>---------------------------------------------------------
	PXPublic PXActionResult PXFileMapToMemory(PXFile* const pxFile, const PXSize size, const PXMemoryAccessMode protectionMode);
	PXPublic PXActionResult PXFileUnmapFromMemory(PXFile* const pxFile);
	//---------------------------------------------------------------------

	//---<Parsing Utility>-----------------------------------------------------
	PXPublic PXSize PXFileRemainingSize(const PXFile* const dataStream);
	PXPublic PXSize PXFileRemainingSizeRelativeFromAddress(const PXFile* const dataStream, const void* const adress);
	PXPublic PXBool PXFileIsAtEnd(const PXFile* const dataStream);

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

	PXPublic PXSize PXFileReadIXXE(PXFile* const pxFile, void* const valueAdress, const PXSize valueSize, const PXEndian pxEndian);
	PXPublic PXSize PXFileReadIXXVE(PXFile* const pxFile, void* const valueList, const PXSize valueListSize, const PXSize valueSizeSingle, const PXEndian pxEndian);

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


	PXPublic PXBool PXFilePathGet(const PXFile* const pxFile, PXText* const filePath);


#ifdef __cplusplus
}
#endif

#endif
#endif