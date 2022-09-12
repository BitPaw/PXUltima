#ifndef FileINCLUDE
#define FileINCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>

#include <Format/Type.h>
#include <OS/OSVersion.h>
#include <Memory/Memory.h>
#include <Error/ActionResult.h>
#include <File/Endian.h>

#if defined(OSUnix)

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>

#define FileHandleType FILE*
#define FileMappingID int

#elif defined(OSWindows)

#include <windows.h>
#include <direct.h>

#define FileHandleType HANDLE
#define FileMappingID HANDLE

#endif

#define FileLineBufferSize 2048

#if defined(OSUnix)
#define PathMaxSize 260
#define DriveMaxSize 3
#define DirectoryMaxSize 256
#define FileNameMaxSize 256
#define ExtensionMaxSize 256
#elif defined(OSWindows)
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
	CPublic unsigned char FileDoesExistA(const char* filePath);
	CPublic unsigned char FileDoesExistW(const wchar_t* filePath);

	CPublic ActionResult FileRemoveA(const char* filePath);
	CPublic ActionResult FileRemoveW(const wchar_t* filePath);
	CPublic ActionResult FileRenameA(const char* oldName, const char* newName);
	CPublic ActionResult FileRenameW(const wchar_t* oldName, const wchar_t* newName);
	CPublic ActionResult FileCopyA(const char* sourceFilePath, const char* destinationFilePath);
	CPublic ActionResult FileCopyW(const wchar_t* sourceFilePath, const wchar_t* destinationFilePath);

	CPublic void FilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName);
	CPublic void FilePathSwapFileNameW(const wchar_t* const inputPath, wchar_t* const exportPath, const wchar_t* const fileName);
	CPublic void FilePathSwapExtensionW(const wchar_t* const inputPath, wchar_t* const exportPath, const wchar_t* const fileExtension);
	//---------------------------------------------------------------------

	//---<Directory>-------------------------------------------------------
	CPublic ActionResult DirectoryCreateA(const char* directoryName);
	CPublic ActionResult DirectoryCreateW(const wchar_t* directoryName);
	CPublic ActionResult WorkingDirectoryChange(const char* directoryName);
	CPublic ActionResult WorkingDirectoryGetA(char* workingDirectory, size_t workingDirectorySize);
	CPublic ActionResult WorkingDirectoryGetW(wchar_t* workingDirectory, size_t workingDirectorySize);
	CPublic ActionResult WorkingDirectoryChangeW(const wchar_t* directoryName);
	CPublic ActionResult DirectoryDeleteA(const char* directoryName);
	CPublic ActionResult DirectoryDeleteW(const wchar_t* directoryName);
	CPublic ActionResult DirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, size_t* listSize);
	CPublic ActionResult DirectoryFilesInFolderW(const wchar_t* folderPath, wchar_t*** list, size_t* listSize);
	//---------------------------------------------------------------------

	CPublic void FilePathSplittA
	(
		const char* fullPath, size_t fullPathMaxSize,
		char* drive, size_t driveMaxSize,
		char* directory, size_t directoryMaxSize,
		char* fileName, size_t fileNameMaxSize,
		char* extension, size_t extensionMaxSize
	);

	CPublic void FilePathSplittW
	(
		const wchar_t* fullPath, size_t fullPathMaxSize,
		wchar_t* drive, size_t driveMaxSize,
		wchar_t* directory, size_t directoryMaxSize,
		wchar_t* fileName, size_t fileNameMaxSize,
		wchar_t* extension, size_t extensionMaxSize
	);

	CPublic void FilePathExtensionGetA(const char* filePath, const size_t filePathSize, char* extension, const size_t extensionSizeMax);
	CPublic void FilePathExtensionGetW(const wchar_t* filePath, const size_t filePathSize, wchar_t* extension, const size_t extensionSizeMax);

#ifdef __cplusplus
}
#endif

#endif
