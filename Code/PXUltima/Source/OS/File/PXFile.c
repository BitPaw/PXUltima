#include "PXFile.h"

#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

#include <stdarg.h>
#include <fcntl.h>


#if OSUnix
#define PrintSVN vsnprintf
#elif OSWindows
// _O_RDONLY, _O_RDWR, _O_RDWR
#define PrintSVN vsprintf_s
#endif


#define FileReadMode "rb"
#define FileReadModeW L"rb"

#define FileWriteMode "wb"
#define FileWriteModeW L"wb"

#define FileReadWriteMode "rwb"
#define FileReadWriteModeW L"rwb"


#if OSUnix
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//#include <sys/io.h>
#include <dirent.h>

#define OSFileOpenA fopen
#define OSFileOpenW(string, mode) FileOpenA((const char*)string, (const char*)mode)
#define OSFileRemoveA remove
#define OSFileRemoveW(string) remove((const char*)string)
#define OSFileRenameA rename
#define OSFileRenameW(oldName, newName) rename((const char*)oldName, (const char*)newName)
#define OSFileDirectoryCreateA(string) mkdir(string, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define OSFileDirectoryCreateW(string) OSFileDirectoryCreateA((const char*)string)
#define OSWorkingDirectoryCurrentA getcwd
#define OSWorkingDirectoryCurrentW(string, size) (wchar_t*)OSWorkingDirectoryCurrentA((char*)string, size)
#define OSWorkingDirectoryChangeA chdir
#define OSWorkingDirectoryChangeW(string) OSWorkingDirectoryChangeA((const char*)string)

#define PipeOpen popen
#define PipeOpenW(wchar, mode) popen((char*)wchar, (const char*) mode) // TODO: instable
#define PipeClose pclose

#elif OSWindows
#include <io.h>

#define OSFileOpenA fopen
#define OSFileOpenW _wfopen
#define OSFileRemoveA remove
#define OSFileRemoveW _wremove
#define OSFileRenameA rename
#define OSFileRenameW _wrename
#define OSFileDirectoryCreateA _mkdir
#define OSFileDirectoryCreateW _wmkdir
#define OSWorkingDirectoryCurrentA _getcwd
#define OSWorkingDirectoryCurrentW _wgetcwd
#define OSWorkingDirectoryChangeA _chdir
#define OSWorkingDirectoryChangeW _wchdir


#define PipeOpen _popen
#define PipeOpenW _wpopen
#define PipeClose _pclose

// WinNT.h
// FILE_ATTRIBUTE_TEMPORARY

#endif

void PXFilePathSplittA(const char* fullPath, PXSize fullPathMaxSize, char* drive, PXSize driveMaxSize, char* directory, PXSize directoryMaxSize, char* fileName, PXSize fileNameMaxSize, char* extension, PXSize extensionMaxSize)
{
#if OSUnix
	char directoryNameCache[PathMaxSize];
	char baseNameCache[FileNameMaxSize];

	PXTextCopyA(fullPath, FileNameMaxSize, directoryNameCache, FileNameMaxSize);
	PXTextCopyA(fullPath, FileNameMaxSize, baseNameCache, FileNameMaxSize);

	char* dirNameResult = dirname(directoryNameCache);
	char* baseNameResult = basename(baseNameCache);

	PXSize directoryLength = PXTextCopyA(dirNameResult, DirectoryMaxSize, directory, DirectoryMaxSize);
	PXSize fileNameLength = PXTextCopyA(baseNameResult, FileNameMaxSize, fileName, FileNameMaxSize);

	for(PXSize i = fileNameLength - 1; i > 0; --i)
	{
		const unsigned char isDot = fileName[i] == '.';

		if(isDot)
		{
			PXTextCopyA(fileName + i + 1, ExtensionMaxSize - i, extension, extensionMaxSize);
			break;
		}
	}
#elif OSWindows
	char fileNameCache[FileNameMaxSize];

	_splitpath_s
	(
		fullPath,
		drive, driveMaxSize,
		directory, directoryMaxSize,
		fileName, fileNameMaxSize,
		extension, extensionMaxSize
	);

	for(PXSize i = 0; fileName[i] != '\0'; i++)
	{
		const PXBool isDot = fileName[i] == '.';

		if(isDot)
		{
			PXTextCopyA(extension + i, extensionMaxSize, fileNameCache, FileNameMaxSize);
			PXTextCopyA(fileNameCache, FileNameMaxSize, extension, extensionMaxSize);
			break;
		}
	}
#endif
}

void PXFilePathSplittW(const wchar_t* fullPath, PXSize fullPathMaxSize, wchar_t* drive, PXSize driveMaxSize, wchar_t* directory, PXSize directoryMaxSize, wchar_t* fileName, PXSize fileNameMaxSize, wchar_t* extension, PXSize extensionMaxSize)
{
#if OSUnix
	char fullPathA[PathMaxSize];
	char driveA[DriveMaxSize];
	char directoryA[DirectoryMaxSize];
	char fileNameA[FileNameMaxSize];
	char extensionA[ExtensionMaxSize];

	PXTextCopyWA(fullPath, PathMaxSize, fullPathA, PathMaxSize);

	FilePathSplittA
	(
		fullPathA,PathMaxSize,
		driveA,DriveMaxSize,
		directoryA,DirectoryMaxSize,
		fileNameA, FileNameMaxSize,
		extensionA, ExtensionMaxSize
	);

	PXTextCopyAW(driveA, DriveMaxSize, drive, DriveMaxSize);
	PXTextCopyAW(directoryA, DirectoryMaxSize, directory, DirectoryMaxSize);
	PXTextCopyAW(fileNameA, FileNameMaxSize, fileName, FileNameMaxSize);
	PXTextCopyAW(extensionA, ExtensionMaxSize, extension, ExtensionMaxSize);
#elif OSWindows
	wchar_t extensionCache[FileNameMaxSize];

	_wsplitpath_s
	(
		fullPath,
		drive, driveMaxSize,
		directory, directoryMaxSize,
		fileName, fileNameMaxSize,
		extension, extensionMaxSize
	);

	for(PXSize i = 0; extension[i] != '\0'; i++)
	{
		const unsigned char isDot = extension[i] == '.';

		if(isDot)
		{
			PXTextCopyW(extension + i + 1, extensionMaxSize, extensionCache, FileNameMaxSize);
			PXTextCopyW(extensionCache, FileNameMaxSize, extension, extensionMaxSize);
			break;
		}
	}
#endif
}

void PXFilePathSplittPositionW(const wchar_t* fullPath, PXSize fullPathMaxSize, PXSize* drivePos, PXSize driveSize, PXSize* directory, PXSize directorySize, PXSize* fileName, PXSize fileNameSize, PXSize* extension, PXSize extensionSize)
{
}

PXSize PXFilePathExtensionGetA(const char* filePath, const PXSize filePathSize, char* extension, const PXSize extensionSizeMax)
{
	const PXSize index = PXTextFindLastA(filePath, filePathSize, '.');
	const unsigned char hasExtension = index != PXTextIndexNotFound;

	if(!hasExtension)
	{
		PXTextClearA(extension, extensionSizeMax);
		return;
	}

	const PXSize writtenBytes = PXTextCopyA(filePath + index, filePathSize - index, extension, extensionSizeMax);

	return writtenBytes;
}

PXSize PXFilePathExtensionGetW(const wchar_t* filePath, const PXSize filePathSize, wchar_t* extension, const PXSize extensionSizeMax)
{
	const PXSize index = PXTextFindLastW(filePath, filePathSize, '.');
	const unsigned char hasExtension = index != PXTextIndexNotFound;

	if(!hasExtension)
	{
		PXTextClearW(extension, extensionSizeMax);
		return;
	}

	const PXSize writtenBytes = PXTextCopyW(filePath + index + 1, filePathSize - index, extension, extensionSizeMax);

	return writtenBytes;
}

FileFormatExtension PXFilePathExtensionDetectTryA(const char* const filePath, const PXSize filePathSize)
{
	char extensionA[ExtensionMaxSize];

	const PXSize writtenBytes = PXFilePathExtensionGetA(filePath, filePathSize, extensionA, ExtensionMaxSize);

	FileFormatExtension fileFormatExtension = PXFileExtensionDetectTryA(extensionA, writtenBytes);

	return fileFormatExtension;
}

FileFormatExtension PXFilePathExtensionDetectTryW(const wchar_t* const filePath, const PXSize filePathSize)
{
	wchar_t extensionW[ExtensionMaxSize];

	const PXSize writtenBytes = PXFilePathExtensionGetW(filePath, filePathSize, extensionW, ExtensionMaxSize);

	FileFormatExtension fileFormatExtension = PXFileExtensionDetectTryW(extensionW, writtenBytes);

	return fileFormatExtension;
}

FileFormatExtension PXFileExtensionDetectTryA(const char* const extension, const PXSize extensionSize)
{
	wchar_t extensionW[ExtensionMaxSize];

	const PXSize extensionWSize = PXTextCopyAW(extension, extensionSize, extensionW, ExtensionMaxSize);
	const FileFormatExtension format = PXFileExtensionDetectTryW(extensionW, extensionWSize);

	return format;
}

FileFormatExtension PXFileExtensionDetectTryW(const wchar_t* const extension, const PXSize extensionSizeC)
{
	const PXSize extensionSize = PXMathMinimumIU(extensionSizeC, ExtensionMaxSize);

	switch (extensionSize)
	{
		case 0:
			return FileFormatInvalid;

		case 1u:
		{
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "O", 1u))  return FileFormatLinuxExecutableAndLinkable;
			break;
		}
		case 2u:
		{
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "SO", 2u))  return FileFormatLinuxExecutableAndLinkable;
			break;
		}
		case 3u:
		{
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "ELF", 3u))  return FileFormatLinuxExecutableAndLinkable;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "OUT", 3u))  return FileFormatLinuxExecutableAndLinkable;

			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "FNT", 3u))  return FileFormatSpriteFont;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "GIF", 3u))  return FileFormatGIF;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "HTM", 3u))  return FileForPXMathTML;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "INI", 3u))  return FileFormatINI;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "M4A", 3u))  return FileFormatM4A;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "3DS", 3u))  return FileFormatA3DS;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "ACC", 3u))  return FileFormatAAC;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "AVI", 3u))  return FileFormatAVI;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "BMP", 3u))  return FileFormatBitMap;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "CSS", 3u))  return FileFormatCSS;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "DLL", 3u))  return FileFormatWindowsDynamicLinkedLibrary;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "EML", 3u))  return FileFormatEML;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "EXE", 3u))  return FileFormatWindowsExecutable;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "FBX", 3u))  return FileFormatFimBox;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "MP3", 3u))  return FileFormatMP3;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "MP4", 3u))  return FileFormatMP4;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "MSI", 3u))  return FileFormatMSI;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "MTL", 3u))  return FileFormatMTL;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "OBJ", 3u))  return FileFormatOBJ;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "OGG", 3u))  return FileFormatOGG;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "PDF", 3u))  return FileFormatPDF;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "PHP", 3u))  return FileFormatPHP;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "PLY", 3u))  return FileFormatPLY;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "PNG", 3u))  return FileFormatPNG;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "QUI", 3u))  return FileFormatQOI;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "STL", 4u))  return FileFormatSTL;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "SVG", 4u))  return FileFormatSVG;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "TIF", 4u))  return FileFormatTagImage;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "TGA", 4u))  return FileFormatTGA;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "TTF", 4u))  return FileFormatTrueTypeFont;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "WAV", 4u))  return FileFormatWave;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "WMA", 4u))  return FileFormatWMA;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "XML", 3u))  return FileFormatXML;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "YML", 3u))  return FileFormatYAML;

			break;
		}
		case 4u:
		{
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "FLAC", 4u))  return FileFormatFLAC;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "MIDI", 4u))  return FileFormatMIDI;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "STEP", 4u))  return FileFormatSTEP;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "TIFF", 4u))  return FileFormatTagImage;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "JPEG", 4u))  return FileFormatJPEG;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "JSON", 4u))  return FileFormatJSON;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "VRML", 4u))  return FileFormatVRML;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "WEBM", 4u))  return FileFormatWEBM;
			if (PXTextCompareIgnoreCaseWA(extension, extensionSize, "WEBP", 4u))  return FileFormatWEBP;

			break;
		}
	}

	return FileFormatUnkown;
}

unsigned char PXFileDoesExistA(const char* filePath)
{
	return 0;
}

unsigned char PXFileDoesExistW(const wchar_t* filePath)
{
	return 0;
}

PXActionResult PXFileRemoveA(const char* filePath)
{
	int removeResult = OSFileRemoveA(filePath);
	//ErrorCode errorCode = ConvertErrorCode(removeResult);

	return PXActionInvalid;
}

PXActionResult PXFileRemoveW(const wchar_t* filePath)
{
	int removeResult = OSFileRemoveW(filePath);
	//ErrorCode errorCode = ConvertErrorCode(removeResult);

	return PXActionInvalid;
}

PXActionResult PXFileRenameA(const char* oldName, const char* newName)
{
	int renameResult = OSFileRenameA(oldName, newName);
	//ErrorCode errorCode = ConvertErrorCode(renameResult);

	return PXActionInvalid;
}

PXActionResult PXFileRenameW(const wchar_t* oldName, const wchar_t* newName)
{
	int renameResult = OSFileRenameW(oldName, newName);
	const unsigned char wasSuccesful = renameResult == 0;

	if(!wasSuccesful)
	{
		return PXActionInvalid;// GetCurrentError();
	}

	return PXActionSuccessful;
}

PXActionResult PXFileCopyA(const char* sourceFilePath, const char* destinationFilePath)
{
	if (!sourceFilePath) return PXActionRefuedInputBufferNull;
	if (!destinationFilePath) return PXActionRefuedOutputBufferNull;

#if OSUnix
	FILE* fileSource = fopen(sourceFilePath, FileReadMode);
	FILE* fileDestination = fopen(destinationFilePath, FileWriteMode);
	const unsigned char fileOpenSuccesful = fileSource && fileDestination;

	const PXSize swapBufferSize = 2048;
	unsigned char swapBuffer[swapBufferSize];

	if(!fileOpenSuccesful)
	{
		return PXActionFailedFileOpen;
	}

	while(!feof(fileSource))
	{
		PXSize readBytes = fread(swapBuffer, sizeof(char), swapBufferSize, fileSource);
		PXSize writtenBytes = fwrite(swapBuffer, sizeof(char), readBytes, fileDestination);
	}

	fclose(fileSource);
	fclose(fileDestination);
#elif OSWindows
	const PXBool succesfull = CopyFileA(sourceFilePath, destinationFilePath, 0);

	if(!succesfull)
	{
		const PXActionResult resultError = GetCurrentError();

		return resultError;
	}
#endif

	return PXActionSuccessful;
}

PXActionResult PXFileCopyW(const wchar_t* sourceFilePath, const wchar_t* destinationFilePath)
{
	if (!sourceFilePath) return PXActionRefuedInputBufferNull;
	if (!destinationFilePath) return PXActionRefuedOutputBufferNull;

#if OSUnix
	char sourceFilePathA[PathMaxSize];
	char destinationFilePathA[PathMaxSize];

	PXTextCopyWA(sourceFilePath, PathMaxSize, sourceFilePathA, PathMaxSize);
	PXTextCopyWA(destinationFilePath, PathMaxSize, destinationFilePathA, PathMaxSize);

	return FileCopyA(sourceFilePathA, destinationFilePathA);
#elif OSWindows
	const PXBool succesfull = CopyFileW(sourceFilePath, destinationFilePath, 0);

	if (!succesfull)
	{
		const PXActionResult resultError = GetCurrentError();

		return resultError;
	}

	return PXActionSuccessful;
#endif
}

void PXFilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName)
{
	const PXSize index = PXTextFindLastW(currnetPath, PathMaxSize, '/');
	const PXBool found = index != -1;

	if(found)
	{
		const PXSize copyedBytes = PXTextCopyW(currnetPath, index + 1, targetPath, index + 1);
		const PXSize toCopy = PathMaxSize - copyedBytes;

		PXTextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
	}
}

void PXFilePathSwapFileNameA(const PXTextASCII inputPath, PXTextASCII exportPath, const PXTextASCII fileName)
{
	PXCharASCII finalPath[PathMaxSize];

	PXCharASCII driveW[DriveMaxSize];
	PXCharASCII directoryW[DirectoryMaxSize];

	{
		PXCharASCII fileNameW[FileNameMaxSize];
		PXCharASCII extensionW[ExtensionMaxSize];

		PXFilePathSplittA
		(
			inputPath, PathMaxSize,
			driveW, DriveMaxSize,
			directoryW, DirectoryMaxSize,
			fileNameW, FileNameMaxSize,
			extensionW, ExtensionMaxSize
		);
	}

	{
		PXSize offset = 0;

		offset += PXTextCopyA(driveW, DriveMaxSize, finalPath, PathMaxSize - offset);
		offset += PXTextCopyA(directoryW, DirectoryMaxSize, finalPath + offset, PathMaxSize - offset);
		offset += PXTextCopyA(fileName, PathMaxSize, finalPath + offset, PathMaxSize - offset);

		PXTextCopyA(finalPath, offset, exportPath, PathMaxSize);
	}
}

void PXFilePathSwapFileNameW(const PXTextUNICODE inputPath, PXTextUNICODE exportPath, const PXTextUNICODE fileName)
{
	wchar_t finalPath[PathMaxSize];

	wchar_t driveW[DriveMaxSize];
	wchar_t directoryW[DirectoryMaxSize];

	{
		wchar_t fileNameW[FileNameMaxSize];
		wchar_t extensionW[ExtensionMaxSize];

		PXFilePathSplittW
		(
			inputPath, PathMaxSize,
			driveW, DriveMaxSize,
			directoryW, DirectoryMaxSize,
			fileNameW, FileNameMaxSize,
			extensionW, ExtensionMaxSize
		);
	}

	{
		PXSize offset = 0;

		offset += PXTextCopyW(driveW, DriveMaxSize, finalPath, PathMaxSize - offset);
		offset += PXTextCopyW(directoryW, DirectoryMaxSize, finalPath + offset, PathMaxSize - offset);
		offset += PXTextCopyW(fileName, PathMaxSize, finalPath + offset, PathMaxSize - offset);

		PXTextCopyW(finalPath, offset, exportPath, PathMaxSize);
	}
}

void PXFilePathSwapFileNameU(const PXTextUTF8 inputPath, PXTextUTF8 exportPath, const PXTextUTF8 fileName)
{
	PXFilePathSwapFileNameA(inputPath, exportPath, fileName);
}


void PXFilePathSwapExtensionA(const PXTextASCII inputPath, PXTextASCII exportPath, const PXTextASCII fileExtension)
{
	const PXSize index = PXTextFindLastA(inputPath, PathMaxSize, '.'); // Find last dot
	const PXBool found = index != -1;

	if (!found)
	{
		return;
	}

	const PXSize written = PXTextCopyA(inputPath, index, exportPath, PathMaxSize); // Copy filePath without extension
	const PXSize writtenFull = PXTextCopyA(fileExtension, PathMaxSize, &exportPath[written], PathMaxSize); // Copy extension on top
}

void PXFilePathSwapExtensionW(const PXTextUNICODE inputPath, PXTextUNICODE exportPath, const PXTextUNICODE fileExtension)
{
	const PXSize index = PXTextFindLastW(inputPath, PathMaxSize, '.'); // Find last dot
	const PXBool found = index != -1;

	if (!found)
	{
		return;
	}

	const PXSize written = PXTextCopyW(inputPath, index + 1, exportPath, PathMaxSize); // Copy filePath without extension
	const PXSize writtenFull = PXTextCopyW(fileExtension, PathMaxSize, &exportPath[written], PathMaxSize); // Copy extension on top
}

void PXFilePathSwapExtensionU(const PXTextUTF8 inputPath, PXTextUTF8 exportPath, const PXTextUTF8 fileExtension)
{
	PXFilePathSwapExtensionA(inputPath, exportPath, fileExtension);
}

PXActionResult PXFileNameA(PXFile* const pxFile, const PXTextASCII fileName, const PXSize fileNameSize, PXSize* const sizeWritten)
{
#if OSUnix

#elif OSWindows

	const DWORD flags = FILE_NAME_NORMALIZED | VOLUME_NAME_DOS; // FILE_NAME_NORMALIZED

	const DWORD result = GetFinalPathNameByHandleA(pxFile->ID, fileName, fileNameSize, flags);

	const char dosDriveTag[] = "\\\\?\\";

	const PXByte isSS = PXTextCompareA(fileName, 4u, dosDriveTag, 4u);

	if (isSS)
	{
		// Move my atleast 4.

		const char wind[] = "C:\\Windows\\System32\\";

		const PXByte isSSEE = PXTextCompareA(fileName+4u, 20u, wind, 20u);


		char* texxxx = fileName + 4u;
		PXSize xxxxxy = result - 4u;

		if (isSSEE)
		{
			texxxx = fileName + 24u;
			xxxxxy = result - 24u;
		}
		else
		{
			texxxx = fileName + 4u;
			xxxxxy = result - 4u;
		}

		const PXSize rerere = MemoryMove(texxxx, xxxxxy, fileName, result);


		*sizeWritten = rerere;

		fileName[rerere] = 0;

	}
	else
	{
		*sizeWritten = result;
	}

#endif

	return PXActionSuccessful;
}

PXActionResult PXFileNameW(PXFile* const pxFile, const PXTextUNICODE fileName, const PXSize fileNameSize, PXSize* const sizeWritten)
{
#if OSUnix

#elif OSWindows

#endif
}

PXSize PXFileDirectoryPathExtract(const wchar_t* path, const PXSize pathSize, wchar_t* const directoryPath, const PXSize directoryPathSize)
{
	PXSize size = PXTextFindLastW(path, pathSize, '/');
	PXBool found = size != -1;

	if (!found)
	{
		size = PXTextFindLastW(path, pathSize, '\\');
		found = size != -1;

		if (!found)
		{
			directoryPath[0] = '\0';

			return 0;
		}
	}

	return PXTextCopyW(path, size+1, directoryPath, directoryPathSize);
}






void PXFileConstruct(PXFile* const pxFile)
{
	MemoryClear(pxFile, sizeof(PXFile));

	pxFile->ID = -1;
}

void PXFileDestruct(PXFile* const pxFile)
{
	switch (pxFile->LocationMode)
	{
		case PXFileLocationModeMappedFromDisk:
			PXFileUnmapFromMemory(pxFile);
			break;

		case PXFileLocationModeInternal:
			MemoryRelease(pxFile->Data, pxFile->DataSize);
			break;

		case PXFileLocationModeMappedVirtual:
			MemoryVirtualRelease(pxFile->Data, pxFile->DataSize);
			break;

		case PXFileLocationModeCachedFromDisk:
			MemoryRelease(pxFile->Data, pxFile->DataSize);
			break;

		case PXFileLocationModeLinked:
			PXFileClose(pxFile);
			break;
	}
}

PXInt32U PXFileMemoryCachingModeConvertToID(const PXMemoryCachingMode pxMemoryCachingMode)
{
	switch (pxMemoryCachingMode)
	{
		default:
		case PXMemoryCachingModeInvalid:
		case PXMemoryCachingModeDefault:
#if OSUnix
			return POSIX_FADV_NORMAL;
#elif OSWindows
			return 0;
#endif
		case PXMemoryCachingModeRandom:
#if OSUnix
			return POSIX_FADV_RANDOM;
#elif OSWindows
			return FILE_FLAG_RANDOM_ACCESS;
#endif
		case PXMemoryCachingModeSequential:
#if OSUnix
			return POSIX_FADV_SEQUENTIAL;
#elif OSWindows
			return FILE_FLAG_SEQUENTIAL_SCAN;
#endif

		case PXMemoryCachingModeTemporary:
		case PXMemoryCachingModeUseOnce:
			return FILE_FLAG_DELETE_ON_CLOSE;

			// Windows only
		case PXMemoryCachingModeWriteThrough:
			return FILE_FLAG_WRITE_THROUGH;

		case PXMemoryCachingModeNoBuffering:
			return FILE_FLAG_NO_BUFFERING;

#if OSUnix
		// UNIX only
		case PXMemoryCachingModeNeedLater:
			return POSIX_FADV_WILLNEED;

		case PXMemoryCachingModeDontNeedNow:
			return POSIX_FADV_DONTNEED;
#endif	
	}
}

 void PXFileBufferAllocate(PXFile* const pxFile, const PXSize dataSize)
{
	 PXFileConstruct(pxFile);

	 pxFile->Data = MemoryAllocate(dataSize);
	 pxFile->DataAllocated = dataSize;
	 pxFile->DataSize = dataSize;
	 pxFile->AccessMode = PXMemoryAccessModeReadAndWrite;
	 pxFile->LocationMode = PXFileLocationModeInternal;
}

 void PXFileBufferExternal(PXFile* const pxFile, void* const data, const PXSize dataSize)
{
	 PXFileConstruct(pxFile);

	 pxFile->Data = data;
	 pxFile->DataAllocated = dataSize;
	 pxFile->DataSize = dataSize;
	 pxFile->AccessMode = PXMemoryAccessModeReadAndWrite;
	 pxFile->LocationMode = PXFileLocationModeExternal;
}

 PXActionResult PXFileOpenFromPathA(PXFile* const pxFile, const PXTextASCII filePath, const PXMemoryAccessMode pxAccessMode, const PXMemoryCachingMode pxMemoryCachingMode)
{
	 PXByte filePathU[PathMaxSize];

	 PXTextCopyAU(filePath, PathMaxSize, filePathU, PathMaxSize);

	 PXActionResult actionResult = PXFileOpenFromPathU(pxFile, filePathU, pxAccessMode, pxMemoryCachingMode);

	 return actionResult;
}

 PXActionResult PXFileOpenFromPathW(PXFile* const pxFile, const PXTextUNICODE filePath, const PXMemoryAccessMode pxAccessMode, const PXMemoryCachingMode pxMemoryCachingMode)
{
	 PXCharUTF8 filePathU[PathMaxSize];

	 PXTextCopyWU(filePath, PathMaxSize, filePathU, PathMaxSize);

	 PXActionResult actionResult = PXFileOpenFromPathU(pxFile, filePathU, pxAccessMode, pxMemoryCachingMode);

	 return actionResult;
}

 PXActionResult PXFileOpenFromPathU(PXFile* const pxFile, const PXTextUTF8 filePath, const PXMemoryAccessMode pxAccessMode, const PXMemoryCachingMode pxMemoryCachingMode)
{
	 PXFileConstruct(pxFile);

#if OSUnix
	 const char* readMode = 0u;

	 switch (fileOpenMode)
	 {
		 case PXMemoryAccessModeReadOnly:
			 readMode = "rb";
			 break;

		 case PXMemoryAccessModeWriteOnly:
			 readMode = "wb";
			 break;
	 }


	 (readMode != 0);

	 // Use this somewhere here
	 // int posix_fadvise(int fd, off_t offset, off_t len, int advice);
	 // int posix_fadvise64(int fd, off_t offset, off_t len, int advice);

	 pxFile->FileHandle = fopen(filePath, readMode);

	 return pxFile->FileHandle ? PXActionSuccessful : PXActionFailedFileOpen;


#elif OSWindows
	 DWORD dwDesiredAccess = 0;
	 DWORD dwShareMode = 0;
	 //SECURITY_ATTRIBUTES securityAttributes = 0;
	 DWORD dwCreationDisposition = 0;
	 DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	 HANDLE hTemplateFile = 0;

	 switch (pxAccessMode)
	 {
		 case PXMemoryAccessModeReadOnly:
		 {
			 dwShareMode = FILE_SHARE_READ;
			 dwCreationDisposition = OPEN_EXISTING;
			 dwDesiredAccess = GENERIC_READ;
			 break;
		 }
		 case PXMemoryAccessModeWriteOnly:
		 {
			 dwShareMode = FILE_SHARE_WRITE;
			 dwCreationDisposition = CREATE_ALWAYS;
			 dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			 break;
		 }
		 case PXMemoryAccessModeReadAndWrite:
		 {
			 dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
			 dwCreationDisposition = CREATE_ALWAYS;
			 dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			 break;
		 }
	 }

	 dwFlagsAndAttributes |= PXFileMemoryCachingModeConvertToID(pxMemoryCachingMode);


	 // Make directory if needed
	 // FilePathExtensionGetW
	 if (pxAccessMode == PXMemoryAccessModeWriteOnly || pxAccessMode == PXMemoryAccessModeReadAndWrite)
	 {
		 //const PXActionResult directoryCreateResult = DirectoryCreateA(filePath);

		 //PXActionExitOnError(directoryCreateResult);
	 }

	 // UTF
	 {
		 HANDLE const fileHandle = CreateFileA
		 (
			 filePath,
			 dwDesiredAccess,
			 dwShareMode,
			 0,
			 dwCreationDisposition,
			 dwFlagsAndAttributes,
			 hTemplateFile
		 );
		 const PXBool successful = fileHandle != INVALID_HANDLE_VALUE;

		 PXActionOnErrorFetchAndExit(!successful);

		 pxFile->ID = fileHandle;
		 pxFile->AccessMode = pxAccessMode;
		 pxFile->CachingMode = pxMemoryCachingMode;
		 pxFile->LocationMode = PXFileLocationModeLinked;
	 }
#if 0
	 // Get a FILE* from file HANDLE
	 {
		 int osHandleMode = 0;
		 const char* fdOpenMode = 0;

		 switch (fileOpenMode)
		 {
			 case PXMemoryAccessModeReadOnly:
			 {
				 osHandleMode = _O_RDONLY;
				 fdOpenMode = "rb";
				 break;
			 }
			 case  PXMemoryAccessModeWriteOnly:
			 {
				 osHandleMode = _O_RDWR;//_O_WRONLY;
				 fdOpenMode = "wb";
				 break;
			 }
			 case  PXMemoryAccessModeReadAndWrite:
			 {
				 osHandleMode = _O_RDWR;
				 fdOpenMode = "wb";
				 break;
			 }
		 }


		 const int nHandle = _open_osfhandle((intptr_t)pxFile->ID, osHandleMode);
		 const PXBool sucessful = nHandle != -1;

		 if (sucessful)
		 {
			 FILE* fp = _fdopen(nHandle, fdOpenMode);
			 const PXBool result = fp;

			 if (result)
			 {
				 pxFile->FileHandleCStyle = fp;
			 }
			 else
			 {
				 // handle?
			 }
		 }
		 else
		 {
			 // handle?
		 }

	 }
#endif

	 return PXActionSuccessful;
#endif
}

 PXActionResult PXFileClose(PXFile* const pxFile)
{
#if OSUnix
	 const int closeResult = fclose(pxFile->FileHandle);

	 switch (closeResult)
	 {
		 case 0:
			 return PXActionSuccessful;

		 default:
			 return PXActionFailedFileClose;
	 }
#elif OSWindows

#if 0
	 if (pxFile->FileHandleCStyle)
	 {
		 _fclose_nolock(pxFile->FileHandleCStyle);

		 pxFile->FileHandleCStyle = PXNull;
		 pxFile->FileHandle = PXNull;
	 }
#endif

	 if (pxFile->ID != -1)
	 {
		 const PXBool successful = CloseHandle(pxFile->ID);

		 if (!successful)
		 {
			 return PXActionFailedFileClose;
		 }

		 pxFile->ID = -1;
	 }

	 return PXActionSuccessful;
#endif
}

 PXActionResult PXFileMapToMemoryA(PXFile* const pxFile, const PXTextASCII filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode)
{
	 PXByte filePathU[PathMaxSize];

	 PXTextCopyAU(filePath, PathMaxSize, filePathU, PathMaxSize);

	 const PXActionResult actionResult = PXFileMapToMemoryU(pxFile, filePathU, fileSize, protectionMode);

	 return actionResult;
}

 PXActionResult PXFileMapToMemoryW(PXFile* const pxFile, const PXTextUNICODE filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode)
{
	 PXCharUTF8 filePathU[PathMaxSize];

	 PXTextCopyWU(filePath, PathMaxSize, filePathU, PathMaxSize);

	 const PXActionResult actionResult = PXFileMapToMemoryU(pxFile, filePathU, fileSize, protectionMode);

	 return actionResult;
}

 PXActionResult PXFileMapToMemoryU(PXFile* const pxFile, const PXTextUTF8 filePath, const PXSize fileSize, const PXMemoryAccessMode protectionMode)
{
	 PXBool useLargeMemoryPages = PXNo;

	 PXFileConstruct(pxFile);

#if OSUnix

	 int accessType = PROT_READ;
	 int flags = MAP_PRIVATE;// | MAP_POPULATE;
	 int fileDescriptor = 0;
	 off_t length = 0;

	 // Open file
	 {
		 int openFlag = 0;

		 switch (protectionMode)
		 {
			 case MemoryNoReadWrite:
				 openFlag = 0;
				 break;

			 case PXMemoryAccessModeReadOnly:
				 openFlag = O_RDONLY;
				 break;

			 case PXMemoryAccessModeWriteOnly:
				 openFlag = O_WRONLY;
				 break;

			 case PXMemoryAccessModeReadAndWrite:
				 openFlag = O_RDWR;
				 break;
		 }

		 const int fileDescriptor = open64(filePath, openFlag);
		 const PXBool sucessfulOpen = fileDescriptor != -1;

		 PXActionOnErrorFetchAndExit(!sucessfulOpen)

			 pxFile->IDMapping = fileDescriptor;
	 }

	 // Get file length
	 {
		 const PXSize fileLength = lseek64(pxFile->IDMapping, 0, SEEK_END);
		 const PXBool sucessful = fileLength > 0;

		 if (!sucessful)
		 {
			 return PXActionFailedFileRead;
		 }

		 pxFile->DataSize = fileLength;
	 }

	 // Map data
	 {
		 const PXMemoryAccessModeType protectionModeID = ConvertFromPXMemoryAccessMode(protectionMode);
		 const int flags = MAP_PRIVATE;// | MAP_POPULATE;
		 const off_t offset = 0;

		 const void* mappedData = mmap
		 (
			 0, // addressPrefered
			 pxFile->DataSize,
			 protectionModeID,
			 flags,
			 pxFile->IDMapping, // fileDescriptor
			 offset
		 );
		 const PXBool successfulMapping = mappedData != 0;

		 if (!successfulMapping)
		 {
			 return PXActionFailedFileMapping;
		 }

		 pxFile->Data = mappedData;
	 }

	 pxFile->DataLocation = FileLocationMappedFromDisk;

	 close(pxFile->IDMapping);

	 pxFile->IDMapping = 0;


#elif OSWindows

	 // Open file
	 {
		 const PXActionResult openResult = PXFileOpenFromPathU(pxFile, filePath, protectionMode, PXMemoryCachingModeSequential);

		 PXActionExitOnError(openResult);
	 }

	 pxFile->DataSize = fileSize;
	 pxFile->DataAllocated = fileSize;

	 // Create mapping
	 {
		 SECURITY_ATTRIBUTES	fileMappingAttributes;
		 DWORD flProtect = SEC_COMMIT;
		 DWORD dwMaximumSizeHigh = 0;
		 DWORD dwMaximumSizeLow = 0; // Problem if file is 0 Length
		 wchar_t* name = filePath;

		 switch (protectionMode)
		 {
			 case PXMemoryAccessModeReadOnly:
			 {
				 LARGE_INTEGER largeInt;

				 const BOOL sizeResult = GetFileSizeEx(pxFile->ID, &largeInt);

				 dwMaximumSizeHigh = 0;
				 dwMaximumSizeLow = 0;

				 pxFile->DataSize = largeInt.QuadPart;
				 pxFile->DataAllocated = largeInt.QuadPart;
				 break;
			 }
			 case PXMemoryAccessModeReadAndWrite:
			 case PXMemoryAccessModeWriteOnly:
			 {
#if OS64Bit
				 dwMaximumSizeHigh = (fileSize & (0xFFFFFFFF00000000)) >> 32u;
				 dwMaximumSizeLow = fileSize & 0x00000000FFFFFFFF;
#elif OS32Bit
				 dwMaximumSizeHigh = 0;
				 dwMaximumSizeLow = fileSize & 0xFFFFFFFF;
#endif
				 break;
			 }
			 default:
				 break;
		 }

		 switch (protectionMode)
		 {
			 case PXMemoryAccessModeNoReadWrite:
				 flProtect |= PAGE_NOACCESS;
				 break;

			 case PXMemoryAccessModeReadOnly:
				 flProtect |= PAGE_READONLY;
				 break;

			 case PXMemoryAccessModeWriteOnly:
				 flProtect |= PAGE_READWRITE; // PAGE_WRITECOPY
				 break;

			 case PXMemoryAccessModeReadAndWrite:
				 flProtect |= PAGE_READWRITE;
				 break;
		 }

		 // [i] I want to add LargePage support but it seems you cant do that with files.

		 const HANDLE fileMappingHandleResult = CreateFileMappingA
		 (
			 pxFile->ID,
			 0, // No security attributes
			 flProtect,
			 dwMaximumSizeHigh,
			 dwMaximumSizeLow,
			 0 // No Name
		 );

		 // check if successful
		 {
			 const PXBool successful = fileMappingHandleResult;

			 PXActionOnErrorFetchAndExit(!successful); // File memory-mapping failed

			 pxFile->MappingID = fileMappingHandleResult; // Mapping [OK]
		 }
	 }

	 {
		 DWORD desiredAccess = 0;
		 DWORD fileOffsetHigh = 0;
		 DWORD fileOffsetLow = 0;
		 PXSize numberOfBytesToMap = 0;
		 void* baseAddressTarget = 0;
		 //DWORD  numaNodePreferred = -1; // (NUMA_NO_PREFERRED_NODE)

		 switch (protectionMode)
		 {
			 case PXMemoryAccessModeReadOnly:
				 desiredAccess |= FILE_MAP_READ;
				 break;

			 case PXMemoryAccessModeWriteOnly:
				 desiredAccess |= FILE_MAP_WRITE;
				 break;

			 case PXMemoryAccessModeReadAndWrite:
				 desiredAccess |= FILE_MAP_ALL_ACCESS;
				 break;
		 }

		 // if large pages are supported, anable if
#if WindowsAtleastVista		
		 if (useLargeMemoryPages)
		 {
			 desiredAccess |= FILE_MAP_LARGE_PAGES;
		 }
#endif

		 void* const fileMapped = MapViewOfFile // MapViewOfFileExNuma is only useable starting windows vista, this function in XP
		 (
			 pxFile->MappingID,
			 desiredAccess,
			 fileOffsetHigh,
			 fileOffsetLow,
			 numberOfBytesToMap
		 );

		 pxFile->Data = fileMapped;
		

		 MemoryVirtualPrefetch(fileMapped, pxFile->DataSize);
	 }

#endif

	 pxFile->LocationMode = PXFileLocationModeMappedFromDisk;

#if MemoryDebugOutput
	 printf("[#][Memory] 0x%p (%10zi B) MMAP %ls\n", pxFile->Data, pxFile->DataSize, filePath);
#endif

	 return PXActionSuccessful;
}

 PXActionResult PXFileMapToMemory(PXFile* const pxFile, const PXSize size, const PXMemoryAccessMode protectionMode)
{
	 const void* data = MemoryVirtualAllocate(size, protectionMode);
	 const PXBool successful = data != 0;

	 if (!successful)
	 {
		 return PXActionFailedAllocation;
	 }

	 PXFileConstruct(pxFile);

	 pxFile->AccessMode = protectionMode;
	 pxFile->LocationMode = PXFileLocationModeMappedVirtual;
	 pxFile->Data = data;
	 pxFile->DataSize = size;

	 return PXActionSuccessful;
}

 PXActionResult PXFileUnmapFromMemory(PXFile* const pxFile)
{
	 // Write pending data
	 PXBool isWriteMapped = 0;

	 switch (pxFile->AccessMode)
	 {
		 default:
		 case PXMemoryAccessModeInvalid:
		 case PXMemoryAccessModeNoReadWrite:
		 case PXMemoryAccessModeReadOnly:
			 isWriteMapped = PXFalse;
			 break;

		 case PXMemoryAccessModeWriteOnly:
		 case PXMemoryAccessModeReadAndWrite:
			 isWriteMapped = PXTrue;
			 break;
	 }


#if MemoryDebugOutput
	 printf("[#][Memory] 0x%p (%10zi B) MMAP-Release\n", pxFile->Data, pxFile->DataSize);
#endif

#if OSUnix
	 const int result = munmap(pxFile->Data, pxFile->DataSize);
	 const PXBool sucessful = result != -1;

	 PXActionOnErrorFetchAndExit(!sucessful)

		 pxFile->Data = PXNull;
	 pxFile->DataSize = 0;

	 return PXActionSuccessful;

#elif OSWindows

	 // Write pending data
	 {
		 if (isWriteMapped)
		 {
			 const BOOL flushSuccessful = FlushViewOfFile(pxFile->Data, pxFile->DataCursor);

			 PXActionOnErrorFetchAndExit(flushSuccessful);
		 }
	 }

	 {
		 const PXBool unmappingSucessful = UnmapViewOfFile(pxFile->Data);

		 PXActionOnErrorFetchAndExit(unmappingSucessful);

		 pxFile->Data = PXNull;
	 }

	 {
		 const PXBool closeMappingSucessful = CloseHandle(pxFile->MappingID);

		 PXActionOnErrorFetchAndExit(closeMappingSucessful);

		 pxFile->MappingID = -1;
	 }

	 // Close
	 {

		 if (isWriteMapped)
		 {
			 //fseek();

			 LARGE_INTEGER largeInteger;

			 largeInteger.QuadPart = pxFile->DataCursor;

			 const BOOL setSuccessful = SetFilePointerEx(pxFile->ID, largeInteger, 0, FILE_BEGIN);

			 const BOOL endSuccessful = SetEndOfFile(pxFile->ID);
		 }

		 const PXActionResult closeFile = PXFileClose(pxFile);

		 PXActionExitOnError(closeFile);

		 pxFile->ID = -1;
	 }

	 return PXActionSuccessful;
#endif
}

 PXSize PXFileRemainingSize(const PXFile* PXRestrict const pxFile)
 {
	 return pxFile->DataSize - pxFile->DataCursor;
 }

 PXSize PXFileRemainingSizeRelativeFromAddress(const PXFile* PXRestrict const pxFile, const void* const adress)
{
	 return ((PXSize)pxFile->Data - (PXSize)adress) - pxFile->DataSize;
}

 PXBool PXFileIsAtEnd(const PXFile* PXRestrict const pxFile)
{
	 return pxFile->DataCursor >= pxFile->DataSize;
}

 void* PXFileCursorPosition(PXFile* const pxFile)
{
	 return (PXAdress)pxFile->Data + pxFile->DataCursor;
}

 void PXFileCursorMoveTo(PXFile* const pxFile, const PXSize position)
{
	 pxFile->DataCursor = PXMathMinimumIU(position, pxFile->DataSize);
}

 void PXFileCursorToBeginning(PXFile* const pxFile)
{
	 pxFile->DataCursor = 0;
}

 PXSize PXFileCursorAdvance(PXFile* const pxFile, const PXSize steps)
{
	 const PXSize readableSize = PXFileRemainingSize(pxFile);
	 const PXSize before = pxFile->DataCursor;
	 const PXSize maximalMove = PXMathMinimumIU(readableSize, steps); // Prevent cursor from going out of bounce

	 pxFile->DataCursor += maximalMove;

	 return maximalMove;
}

 void PXFileCursorRewind(PXFile* const pxFile, const PXSize steps)
{
	 pxFile->DataCursor -= steps; // Check underflow
}

 void PXFileCursorToEnd(PXFile* const pxFile)
{
	 pxFile->DataCursor = pxFile->DataSize;
}

 PXSize PXFileSkipEndOfLineCharacters(PXFile* const pxFile)
{
	 const PXSize oldPosition = pxFile->DataCursor;

	 while (!PXFileIsAtEnd(pxFile))
	 {
		 const char* data = PXFileCursorPosition(pxFile);
		 const PXBool advance = IsEmptySpace(*data) || IsTab(*data);

		 if (!advance)
		 {
			 break;
		 }

		 PXFileCursorAdvance(pxFile, 1u);
	 }

	 return pxFile->DataCursor - oldPosition;	
}

 PXSize PXFileSkipEmptySpace(PXFile* const pxFile)
{
	 const PXSize dataPositionBefore = pxFile->DataCursor;

	 while (!PXFileIsAtEnd(pxFile))
	 {
		 const unsigned char* data = PXFileCursorPosition(pxFile);
		 const PXBool advance = IsEndOfLineCharacter(*data) && !IsEndOfString(*data);

		 if (!advance)
		 {
			 break;
		 }

		 PXFileCursorAdvance(pxFile, 1u);
	 }

	 return pxFile->DataCursor - dataPositionBefore;
}

 PXSize PXFileReadNextLineInto(PXFile* const pxFile, void* exportBuffer, const PXSize exportBufferSize)
{
	 const PXSize dataPositionBefore = pxFile->DataCursor;

	 PXFileSkipEndOfLineCharacters(pxFile);

	 while (!PXFileIsAtEnd(pxFile))
	 {
		 const unsigned char* data = PXFileCursorPosition(pxFile);
		 const PXBool advance = !IsEndOfLineCharacter(*data) && !IsEndOfString(*data);

		 if (!advance)
		 {
			 break;
		 }

		 PXFileCursorAdvance(pxFile, 1u);
	 }

	 const char* dataPoint = (char*)pxFile->Data + dataPositionBefore;
	 const PXSize dataPositionAfter = pxFile->DataCursor;
	 const PXSize length = dataPositionAfter - dataPositionBefore;

	 if (length == 0)
	 {
		 return 0;
	 }

	 PXTextCopyA(dataPoint, length, exportBuffer, length);

	 PXFileSkipEndOfLineCharacters(pxFile);

	 return length;
}

 PXSize PXFileSkipBlock(PXFile* const pxFile)
{
	 const PXSize oldPosition = pxFile->DataCursor;

	 while (!PXFileIsAtEnd(pxFile))
	 {
		 const char* data = PXFileCursorPosition(pxFile);
		 const char character = *data;
		 const PXBool advance = !IsEndOfString(character) && !IsEmptySpace(character) && !IsEndOfLineCharacter(character);

		 if (!advance)
		 {
			 break;
		 }

		 PXFileCursorAdvance(pxFile, 1u);
	 }

	 return pxFile->DataCursor - oldPosition;
}

 PXSize PXFileSkipToNextBlock(PXFile* const pxFile)
{
	 const PXSize oldPosition = pxFile->DataCursor;

	 PXFileSkipBlock(pxFile);

	 PXFileSkipEmptySpace(pxFile);

	 return pxFile->DataCursor - oldPosition;
}

 PXSize PXFileSkipLine(PXFile* const pxFile)
{
	 const PXSize positionBefore = pxFile->DataCursor;

	 while (!PXFileIsAtEnd(pxFile))
	 {
		 const unsigned char* data = PXFileCursorPosition(pxFile);
		 const PXBool advance = !IsEndOfLineCharacter(*data) && !IsEndOfString(*data);

		 if (!advance)
		 {
			 break;
		 }

		 PXFileCursorAdvance(pxFile, 1u);
	 }

	 PXFileSkipEndOfLineCharacters(pxFile);

	 const PXSize skippedBytes = pxFile->DataCursor - positionBefore;

	 return skippedBytes;
}

 PXSize PXFileDataCopy(PXFile* const pxInputStream, PXFile* const pxOutputStream, const PXSize length)
{
	 void* const insertionPoint = PXFileCursorPosition(pxOutputStream); // Position of output
	 const PXSize copyedBytes = PXFileReadB(pxInputStream, insertionPoint, length); // Copy from input to output
	 const PXSize movedBytes = PXFileCursorAdvance(pxOutputStream, copyedBytes); // Move output by copyed amount

	 return movedBytes; // return moved amount of bytes
}

 PXSize PXFileReadTextIU8(PXFile* const pxFile, PXInt8U* const number)
{
	 unsigned int value = 0;
	 const PXSize size = PXFileReadTextI(pxFile, &value);

	 *number = value;

	 return size;
}

 PXSize PXFileReadTextI(PXFile* const pxFile, int* const number)
{
	 const void* const adress = PXFileCursorPosition(pxFile);
	 const PXSize size = PXFileRemainingSize(pxFile);
	 const PXSize sizeRead = PXTextToIntA(adress, size, number);

	 PXFileCursorAdvance(pxFile, sizeRead);

	 return sizeRead;
}

 PXSize PXFileReadI8S(PXFile* const pxFile, PXInt8S* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt8S));
}

 PXSize PXFileReadI8SV(PXFile* const pxFile, PXInt8S* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt8S) * valueListSize);
}

 PXSize PXFileReadI8U(PXFile* const pxFile, PXInt8U* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt8U));
}

 PXSize PXFileReadI8UV(PXFile* const pxFile, PXInt8U* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt8U) * valueListSize);
}

 PXSize PXFileReadI16S(PXFile* const pxFile, PXInt16S* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt16S));
}

 PXSize PXFileReadI16SV(PXFile* const pxFile, PXInt16S* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt16S) * valueListSize);
}

 PXSize PXFileReadI16SE(PXFile* const pxFile, PXInt16S* const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt16S);
	 const PXInt16S* const data = (const PXInt16S* const)PXFileCursorPosition(pxFile);
	 const PXSize movedSteps = PXFileCursorAdvance(pxFile, dataSize);

	 *value = *data;

	 PXEndianSwap(value, movedSteps, pxEndian, EndianCurrentSystem);

	 return movedSteps;
}

 PXSize PXFileReadI16SVE(PXFile* const pxFile, PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileReadI16SE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileReadI16U(PXFile* const pxFile, PXInt16U* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt16U));
}

 PXSize PXFileReadI16UV(PXFile* const pxFile, PXInt16U* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt16U) * valueListSize);
}

 PXSize PXFileReadI16UE(PXFile* const pxFile, PXInt16U* const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt16U);
	 const PXInt16U* const data = (const PXInt16U* const)PXFileCursorPosition(pxFile);
	 const PXSize movedSteps = PXFileCursorAdvance(pxFile, dataSize);

	 *value = *data;

	 PXEndianSwap(value, movedSteps, pxEndian, EndianCurrentSystem);

	 return movedSteps;
}

 PXSize PXFileReadI16UVE(PXFile* const pxFile, PXInt16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileReadI16UE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileReadI32S(PXFile* const pxFile, PXInt32S* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt32S));
}

 PXSize PXFileReadI32SV(PXFile* const pxFile, PXInt32S* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt32S) * valueListSize);
}

 PXSize PXFileReadI32SE(PXFile* const pxFile, PXInt32S* const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt32S);
	 const PXInt32S* const data = (const PXInt32S* const)PXFileCursorPosition(pxFile);
	 const PXSize movedSteps = PXFileCursorAdvance(pxFile, dataSize);

	 *value = *data;

	 PXEndianSwap(value, movedSteps, pxEndian, EndianCurrentSystem);

	 return movedSteps;
}

 PXSize PXFileReadI32SVE(PXFile* const pxFile, PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileReadI32SE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileReadI32U(PXFile* const pxFile, PXInt32U* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt32U));
}

 PXSize PXFileReadI32UV(PXFile* const pxFile, PXInt32U* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt32U) * valueListSize);
}

 PXSize PXFileReadI32UE(PXFile* const pxFile, PXInt32U* const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt32U);
	 const PXInt32U* const data = (const PXInt32U* const)PXFileCursorPosition(pxFile);
	 const PXSize movedSteps = PXFileCursorAdvance(pxFile, dataSize);

	 *value = *data;

	 PXEndianSwap(value, movedSteps, pxEndian, EndianCurrentSystem);

	 return movedSteps;
}

 PXSize PXFileReadI32UVE(PXFile* const pxFile, PXInt32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileReadI32UE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileReadI64S(PXFile* const pxFile, PXInt64S* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt64S));
}

 PXSize PXFileReadI64SV(PXFile* const pxFile, PXInt64S* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt64S) * valueListSize);
}

 PXSize PXFileReadI64SE(PXFile* const pxFile, PXInt64S* const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt64S);
	 const PXInt64S* const data = (const PXInt64S* const)PXFileCursorPosition(pxFile);
	 const PXSize movedSteps = PXFileCursorAdvance(pxFile, dataSize);

	 *value = *data;

	 PXEndianSwap(value, movedSteps, pxEndian, EndianCurrentSystem);

	 return movedSteps;
}

 PXSize PXFileReadI64VE(PXFile* const pxFile, PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileReadI64SE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileReadI64U(PXFile* const pxFile, PXInt64U* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(PXInt64U));
}

 PXSize PXFileReadI64UV(PXFile* const pxFile, PXInt64U* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(PXInt64U) * valueListSize);
}

 PXSize PXFileReadI64UE(PXFile* const pxFile, PXInt64U* const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt64U);
	 const PXInt64U* const data = (const PXInt64U* const)PXFileCursorPosition(pxFile);
	 const PXSize movedSteps = PXFileCursorAdvance(pxFile, dataSize);

	 *value = *data;

	 PXEndianSwap(value, movedSteps, pxEndian, EndianCurrentSystem);

	 return movedSteps;
}

 PXSize PXFileReadI64UVE(PXFile* const pxFile, PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileReadI64UE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileReadF(PXFile* const pxFile, float* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(float));
}

 PXSize PXFileReadFV(PXFile* const pxFile, float* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(float) * valueListSize);
}

 PXSize PXFileReadD(PXFile* const pxFile, double* const value)
{
	 return PXFileReadB(pxFile, value, sizeof(double));
}

 PXSize PXFileReadDV(PXFile* const pxFile, double* const valueList, const PXSize valueListSize)
{
	 return PXFileReadB(pxFile, valueList, sizeof(double) * valueListSize);
}

 PXSize PXFileReadMultible(PXFile* const pxFile, PXFileDataElementType* const pxFileElementList, const PXSize pxFileElementListSize)
{
	 PXSize totalReadBytes = 0;

	 for (PXSize i = 0; i < pxFileElementListSize; ++i)
	 {
		 PXFileDataElementType* const pxFileDataElementType = &pxFileElementList[i];

		 switch (pxFileDataElementType->Type)
		 {
			 case PXDataTypeInt8S:
				 totalReadBytes += PXFileReadI8S(pxFile, (PXInt8S*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeInt8U:
				 totalReadBytes += PXFileReadI8U(pxFile, (PXInt8S*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeLEInt16S:
				 totalReadBytes += PXFileReadI16SE(pxFile, (PXInt16S*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt16U:
				 totalReadBytes += PXFileReadI16UE(pxFile, (PXInt16U*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt32S:
				 totalReadBytes += PXFileReadI32SE(pxFile, (PXInt32S*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt32U:
				 totalReadBytes += PXFileReadI32UE(pxFile, (PXInt32U*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt64S:
				 totalReadBytes += PXFileReadI64SE(pxFile, (PXInt64S*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt64U:
				 totalReadBytes += PXFileReadI64UE(pxFile, (PXInt64U*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeBEInt16S:
				 totalReadBytes += PXFileReadI16SE(pxFile, (PXInt16S*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt16U:
				 totalReadBytes += PXFileReadI16UE(pxFile, (PXInt16U*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt32S:
				 totalReadBytes += PXFileReadI32SE(pxFile, (PXInt32S*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt32U:
				 totalReadBytes += PXFileReadI32UE(pxFile, (PXInt32U*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt64S:
				 totalReadBytes += PXFileReadI64SE(pxFile, (PXInt64S*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt64U:
				 totalReadBytes += PXFileReadI64UE(pxFile, (PXInt64U*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeFloat:
				 totalReadBytes += PXFileReadF(pxFile, (float*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeDouble:
				 totalReadBytes += PXFileReadD(pxFile, (double*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeTypeInvalid:
			 default:
				 break;
		 }
	 }

	 return totalReadBytes;
}

 PXSize PXFileReadB(PXFile* const pxFile, void* const value, const PXSize length)
{
	 const void* currentPosition = PXFileCursorPosition(pxFile);
	 const PXSize moveSize = PXFileCursorAdvance(pxFile, length);

	 MemoryCopy(currentPosition, moveSize, value, moveSize);

	 return moveSize;
}

 PXSize PXFileReadTextA(PXFile* const pxFile, PXTextASCII value, const PXSize length)
{
	 const PXSize size = PXFileReadB(pxFile, value, length);

	 value[size] = 0;

	 return size;
}

 PXSize PXFileReadTextW(PXFile* const pxFile, PXTextUNICODE value, const PXSize length)
{
	 const PXSize size = PXFileReadB(pxFile, value, length);

	 value[size] = 0;

	 return size;
}

 PXSize PXFileReadTextU(PXFile* const pxFile, PXTextUNICODE value, const PXSize length)
{
	 const PXSize size = PXFileReadB(pxFile, value, length);

	 value[size] = 0;

	 return size;
}

 void PXFileReadUntil(PXFile* const pxFile, void* value, const PXSize length, const char character)
{
	 const unsigned char* currentPosition = PXFileCursorPosition(pxFile);

	 PXSize lengthCopy = 0;

	 while (!PXFileIsAtEnd(pxFile))
	 {
		 const unsigned char* data = PXFileCursorPosition(pxFile);
		 const unsigned char advance = *data != character && length <= lengthCopy;

		 if (!advance)
		 {
			 break;
		 }

		 ++lengthCopy;

		 PXFileCursorAdvance(pxFile, 1u);
	 }

	 const PXSize readableSize = PXFileRemainingSize(pxFile);

	 MemoryCopy(currentPosition, readableSize, value, lengthCopy);
}

 PXBool PXFileReadAndCompareI64U(PXFile* const pxFile, const PXInt64U value)
{
	 const void* currentPosition = PXFileCursorPosition(pxFile);
	 const PXSize readableSize = PXFileRemainingSize(pxFile);

	 const PXInt64U valueA = PXInt64FromAdress(currentPosition);
	 const PXBool result = value == valueA;

	 if (result)
	 {
		 PXFileCursorAdvance(pxFile, sizeof(PXInt64U));
	 }

	 return result;
}

 PXBool PXFileReadAndCompare(PXFile* const pxFile, const void* value, const PXSize length)
{
	 const void* currentPosition = PXFileCursorPosition(pxFile);
	 const PXSize readableSize = PXFileRemainingSize(pxFile);

	 const PXBool result = MemoryCompare(currentPosition, readableSize, value, length);

	 if (result)
	 {
		 PXFileCursorAdvance(pxFile, length);
	 }

	 return result;
}

 PXBool PXFileReadAndCompareV(PXFile* const pxFile, const void** value, const PXSize* valueElementSizeList, const PXSize valueLength)
{
	 const void* currentPosition = PXFileCursorPosition(pxFile);
	 const PXSize readableSize = PXFileRemainingSize(pxFile);

	 for (size_t i = 0; i < valueLength; ++i)
	 {
		 void* text = value[i];
		 PXSize size = valueElementSizeList[i];
		 const PXBool result = MemoryCompare(currentPosition, readableSize, text, size);

		 if (result)
		 {
			 PXFileCursorAdvance(pxFile, size);
			 return PXTrue;
		 }
	 }

	 return PXFalse;
}

 PXSize PXFileWriteI8S(PXFile* const pxFile, const PXInt8S value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt8S));
}

 PXSize PXFileWriteI8SV(PXFile* const pxFile, const PXInt8S* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt8S) * valueListSize);
}

 PXSize PXFileWriteI8U(PXFile* const pxFile, const PXInt8U value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt8U));
}

 PXSize PXFileWriteAtI8U(PXFile* const pxFile, const PXInt8U value, const PXSize index)
{
	 const PXSize positionBefore = pxFile->DataCursor; // save current position

	 pxFile->DataCursor = index; // jump to offset

	 const PXSize writtenBytes = PXFileWriteI8U(pxFile, value); // Length

	 pxFile->DataCursor = positionBefore; // Reset old position

	 return writtenBytes;
}

 PXSize PXFileWriteI8UV(PXFile* const pxFile, const PXInt8U* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt8U) * valueListSize);
}

 PXSize PXFileWriteI16S(PXFile* const pxFile, const PXInt16S const value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt16S));
}

 PXSize PXFileWriteI16SV(PXFile* const pxFile, const PXInt16S* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt16S) * valueListSize);
}

 PXSize PXFileWriteI16SE(PXFile* const pxFile, const PXInt16S const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt16S);
	 PXInt16S dataValue = value;

	 PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

	 PXFileWriteB(pxFile, &dataValue, dataSize);

	 return dataSize;
}

 PXSize PXFileWriteI16SVE(PXFile* const pxFile, const PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileWriteI16SE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileWriteI16U(PXFile* const pxFile, const PXInt16U const value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt16U));
}

 PXSize PXFileWriteAtI16U(PXFile* const pxFile, const PXInt16U const value, const PXSize index)
{
	 const PXSize positionBefore = pxFile->DataCursor; // save current position

	 pxFile->DataCursor = index; // jump to offset

	 const PXSize writtenBytes = PXFileWriteI16U(pxFile, value); // Length

	 pxFile->DataCursor = positionBefore; // Reset old position

	 return writtenBytes;
}

 PXSize PXFileWriteI16UV(PXFile* const pxFile, const PXInt16U* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt16U) * valueListSize);
}

 PXSize PXFileWriteI16UE(PXFile* const pxFile, const PXInt16U const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt16U);
	 PXInt16U dataValue = value;

	 PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

	 PXFileWriteB(pxFile, &dataValue, dataSize);

	 return dataSize;
}

 PXSize PXFileWriteI16UVE(PXFile* const pxFile, const PXInt16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileWriteI16UE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileWriteI32S(PXFile* const pxFile, const PXInt32S value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt32S));
}

 PXSize PXFileWriteI32SV(PXFile* const pxFile, const PXInt32S* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt32S) * valueListSize);
}

 PXSize PXFileWriteI32SE(PXFile* const pxFile, const PXInt32S value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt32S);
	 PXInt32S dataValue = value;

	 PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

	 PXFileWriteB(pxFile, &dataValue, dataSize);

	 return dataSize;
}

 PXSize PXFileWriteI32SVE(PXFile* const pxFile, const PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileWriteI32SE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileWriteI32U(PXFile* const pxFile, const PXInt32U value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt32U));
}

 PXSize PXFileWriteAtI32U(PXFile* const pxFile, const PXInt32U value, const PXSize index)
{
	 const PXSize positionBefore = pxFile->DataCursor; // save current position

	 pxFile->DataCursor = index; // jump to offset

	 const PXSize writtenBytes = PXFileWriteI32U(pxFile, value); // Length

	 pxFile->DataCursor = positionBefore; // Reset old position

	 return writtenBytes;
}

 PXSize PXFileWriteI32UV(PXFile* const pxFile, const PXInt32U* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt32U) * valueListSize);
}

 PXSize PXFileWriteI32UE(PXFile* const pxFile, const PXInt32U value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt32U);
	 PXInt32U dataValue = value;

	 PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

	 PXFileWriteB(pxFile, &dataValue, dataSize);

	 return dataSize;
}

 PXSize PXFileWriteAtI32UE(PXFile* const pxFile, const PXInt32U value, const PXEndian pxEndian, const PXSize index)
{
	 const PXSize positionBefore = pxFile->DataCursor; // save current position

	 pxFile->DataCursor = index; // jump to offset

	 const PXSize writtenBytes = PXFileWriteI32UE(pxFile, value, pxEndian); // Length

	 pxFile->DataCursor = positionBefore; // Reset old position

	 return writtenBytes;
}

 PXSize PXFileWriteI32UVE(PXFile* const pxFile, const PXInt32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileWriteI32UE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileWriteI64S(PXFile* const pxFile, const PXInt64S const value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt64S));
}

 PXSize PXFileWriteI64SV(PXFile* const pxFile, const PXInt64S* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt64S) * valueListSize);
}

 PXSize PXFileWriteI64SE(PXFile* const pxFile, const PXInt64S const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt64S);
	 PXInt64S dataValue = value;

	 PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

	 PXFileWriteB(pxFile, &dataValue, dataSize);

	 return dataSize;
}

 PXSize PXFileWriteI64VE(PXFile* const pxFile, const PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileWriteI64SE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileWriteI64U(PXFile* const pxFile, const PXInt64U const value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(PXInt64U));
}

 PXSize PXFileWriteI64UV(PXFile* const pxFile, const PXInt64U* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(PXInt64U) * valueListSize);
}

 PXSize PXFileWriteI64UE(PXFile* const pxFile, const PXInt64U const value, const PXEndian pxEndian)
{
	 const PXSize dataSize = sizeof(PXInt64U);
	 PXInt64U dataValue = value;

	 PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

	 PXFileWriteB(pxFile, &dataValue, dataSize);

	 return dataSize;
}

 PXSize PXFileWriteI64UVE(PXFile* const pxFile, const PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	 PXSize accumulator = 0;

	 for (PXSize i = 0; i < valueListSize; ++i)
	 {
		 accumulator += PXFileWriteI64UE(pxFile, &valueList[i], pxEndian);
	 }

	 return accumulator;
}

 PXSize PXFileWriteF(PXFile* const pxFile, const float value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(float));
}

 PXSize PXFileWriteFV(PXFile* const pxFile, const float* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(float) * valueListSize);
}

 PXSize PXFileWriteD(PXFile* const pxFile, const double value)
{
	 return PXFileWriteB(pxFile, &value, sizeof(double));
}

 PXSize PXFileWriteDV(PXFile* const pxFile, const double* const valueList, const PXSize valueListSize)
{
	 return PXFileWriteB(pxFile, valueList, sizeof(double) * valueListSize);
}

 PXSize PXFileWriteB(PXFile* const pxFile, const void* const value, const PXSize length)
{
	 const PXSize writableSize = PXFileRemainingSize(pxFile);
	 void* const currentPosition = PXFileCursorPosition(pxFile);
	 const PXSize copyedBytes = MemoryCopy(value, length, currentPosition, writableSize);

	 PXFileCursorAdvance(pxFile, copyedBytes);

	 return copyedBytes;
}

 PXSize PXFileWriteAtB(PXFile* const pxFile, const void* const data, const PXSize dataSize, const PXSize index)
{
	 const PXSize positionBefore = pxFile->DataCursor; // save current position

	 pxFile->DataCursor = index; // jump to offset

	 const PXSize writtenBytes = PXFileWriteB(pxFile, data, dataSize); // Length

	 pxFile->DataCursor = positionBefore; // Reset old position

	 return writtenBytes;
}

 PXSize PXFileWriteMultible(PXFile* const pxFile, const PXFileDataElementType* const pxFileElementList, const PXSize pxFileElementListSize)
{
	 PXSize totalReadBytes = 0;

	 for (PXSize i = 0; i < pxFileElementListSize; ++i)
	 {
		 PXFileDataElementType* const pxFileDataElementType = &pxFileElementList[i];

		 switch (pxFileDataElementType->Type)
		 {
			 case PXDataTypeInt8S:
				 totalReadBytes += PXFileWriteI8S(pxFile, *(PXInt8S*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeInt8U:
				 totalReadBytes += PXFileWriteI8U(pxFile, *(PXInt8S*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeLEInt16S:
				 totalReadBytes += PXFileWriteI16SE(pxFile, *(PXInt16S*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt16U:
				 totalReadBytes += PXFileWriteI16UE(pxFile, *(PXInt16U*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt32S:
				 totalReadBytes += PXFileWriteI32SE(pxFile, *(PXInt32S*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt32U:
				 totalReadBytes += PXFileWriteI32UE(pxFile, *(PXInt32U*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt64S:
				 totalReadBytes += PXFileWriteI64SE(pxFile, *(PXInt64S*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeLEInt64U:
				 totalReadBytes += PXFileWriteI64UE(pxFile, *(PXInt64U*)pxFileDataElementType->Adress, PXEndianLittle);
				 break;

			 case PXDataTypeBEInt16S:
				 totalReadBytes += PXFileWriteI16SE(pxFile, *(PXInt16S*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt16U:
				 totalReadBytes += PXFileWriteI16UE(pxFile, *(PXInt16U*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt32S:
				 totalReadBytes += PXFileWriteI32SE(pxFile, *(PXInt32S*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt32U:
				 totalReadBytes += PXFileWriteI32UE(pxFile, *(PXInt32U*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt64S:
				 totalReadBytes += PXFileWriteI64SE(pxFile, *(PXInt64S*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeBEInt64U:
				 totalReadBytes += PXFileWriteI64UE(pxFile, *(PXInt64U*)pxFileDataElementType->Adress, PXEndianBig);
				 break;

			 case PXDataTypeFloat:
				 totalReadBytes += PXFileWriteF(pxFile, *(float*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeDouble:
				 totalReadBytes += PXFileWriteD(pxFile, *(double*)pxFileDataElementType->Adress);
				 break;

			 case PXDataTypeTypeInvalid:
			 default:
				 break;
		 }
	 }

	 return totalReadBytes;
}

 PXSize PXFileWriteFill(PXFile* const pxFile, const unsigned char value, const PXSize length)
{
	 void* const beforePosition = PXFileCursorPosition(pxFile);
	 const PXSize writableSize = PXFileRemainingSize(pxFile);
	 const PXSize write = PXMathMinimumIU(writableSize, length);

	 MemorySet(beforePosition, value, write);

	 PXFileCursorAdvance(pxFile, write);

	 return write;
}

 PXSize PXFileWriteA(PXFile* const pxFile, const PXTextASCII text, PXSize textSize)
{
#if 1
	 return PXFileWriteB(pxFile, text, textSize);
#else
	 const PXSize writableSize = PXFileRemainingSize(pxFile);
	 char* const currentPosition = (char* const)PXFileCursorPosition(pxFile);

	 const PXSize writtenBytes = PXTextCopyA(text, textSize, currentPosition, writableSize);

	 PXFileCursorAdvance(pxFile, writtenBytes);

	 return writtenBytes;
#endif
}

 PXSize PXFileWriteW(PXFile* const pxFile, const PXTextUNICODE const text, PXSize textSize)
{
	 const PXSize writableSize = PXFileRemainingSize(pxFile);
	 wchar_t* const currentPosition = (wchar_t* const)PXFileCursorPosition(pxFile);

	 const PXSize writtenBytes = PXTextCopyW(text, textSize, currentPosition, writableSize);

	 PXFileCursorAdvance(pxFile, writtenBytes);

	 return writtenBytes;
}

 PXSize PXFileWriteAF(PXFile* const pxFile, const PXTextASCII format, ...)
{
	 const unsigned char* currentPosition = PXFileCursorPosition(pxFile);

	 va_list args;
	 va_start(args, format);

	 const PXSize writableSize = PXFileRemainingSize(pxFile);
	 const int writtenBytes = PrintSVN(currentPosition, writableSize, format, args);

	 va_end(args);

	 {
		 const unsigned char sucessful = writtenBytes >= 0;

		 if (!sucessful)
		 {
			 return 0;
		 }
	 }

	 PXFileCursorAdvance(pxFile, writtenBytes);

	 return writtenBytes;
}

 PXSize PXFileWriteWF(PXFile* const pxFile, const PXTextUNICODE format, ...)
{
	return  0;
}

PXSize PXFileSkipBitsToNextByte(PXFile* const pxFile)
{
	const unsigned char hasAnyBitConsumed = pxFile->DataCursorBitOffset > 0;

	if (hasAnyBitConsumed)
	{
		pxFile->DataCursorBitOffset = 0; // Reset
		++pxFile->DataCursor; // Go 1 Byte further
	}

	return 0;
}

PXSize PXFileCursorMoveBits(PXFile* const pxFile, const PXSize amountOfBits)
{
	pxFile->DataCursorBitOffset += amountOfBits;

	PXFileBitsAllign(pxFile);

	return 0;
}

PXSize PXFileBitsAllign(PXFile* const pxFile)
{
	PXSize accumulator = 0;

	while (pxFile->DataCursorBitOffset >= 8u) // Move a Byte__ at the time forward, 8 Bits = 1 Byte__.
	{
		pxFile->DataCursor++;
		pxFile->DataCursorBitOffset -= 8u;

		++accumulator;
	}

	return accumulator;
}

PXSize PXFilePeekBits(PXFile* const pxFile, const PXSize amountOfBits)
{
	unsigned int bitMask = ((1u << amountOfBits) - 1u) << pxFile->DataCursorBitOffset; // 0000111111
	unsigned int bitBlock;
	unsigned char* a = PXFileCursorPosition(pxFile);
	unsigned char* b = a + 1;
	unsigned char* c = a + 2;
	unsigned char* d = a + 3;
	unsigned char* maxAdress = (unsigned char*)pxFile->Data + (pxFile->DataSize - 1);

	unsigned int ai = a > maxAdress ? 0 : *a;
	unsigned int bi = b > maxAdress ? 0 : *b;
	unsigned int ci = c > maxAdress ? 0 : *c;
	unsigned int di = d > maxAdress ? 0 : *d;

	// [d][c][b][a] Little pxEndian

	bitBlock = ai | bi << 8 | ci << 16 | di << 24;

	unsigned int result = bitBlock & bitMask;

#if BitStreamDebug
	printf("Extract %i Bits. Byte__:%i Offset:%i\n", amountOfBits, pxFile->DataCursor, pxFile->BitOffset);
	printf("BitBlock : ");
	PrintBinary(bitBlock);
	printf("BitMask  : ");
	PrintBinary(bitMask);
	printf("Result   : ");
	//PrintBinary(result);
#endif

	result >>= pxFile->DataCursorBitOffset; // Shoitft correction

#if BitStreamDebug
	printf("Shifted  : ");
	//PrintBinary(result);
#endif

	return result;
}

PXSize PXFileReadBits(PXFile* const pxFile, const PXSize amountOfBits)
{
	const PXSize result = PXFilePeekBits(pxFile, amountOfBits);

	PXFileCursorMoveBits(pxFile, amountOfBits);

	return result;
}

PXSize PXFileWriteBits(PXFile* const pxFile, const PXSize bitData, const PXSize amountOfBits)
{

	PXSize* const currentPos = (PXSize* const)PXFileCursorPosition(pxFile);
	PXSize bitBlockCache = 0;

	for (PXSize i = 0; i < amountOfBits; i++)
	{
		// Data = current bit << current offset
		bitBlockCache |= (bitData & ((PXSize)1u << i));
	}

	*currentPos |= bitBlockCache << pxFile->DataCursorBitOffset;
	pxFile->DataCursorBitOffset += amountOfBits;

	const PXSize movedBytes = PXFileBitsAllign(pxFile);

	return amountOfBits;
}

PXSize PXFileFilePathGetA(PXFile* const pxFile, char* const filePath, const PXSize filePathMaxSize)
{
#if OSUnix
	return 0; // TODO

#elif OSWindows
	const PXSize length = GetFinalPathNameByHandleA(pxFile->ID, filePath, filePathMaxSize, VOLUME_NAME_DOS); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool sucessful = 0u == length;

	if (!sucessful)
	{
		const PXActionResult result = GetCurrentError();

		return result;
	}

	return PXActionSuccessful;
#endif
}

PXSize PXFileFilePathGetW(PXFile* const pxFile, wchar_t* const filePath, const PXSize filePathMaxSize)
{
#if OSUnix
	return 0; // TODO

#elif OSWindows
	const DWORD flags = VOLUME_NAME_DOS | FILE_NAME_NORMALIZED;
	const DWORD length = GetFinalPathNameByHandleW(pxFile->ID, filePath, filePathMaxSize, flags); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool sucessful = 0u != length;

	if (!sucessful)
	{
		const PXActionResult result = GetCurrentError();

		return result;
	}

	wchar_t buffer[PathMaxSize];

	if (filePath[0] == '\\')
	{
		PXTextCopyW(filePath + 4, length - 4, buffer, PathMaxSize);
		PXTextCopyW(buffer, PathMaxSize, filePath, length - 4);
	}

	return PXActionSuccessful;
#endif
}















/*

PXActionResult FileWorkingDirectoryChangeA(const char* directoryName)
{
	int creationResult = WorkingDirectoryChangeA(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

PXActionResult FileWorkingDirectoryChangeW(const wchar_t* directoryName)
{
	int creationResult = WorkingDirectoryChangeW(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

PXActionResult FileDirectoryDeleteA(const char* directoryName)
{
	int creationResult = FileRemoveA(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

PXActionResult FileDirectoryDeleteW(const wchar_t* directoryName)
{
	int creationResult = FileRemoveW(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

PXActionResult FileMapToVirtualMemoryA(const char* filePath, const PXMemoryAccessMode protectionMode)
{
}

PXActionResult FileMapToVirtualMemoryW(const wchar_t* filePath, const PXMemoryAccessMode protectionMode)
{

}

PXActionResult FileMapToVirtualMemory(const PXSize size, const PXMemoryAccessMode protectionMode)
{

}

PXActionResult FileUnmapFromVirtualMemory()
{

}

PXActionResult FileReadFromDisk(const char* filePath, bool addNullTerminator, FilePersistence filePersistence)
{
	File file;

	// Open file
	{
		const PXActionResult result = file.Open(filePath, FileOpenMode::Read, FileCachingMode::Sequential);
		const bool sucessful = result == PXActionSuccessful;

		if(!sucessful)
		{
			return result;
		}
	}

	// Read
	{
		const PXActionResult result = file.ReadFromDisk(&Data, DataSize, addNullTerminator);
		const bool sucessful = result == PXActionSuccessful;

		if(!sucessful)
		{
			file.Close();
			return result;
		}
	}

	// Close
	{
		const PXActionResult result = file.Close();
		const bool sucessful = result == PXActionSuccessful;

		if(!sucessful)
		{
			return result;
		}
	}

	_fileLocation = FileLocation::CachedFromDisk;

	return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(const wchar_t* filePath, bool addNullTerminator, FilePersistence filePersistence)
{
	File file;
	PXActionResult result = file.Open(filePath, FileOpenMode::Read, FileCachingMode::Sequential);

	if(result != PXActionSuccessful)
	{
		return result;
	}

	result = file.ReadFromDisk(&Data, DataSize, addNullTerminator);

	if(result != PXActionSuccessful)
	{
		file.Close();
		return result;
	}

	result = file.Close();

	if(result != PXActionSuccessful)
	{
		return result;
	}

	_fileLocation = FileLocation::CachedFromDisk;

	return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(FILE* file, Byte__** targetBuffer, PXSize& bufferSize, bool addNullTerminator)
{
	fseek(file, 0, SEEK_END); // Jump to end of file
	bufferSize = ftell(file); // Get current 'data-cursor' position

	if(!bufferSize) // If no bytes in file, exit.
	{
		return ResultFileEmpty;
	}

	rewind(file); // Jump to the begining of the file

	if(addNullTerminator)
	{
		++bufferSize;
	}

	Byte__* dataBuffer = Memory::Allocate<Byte__>(bufferSize);

	if(!dataBuffer) // If malloc failed
	{
		return ResultOutOfMemory;
	}

	*targetBuffer = dataBuffer;

	if(addNullTerminator)
	{
		dataBuffer[bufferSize - 1] = '\0';
		--bufferSize;
	}

	PXSize readBytes = fread(dataBuffer, 1u, bufferSize, file);
	PXSize overAllocatedBytes = bufferSize - readBytes; // if overAllocatedBytes > 0 there was a reading error.

	assert(bufferSize == readBytes);

	return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(const wchar_t* filePath, Byte__** targetBuffer, PXSize& bufferSize, bool addNullTerminator, FilePersistence filePersistence)
{
	File file;
	PXActionResult result = file.Open(filePath, FileOpenMode::Read);

	if(result != PXActionSuccessful)
	{
		return result;
	}

	//result = ReadFromDisk(file.FileMarker, targetBuffer, bufferSize, addNullTerminator);

	file.ReadFromDisk(targetBuffer, bufferSize, addNullTerminator);

	result = file.Close();

	return PXActionSuccessful;
}

PXActionResult FileWriteToDisk(const bool value)
{
	return WriteToDisk(&value, sizeof(bool));
}

PXActionResult FileWriteToDisk(const char value)
{
	return WriteToDisk(&value, sizeof(char));
}

PXActionResult FileWriteToDisk(const unsigned char value)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const short value, const pxEndian pxEndian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const unsigned short value, const pxEndian pxEndian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const int value, const pxEndian pxEndian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const unsigned int value, const pxEndian pxEndian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const char* string, const PXSize length)
{
	return WriteToDisk(string, length);
}

PXActionResult FileWriteToDisk(const unsigned char* string, const PXSize length)
{
	return WriteToDisk(string, length);
}

PXActionResult FileWriteToDisk(const unsigned long long& value, const pxEndian pxEndian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const void* value, const PXSize length)
{
#if OSUnix
FILE* fileHandle = FileHandle;
#elif OSWindows
FILE* fileHandle = FileHandleCStyle;
#endif

	const PXSize writtenSize = fwrite(value, sizeof(Byte__), length, fileHandle);

	if(writtenSize > 0)
	{
		return PXActionSuccessful;
	}
	else
	{
		return ResultWriteFailure;
	}
}

PXActionResult FileWriteToDisk(const char* format, ...)
{
#if OSUnix
    FILE* fileHandle = FileHandle;
#elif OSWindows
    FILE* fileHandle = FileHandleCStyle;
#endif

	va_list args;
	va_start(args, format);

	const int writtenBytes = vfprintf(fileHandle, format, args);
	const bool sucessful = writtenBytes >= 0;

	va_end(args);

	if(!sucessful)
	{
		return ResultWriteFailure;
	}

	return PXActionSuccessful;
}

PXActionResult FileWriteIntoFile(const void* data, const PXSize dataSize)
{
#if OSUnix
	PXSize writtenBytes = fwrite(data, sizeof(char), dataSize, FileHandle);
#elif OSWindows
	DWORD writtenBytes = 0;
	const bool successful = WriteFile(FileHandle, data, dataSize, &writtenBytes, nullptr);
#endif

	return PXActionResult();
}

PXActionResult FileWriteToDisk(const char* filePath, FilePersistence filePersistence)
{
	File file;
	PXActionResult filePXActionResult = file.Open(filePath, FileOpenMode::Write);

	if(filePXActionResult != PXActionSuccessful)
	{
		return filePXActionResult;
	}

	WriteIntoFile(Data, DataSize);

	filePXActionResult = file.Close();

	return PXActionSuccessful;
}

PXActionResult FileWriteToDisk(const wchar_t* filePath, FilePersistence filePersistence)
{
	File file;

	{
		const PXActionResult filePXActionResult = file.Open(filePath, FileOpenMode::Write);
		const bool sucessful = filePXActionResult == PXActionSuccessful;

		if(!sucessful)
		{
			return filePXActionResult;
		}
	}

#if OSUnix
	PXSize writtenBytes = fwrite(Data, sizeof(char), DataCursorPosition, file.FileHandle);
#elif OSWindows
	DWORD writtenBytes = 0;
	const bool successful = WriteFile(file.FileHandle, Data, DataCursor, &writtenBytes, nullptr);
#endif

	{
		const PXActionResult closeResult = file.Close();
		const bool sucessful = closeResult == PXActionSuccessful;

		if(!sucessful)
		{
			return closeResult;
		}
	}

	return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(unsigned char** outPutBuffer, PXSize& outPutBufferSize, const bool addTerminatorByte)
{
#if OSUnix
	fseek(FileHandle, 0, SEEK_END); // Jump to end of file
	outPutBufferSize = ftell(FileHandle); // Get current 'data-cursor' position

	if (!outPutBufferSize) // If no bytes in file, exit.
	{
		return ResultFileEmpty;
	}

	rewind(FileHandle); // Jump to the begining of the file

	if (addTerminatorByte)
	{
		++outPutBufferSize;
	}

	unsigned char* dataBuffer = Memory::Allocate<unsigned char>(outPutBufferSize);

	if (!dataBuffer) // If malloc failed
	{
		return ResultOutOfMemory;
	}

	*outPutBuffer = dataBuffer;

	if (addTerminatorByte)
	{
		dataBuffer[outPutBufferSize - 1] = '\0';
		--outPutBufferSize;
	}

	PXSize readBytes = fread(dataBuffer, 1u, outPutBufferSize, FileHandle);
	PXSize overAllocatedBytes = outPutBufferSize - readBytes; // if overAllocatedBytes > 0 there was a reading error.

	assert(outPutBufferSize == readBytes);

	return PXActionSuccessful;
#elif OSWindows
	const DWORD fileSize = GetFileSize(FileHandle, nullptr);
	DWORD numberOfBytesRead = 0;
	OVERLAPPED* overlapped = nullptr;
	//LARGE_INTEGER size;
	//bool succesSize = GetFileSizeEx(FileHandle, &size);
	PXSize allocationSize = fileSize;

	if (addTerminatorByte)
	{
		++allocationSize;
	}

	unsigned char* buffer = Memory::Allocate<unsigned char>(allocationSize);

	if (!buffer)
	{
		return ResultOutOfMemory;
	}

	bool sucessful = ReadFile
	(
		FileHandle, // file
		buffer, // input
		fileSize, // Shall read
		&numberOfBytesRead, // Has read
		overlapped
	);

	if (!sucessful)
	{
		return ResultFileReadFailure;
	}

	buffer[allocationSize - 1] = '\0';

	(*outPutBuffer) = buffer;
	outPutBufferSize = numberOfBytesRead;

	return PXActionSuccessful;
#endif
}

void FilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName)
{
	const PXSize index = PXTextFindLastW(currnetPath, PathMaxSize, '/');
	const bool found = index != -1;

	if (found)
	{
		const PXSize copyedBytes = PXTextCopyW(currnetPath, index + 1, targetPath, index + 1);
		const PXSize toCopy = PathMaxSize - copyedBytes;

		PXTextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
	}
}

void FileFilesInFolder(const char* folderPath, wchar_t*** list, PXSize& listSize)
{
#if OSUnix
	DIR* directory = opendir(folderPath);

	if (directory)
	{
		struct dirent* directoryInfo = nullptr;

		while (directoryInfo = readdir(directory))
		{
			++listSize;
		}

		rewinddir(directory);

		(*list) = Memory::Allocate<wchar_t*>(listSize);

		for (PXSize index = 0; directoryInfo = readdir(directory); index++)
		{
			const bool isFile = directoryInfo->d_type == DT_REG || true;

			if (isFile)
			{
				const char* fileName = directoryInfo->d_name;
				const PXSize length = Text::Length(fileName);
				wchar_t* newString = Memory::Allocate<wchar_t>(length + 1);
				wchar_t** target = &(*list)[index];

				if (!newString)
				{
					return; // Error: OutOfMemory
				}

				(*target) = newString;
				PXSize writtenBytes = Text::Copy(fileName, length, *target, length);
			}
		}

		closedir(directory);
	}
#elif OSWindows
	wchar_t folderPathW[PathMaxSize];
	PXSize writtenBytes = PXTextCopyAW(folderPath, PathMaxSize, folderPathW, PathMaxSize);

	WIN32_FIND_DATA dataCursour{0};
	HANDLE hFind = 0;

	//Memory::Set(&dataCursour, 0, sizeof(WIN32_FIND_DATA));

	hFind = FindFirstFile(folderPathW, &dataCursour); 	// "/*.*";

	bool foundData = hFind != INVALID_HANDLE_VALUE;

	if (!foundData)
	{
		return;
	}

	++listSize;

	for (; FindNextFile(hFind, &dataCursour); listSize++);

	MemorySet(&dataCursour, 0, sizeof(WIN32_FIND_DATA));

	(*list) = Memory::Allocate<wchar_t*>(listSize);

	hFind = FindFirstFile(folderPathW, &dataCursour); // Expected "." Folder
	PXSize fileIndex = 0;

	do
	{
		const PXSize length = TextLengthW(dataCursour.cFileName);
		const wchar_t* filePathSource = dataCursour.cFileName;
		wchar_t* newString = Memory::Allocate<wchar_t>(length + 1);

		if (!newString)
		{
			return; // Error: OutOfMemory
		}

		PXTextCopyW(filePathSource, length, newString, length);

		(*list)[fileIndex] = newString;

		fileIndex++;
	}
	while (FindNextFile(hFind, &dataCursour));

	FindClose(hFind);
#endif
}

void FileFilesInFolder(const wchar_t* folderPath, wchar_t*** list, PXSize& listSize)
{

}

unsigned char FileDoesExistA(const char* filePath)
{
	FILE* file = OSFileOpenA(filePath, "rb");

	if (file)
	{
		fclose(file);

		return 1;
	}

	return 0;
}

unsigned char FileDoesExistW(const wchar_t* filePath)
{
#if OSUnix
    char filePathA[PathMaxSize];

    Text::Copy(filePath, PathMaxSize, filePathA, PathMaxSize);

    return DoesFileExist(filePathA);

#elif OSWindows
	FILE* file = OSFileOpenW(filePath, L"rb");

	if(file)
	{
		fclose(file);

		return 1;
	}
#endif // defined

	return 0;
}*/

