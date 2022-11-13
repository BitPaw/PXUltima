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

#define FileHandleType FILE*
#define FileMappingID int

#elif OSWindows

#include <windows.h>
#include <direct.h>

#define FileHandleType HANDLE
#define FileMappingID HANDLE

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

	PXPublic FileFormatExtension FilePathExtensionDetectTryA(const char* const filePath, const size_t filePathSize);
	PXPublic FileFormatExtension FilePathExtensionDetectTryW(const wchar_t* const filePath, const size_t filePathSize);

	PXPublic FileFormatExtension FileExtensionDetectTryA(const char* const extension, const size_t extensionSize);
	PXPublic FileFormatExtension FileExtensionDetectTryW(const wchar_t* const extension, const size_t extensionSize);



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
	PXPublic ActionResult WorkingDirectoryGetA(char* workingDirectory, size_t workingDirectorySize);
	PXPublic ActionResult WorkingDirectoryGetW(wchar_t* workingDirectory, size_t workingDirectorySize);
	PXPublic ActionResult WorkingDirectoryChangeW(const wchar_t* directoryName);
	PXPublic ActionResult DirectoryDeleteA(const char* directoryName);
	PXPublic ActionResult DirectoryDeleteW(const wchar_t* directoryName);
	PXPublic ActionResult DirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, size_t* listSize);
	PXPublic ActionResult DirectoryFilesInFolderW(const wchar_t* folderPath, wchar_t*** list, size_t* listSize);
	//---------------------------------------------------------------------

	PXPublic void FilePathSplittA
	(
		const char* fullPath, size_t fullPathMaxSize,
		char* drive, size_t driveMaxSize,
		char* directory, size_t directoryMaxSize,
		char* fileName, size_t fileNameMaxSize,
		char* extension, size_t extensionMaxSize
	);

	PXPublic void FilePathSplittW
	(
		const wchar_t* fullPath, size_t fullPathMaxSize,
		wchar_t* drive, size_t driveMaxSize,
		wchar_t* directory, size_t directoryMaxSize,
		wchar_t* fileName, size_t fileNameMaxSize,
		wchar_t* extension, size_t extensionMaxSize
	);

	PXPublic void FilePathSplittPositionW
	(
		const wchar_t* fullPath, size_t fullPathMaxSize,
		size_t* drivePos, size_t driveSize,
		size_t* directory, size_t directorySize,
		size_t* fileName, size_t fileNameSize,
		size_t* extension, size_t extensionSize
	);

	PXPublic size_t FilePathExtensionGetA(const char* filePath, const size_t filePathSize, char* extension, const size_t extensionSizeMax);
	PXPublic size_t FilePathExtensionGetW(const wchar_t* filePath, const size_t filePathSize, wchar_t* extension, const size_t extensionSizeMax);

	PXPublic void FileExtensionDetect();

#ifdef __cplusplus
}
#endif

#endif
