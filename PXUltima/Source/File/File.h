#ifndef FileINCLUDE
#define FileINCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>

#include <Format/Type.h>
#include <OS/OSVersion.h>
#include <Memory/PXMemory.h>
#include <Error/ActionResult.h>
#include <File/Endian.h>

#if OSUnix

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>

typedef FILE* FileHandleType;
typedef int FileMappingID;

#elif OSWindows

#include <windows.h>
#include <direct.h>

typedef HANDLE FileHandleType;
typedef HANDLE FileMappingID;

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

	typedef enum FileFormatExtension_
	{
		FileFormatInvalid,
		FileFormatUnkown,

		FileFormatA3DS,
		FileFormatAAC,
		FileFormatAVI,
		FileFormatBitMap,
		FileFormatCSS,
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

	PXPublic FileFormatExtension FilePathExtensionDetectTryA(const char* const filePath, const PXSize filePathSize);
	PXPublic FileFormatExtension FilePathExtensionDetectTryW(const wchar_t* const filePath, const PXSize filePathSize);

	PXPublic FileFormatExtension FileExtensionDetectTryA(const char* const extension, const PXSize extensionSize);
	PXPublic FileFormatExtension FileExtensionDetectTryW(const wchar_t* const extension, const PXSize extensionSize);



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
	PXPublic unsigned char FileDoesExistA(const char* filePath);
	PXPublic unsigned char FileDoesExistW(const wchar_t* filePath);

	PXPublic ActionResult FileRemoveA(const char* filePath);
	PXPublic ActionResult FileRemoveW(const wchar_t* filePath);
	PXPublic ActionResult FileRenameA(const char* oldName, const char* newName);
	PXPublic ActionResult FileRenameW(const wchar_t* oldName, const wchar_t* newName);
	PXPublic ActionResult FileCopyA(const char* sourceFilePath, const char* destinationFilePath);
	PXPublic ActionResult FileCopyW(const wchar_t* sourceFilePath, const wchar_t* destinationFilePath);

	PXPublic void FilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName);
	PXPublic void FilePathSwapFileNameW(const wchar_t* const inputPath, wchar_t* const exportPath, const wchar_t* const fileName);
	PXPublic void FilePathSwapExtensionW(const wchar_t* const inputPath, wchar_t* const exportPath, const wchar_t* const fileExtension);
	//---------------------------------------------------------------------

	//---<Directory>-------------------------------------------------------
	PXPublic ActionResult DirectoryCreateA(const char* directoryName);
	PXPublic ActionResult DirectoryCreateW(const wchar_t* directoryName);
	PXPublic ActionResult WorkingDirectoryChange(const char* directoryName);
	PXPublic ActionResult WorkingDirectoryGetA(char* workingDirectory, PXSize workingDirectorySize);
	PXPublic ActionResult WorkingDirectoryGetW(wchar_t* workingDirectory, PXSize workingDirectorySize);
	PXPublic ActionResult WorkingDirectoryChangeW(const wchar_t* directoryName);
	PXPublic ActionResult DirectoryDeleteA(const char* directoryName);
	PXPublic ActionResult DirectoryDeleteW(const wchar_t* directoryName);
	PXPublic ActionResult DirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize);
	PXPublic ActionResult DirectoryFilesInFolderW(const wchar_t* folderPath, wchar_t*** list, PXSize* listSize);
	//---------------------------------------------------------------------

	PXPublic void FilePathSplittA
	(
		const char* fullPath, PXSize fullPathMaxSize,
		char* drive, PXSize driveMaxSize,
		char* directory, PXSize directoryMaxSize,
		char* fileName, PXSize fileNameMaxSize,
		char* extension, PXSize extensionMaxSize
	);

	PXPublic void FilePathSplittW
	(
		const wchar_t* fullPath, PXSize fullPathMaxSize,
		wchar_t* drive, PXSize driveMaxSize,
		wchar_t* directory, PXSize directoryMaxSize,
		wchar_t* fileName, PXSize fileNameMaxSize,
		wchar_t* extension, PXSize extensionMaxSize
	);

	PXPublic void FilePathSplittPositionW
	(
		const wchar_t* fullPath, PXSize fullPathMaxSize,
		PXSize* drivePos, PXSize driveSize,
		PXSize* directory, PXSize directorySize,
		PXSize* fileName, PXSize fileNameSize,
		PXSize* extension, PXSize extensionSize
	);

	PXPublic PXSize FilePathExtensionGetA(const char* filePath, const PXSize filePathSize, char* extension, const PXSize extensionSizeMax);
	PXPublic PXSize FilePathExtensionGetW(const wchar_t* filePath, const PXSize filePathSize, wchar_t* extension, const PXSize extensionSizeMax);

	PXPublic void FileExtensionDetect();

#ifdef __cplusplus
}
#endif

#endif
