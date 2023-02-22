#include "File.h"

#include <Text/Text.h>
#include <Memory/PXMemory.h>
#include <Math/PXMath.h>

#include <stdarg.h>
#include <fcntl.h>


#define FileReadMode "rb"
#define FileReadModeW L"rb"

#define FileWriteMode "wb"
#define FileWriteModeW L"wb"

#define FileReadWriteMode "rwb"
#define FileReadWriteModeW L"rwb"


#if OSUnix
#include <sys/mman.h>
#include <sys/stat.h>

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

void FilePathSplittA(const char* fullPath, PXSize fullPathMaxSize, char* drive, PXSize driveMaxSize, char* directory, PXSize directoryMaxSize, char* fileName, PXSize fileNameMaxSize, char* extension, PXSize extensionMaxSize)
{
#if OSUnix
	char directoryNameCache[PathMaxSize];
	char baseNameCache[FileNameMaxSize];

	TextCopyA(fullPath, FileNameMaxSize, directoryNameCache, FileNameMaxSize);
	TextCopyA(fullPath, FileNameMaxSize, baseNameCache, FileNameMaxSize);

	char* dirNameResult = dirname(directoryNameCache);
	char* baseNameResult = basename(baseNameCache);

	PXSize directoryLength = TextCopyA(dirNameResult, DirectoryMaxSize, directory, DirectoryMaxSize);
	PXSize fileNameLength = TextCopyA(baseNameResult, FileNameMaxSize, fileName, FileNameMaxSize);

	for(PXSize i = fileNameLength - 1; i > 0; --i)
	{
		const unsigned char isDot = fileName[i] == '.';

		if(isDot)
		{
			TextCopyA(fileName + i + 1, ExtensionMaxSize - i, extension, extensionMaxSize);
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
		const unsigned char isDot = fileName[i] == '.';

		if(isDot)
		{
			TextCopyA(extension + i, extensionMaxSize, fileNameCache, FileNameMaxSize);
			TextCopyA(fileNameCache, FileNameMaxSize, extension, extensionMaxSize);
			break;
		}
	}
#endif
}

void FilePathSplittW(const wchar_t* fullPath, PXSize fullPathMaxSize, wchar_t* drive, PXSize driveMaxSize, wchar_t* directory, PXSize directoryMaxSize, wchar_t* fileName, PXSize fileNameMaxSize, wchar_t* extension, PXSize extensionMaxSize)
{
#if OSUnix
	char fullPathA[PathMaxSize];
	char driveA[DriveMaxSize];
	char directoryA[DirectoryMaxSize];
	char fileNameA[FileNameMaxSize];
	char extensionA[ExtensionMaxSize];

	TextCopyWA(fullPath, PathMaxSize, fullPathA, PathMaxSize);

	FilePathSplittA
	(
		fullPathA,PathMaxSize,
		driveA,DriveMaxSize,
		directoryA,DirectoryMaxSize,
		fileNameA, FileNameMaxSize,
		extensionA, ExtensionMaxSize
	);

	TextCopyAW(driveA, DriveMaxSize, drive, DriveMaxSize);
	TextCopyAW(directoryA, DirectoryMaxSize, directory, DirectoryMaxSize);
	TextCopyAW(fileNameA, FileNameMaxSize, fileName, FileNameMaxSize);
	TextCopyAW(extensionA, ExtensionMaxSize, extension, ExtensionMaxSize);
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
			TextCopyW(extension + i + 1, extensionMaxSize, extensionCache, FileNameMaxSize);
			TextCopyW(extensionCache, FileNameMaxSize, extension, extensionMaxSize);
			break;
		}
	}
#endif
}

void FilePathSplittPositionW(const wchar_t* fullPath, PXSize fullPathMaxSize, PXSize* drivePos, PXSize driveSize, PXSize* directory, PXSize directorySize, PXSize* fileName, PXSize fileNameSize, PXSize* extension, PXSize extensionSize)
{
}

PXSize FilePathExtensionGetA(const char* filePath, const PXSize filePathSize, char* extension, const PXSize extensionSizeMax)
{
	const PXSize index = TextFindLastA(filePath, filePathSize, '.');
	const unsigned char hasExtension = index != TextIndexNotFound;

	if(!hasExtension)
	{
		TextClearA(extension, extensionSizeMax);
		return;
	}

	const PXSize writtenBytes = TextCopyA(filePath + index, filePathSize - index, extension, extensionSizeMax);

	return writtenBytes;
}

PXSize FilePathExtensionGetW(const wchar_t* filePath, const PXSize filePathSize, wchar_t* extension, const PXSize extensionSizeMax)
{
	const PXSize index = TextFindLastW(filePath, filePathSize, '.');
	const unsigned char hasExtension = index != TextIndexNotFound;

	if(!hasExtension)
	{
		TextClearW(extension, extensionSizeMax);
		return;
	}

	const PXSize writtenBytes = TextCopyW(filePath + index + 1, filePathSize - index, extension, extensionSizeMax);

	return writtenBytes;
}

FileFormatExtension FilePathExtensionDetectTryA(const char* const filePath, const PXSize filePathSize)
{
	char extensionA[ExtensionMaxSize];

	const PXSize writtenBytes = FilePathExtensionGetA(filePath, filePathSize, extensionA, ExtensionMaxSize);

	FileFormatExtension fileFormatExtension = FileExtensionDetectTryA(extensionA, writtenBytes);

	return fileFormatExtension;
}

FileFormatExtension FilePathExtensionDetectTryW(const wchar_t* const filePath, const PXSize filePathSize)
{
	wchar_t extensionW[ExtensionMaxSize];

	const PXSize writtenBytes = FilePathExtensionGetW(filePath, filePathSize, extensionW, ExtensionMaxSize);

	FileFormatExtension fileFormatExtension = FileExtensionDetectTryW(extensionW, writtenBytes);

	return fileFormatExtension;
}

FileFormatExtension FileExtensionDetectTryA(const char* const extension, const PXSize extensionSize)
{
	wchar_t extensionW[ExtensionMaxSize];

	const PXSize extensionWSize = TextCopyAW(extension, extensionSize, extensionW, ExtensionMaxSize);
	const FileFormatExtension format = FileExtensionDetectTryW(extensionW, extensionWSize);

	return format;
}

FileFormatExtension FileExtensionDetectTryW(const wchar_t* const extension, const PXSize extensionSizeC)
{
	const PXSize extensionSize = MathMinimumIU(extensionSizeC, ExtensionMaxSize);

	switch (extensionSize)
	{
		case 0:
			return FileFormatInvalid;

		case 1u:
		{
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "O", 1u))  return FileFormatLinuxExecutableAndLinkable;
			break;
		}
		case 2u:
		{
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "SO", 2u))  return FileFormatLinuxExecutableAndLinkable;
			break;
		}
		case 3u:
		{
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "ELF", 3u))  return FileFormatLinuxExecutableAndLinkable;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "OUT", 3u))  return FileFormatLinuxExecutableAndLinkable;

			if (TextCompareIgnoreCaseWA(extension, extensionSize, "FNT", 3u))  return FileFormatSpriteFont;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "GIF", 3u))  return FileFormatGIF;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "HTM", 3u))  return FileFormatHTML;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "INI", 3u))  return FileFormatINI;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "M4A", 3u))  return FileFormatM4A;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "3DS", 3u))  return FileFormatA3DS;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "ACC", 3u))  return FileFormatAAC;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "AVI", 3u))  return FileFormatAVI;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "BMP", 3u))  return FileFormatBitMap;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "CSS", 3u))  return FileFormatCSS;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "DLL", 3u))  return FileFormatWindowsDynamicLinkedLibrary;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "EML", 3u))  return FileFormatEML;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "EXE", 3u))  return FileFormatWindowsExecutable;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "FBX", 3u))  return FileFormatFimBox;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "MP3", 3u))  return FileFormatMP3;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "MP4", 3u))  return FileFormatMP4;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "MSI", 3u))  return FileFormatMSI;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "MTL", 3u))  return FileFormatMTL;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "OBJ", 3u))  return FileFormatOBJ;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "OGG", 3u))  return FileFormatOGG;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "PDF", 3u))  return FileFormatPDF;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "PHP", 3u))  return FileFormatPHP;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "PLY", 3u))  return FileFormatPLY;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "PNG", 3u))  return FileFormatPNG;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "QUI", 3u))  return FileFormatQOI;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "STL", 4u))  return FileFormatSTL;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "SVG", 4u))  return FileFormatSVG;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "TIF", 4u))  return FileFormatTagImage;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "TGA", 4u))  return FileFormatTGA;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "TTF", 4u))  return FileFormatTrueTypeFont;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "WAV", 4u))  return FileFormatWave;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "WMA", 4u))  return FileFormatWMA;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "XML", 3u))  return FileFormatXML;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "YML", 3u))  return FileFormatYAML;

			break;
		}
		case 4u:
		{
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "FLAC", 4u))  return FileFormatFLAC;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "MIDI", 4u))  return FileFormatMIDI;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "STEP", 4u))  return FileFormatSTEP;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "TIFF", 4u))  return FileFormatTagImage;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "JPEG", 4u))  return FileFormatJPEG;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "JSON", 4u))  return FileFormatJSON;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "VRML", 4u))  return FileFormatVRML;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "WEBM", 4u))  return FileFormatWEBM;
			if (TextCompareIgnoreCaseWA(extension, extensionSize, "WEBP", 4u))  return FileFormatWEBP;

			break;
		}
	}

	return FileFormatUnkown;
}

void PXFileConstruct(PXFile* const pxFile)
{
	MemoryClear(pxFile, sizeof(PXFile));
}

void PXFileDestruct(PXFile* const pxFile)
{
	
}

unsigned char FileDoesExistA(const char* filePath)
{
	return 0;
}

unsigned char FileDoesExistW(const wchar_t* filePath)
{
	return 0;
}

PXActionResult FileRemoveA(const char* filePath)
{
	int removeResult = OSFileRemoveA(filePath);
	//ErrorCode errorCode = ConvertErrorCode(removeResult);

	return PXActionInvalid;
}

PXActionResult FileRemoveW(const wchar_t* filePath)
{
	int removeResult = OSFileRemoveW(filePath);
	//ErrorCode errorCode = ConvertErrorCode(removeResult);

	return PXActionInvalid;
}

PXActionResult FileRenameA(const char* oldName, const char* newName)
{
	int renameResult = OSFileRenameA(oldName, newName);
	//ErrorCode errorCode = ConvertErrorCode(renameResult);

	return PXActionInvalid;
}

PXActionResult FileRenameW(const wchar_t* oldName, const wchar_t* newName)
{
	int renameResult = OSFileRenameW(oldName, newName);
	const unsigned char wasSuccesful = renameResult == 0;

	if(!wasSuccesful)
	{
		return PXActionInvalid;// GetCurrentError();
	}

	return PXActionSuccessful;
}

PXActionResult FileCopyA(const char* sourceFilePath, const char* destinationFilePath)
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

PXActionResult FileCopyW(const wchar_t* sourceFilePath, const wchar_t* destinationFilePath)
{
	if (!sourceFilePath) return PXActionRefuedInputBufferNull;
	if (!destinationFilePath) return PXActionRefuedOutputBufferNull;

#if OSUnix
	char sourceFilePathA[PathMaxSize];
	char destinationFilePathA[PathMaxSize];

	TextCopyWA(sourceFilePath, PathMaxSize, sourceFilePathA, PathMaxSize);
	TextCopyWA(destinationFilePath, PathMaxSize, destinationFilePathA, PathMaxSize);

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

void FilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName)
{
	const PXSize index = TextFindLastW(currnetPath, PathMaxSize, '/');
	const PXBool found = index != -1;

	if(found)
	{
		const PXSize copyedBytes = TextCopyW(currnetPath, index + 1, targetPath, index + 1);
		const PXSize toCopy = PathMaxSize - copyedBytes;

		TextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
	}
}

void FilePathSwapFileNameA(const PXTextASCII inputPath, PXTextASCII exportPath, const PXTextASCII fileName)
{
	PXCharASCII finalPath[PathMaxSize];

	PXCharASCII driveW[DriveMaxSize];
	PXCharASCII directoryW[DirectoryMaxSize];

	{
		PXCharASCII fileNameW[FileNameMaxSize];
		PXCharASCII extensionW[ExtensionMaxSize];

		FilePathSplittA
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

		offset += TextCopyA(driveW, DriveMaxSize, finalPath, PathMaxSize - offset);
		offset += TextCopyA(directoryW, DirectoryMaxSize, finalPath + offset, PathMaxSize - offset);
		offset += TextCopyA(fileName, PathMaxSize, finalPath + offset, PathMaxSize - offset);

		TextCopyA(finalPath, offset, exportPath, PathMaxSize);
	}
}

void FilePathSwapFileNameW(const PXTextUNICODE inputPath, PXTextUNICODE exportPath, const PXTextUNICODE fileName)
{
	wchar_t finalPath[PathMaxSize];

	wchar_t driveW[DriveMaxSize];
	wchar_t directoryW[DirectoryMaxSize];

	{
		wchar_t fileNameW[FileNameMaxSize];
		wchar_t extensionW[ExtensionMaxSize];

		FilePathSplittW
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

		offset += TextCopyW(driveW, DriveMaxSize, finalPath, PathMaxSize - offset);
		offset += TextCopyW(directoryW, DirectoryMaxSize, finalPath + offset, PathMaxSize - offset);
		offset += TextCopyW(fileName, PathMaxSize, finalPath + offset, PathMaxSize - offset);

		TextCopyW(finalPath, offset, exportPath, PathMaxSize);
	}
}

void FilePathSwapFileNameU(const PXTextUTF8 inputPath, PXTextUTF8 exportPath, const PXTextUTF8 fileName)
{
	FilePathSwapFileNameA(inputPath, exportPath, fileName);
}


void FilePathSwapExtensionA(const PXTextASCII inputPath, PXTextASCII exportPath, const PXTextASCII fileExtension)
{
	const PXSize index = TextFindLastA(inputPath, PathMaxSize, '.'); // Find last dot
	const PXBool found = index != -1;

	if (!found)
	{
		return;
	}

	const PXSize written = TextCopyA(inputPath, index, exportPath, PathMaxSize); // Copy filePath without extension
	const PXSize writtenFull = TextCopyA(fileExtension, PathMaxSize, &exportPath[written], PathMaxSize); // Copy extension on top
}

void FilePathSwapExtensionW(const PXTextUNICODE inputPath, PXTextUNICODE exportPath, const PXTextUNICODE fileExtension)
{
	const PXSize index = TextFindLastW(inputPath, PathMaxSize, '.'); // Find last dot
	const PXBool found = index != -1;

	if (!found)
	{
		return;
	}

	const PXSize written = TextCopyW(inputPath, index + 1, exportPath, PathMaxSize); // Copy filePath without extension
	const PXSize writtenFull = TextCopyW(fileExtension, PathMaxSize, &exportPath[written], PathMaxSize); // Copy extension on top
}

void FilePathSwapExtensionU(const PXTextUTF8 inputPath, PXTextUTF8 exportPath, const PXTextUTF8 fileExtension)
{
	FilePathSwapExtensionA(inputPath, exportPath, fileExtension);
}

PXActionResult DirectoryCreateA(const char* directoryName)
{
	const int creationResult = OSFileDirectoryCreateA(directoryName);
	const PXBool wasSuccesful = creationResult == 0;

	PXActionOnErrorFetchAndExit(!wasSuccesful);

	return PXActionSuccessful;
}

PXActionResult DirectoryCreateW(const wchar_t* directoryName)
{
	wchar_t directoryNameSegment[PathMaxSize];
	PXSize starPos = 0;
	PXSize successful = 0;

	do
	{
		PXSize offset = TextFindFirstW(directoryName + starPos, PathMaxSize - starPos, '/');

		if (offset == -1)
		{
			break;
		}

		TextCopyW(directoryName + starPos, offset-1, directoryNameSegment, PathMaxSize);

		const int creationResult = OSFileDirectoryCreateW(directoryNameSegment);
		const PXBool wasSuccesful = creationResult == 0;

		PXActionOnErrorFetchAndExit(!wasSuccesful);

		starPos += offset;
		++successful;
	}
	while (1);

	return PXActionSuccessful;
}

PXActionResult WorkingDirectoryChange(const char* directoryName)
{
	return PXActionInvalid;
}

PXActionResult WorkingDirectoryGetA(char* workingDirectory, PXSize workingDirectorySize)
{
	char* workingDirectoryResult = OSWorkingDirectoryCurrentA(workingDirectory, workingDirectorySize);
	const PXBool wasSuccesful = workingDirectoryResult;

	PXActionOnErrorFetchAndExit(!wasSuccesful);

	return PXActionSuccessful;
}

PXActionResult WorkingDirectoryGetW(wchar_t* workingDirectory, PXSize workingDirectorySize)
{
	wchar_t* workingDirectoryResult = OSWorkingDirectoryCurrentW(workingDirectory, workingDirectorySize);
	const PXBool wasSuccesful = workingDirectoryResult;

	PXActionOnErrorFetchAndExit(!wasSuccesful);

	return PXActionSuccessful;
}

PXActionResult WorkingDirectoryChangeW(const wchar_t* directoryName)
{
	return PXActionInvalid;
}

PXActionResult DirectoryDeleteA(const char* directoryName)
{
	return PXActionInvalid;
}

PXActionResult DirectoryDeleteW(const wchar_t* directoryName)
{
	return PXActionInvalid;
}

PXActionResult DirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize)
{
	return PXActionInvalid;
}

PXActionResult DirectoryFilesInFolderW(const wchar_t* folderPath, wchar_t*** list, PXSize* listSize)
{
	return PXActionInvalid;
}

PXActionResult PXFileNameA(PXFile* const pxFile, const PXTextASCII fileName, const PXSize fileNameSize, PXSize* const sizeWritten)
{
#if OSUnix

#elif OSWindows

	const DWORD flags = FILE_NAME_NORMALIZED | VOLUME_NAME_DOS; // FILE_NAME_NORMALIZED

	const DWORD result = GetFinalPathNameByHandleA(pxFile->Context, fileName, fileNameSize, flags);

	const char dosDriveTag[] = "\\\\?\\";

	const PXByte isSS = TextCompareA(fileName, 4u, dosDriveTag, 4u);

	if (isSS)
	{
		// Move my atleast 4.

		const char wind[] = "C:\\Windows\\System32\\";

		const PXByte isSSEE = TextCompareA(fileName+4u, 20u, wind, 20u);


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

PXActionResult FileMapToVirtualMemoryA(const char* filePath, const MemoryProtectionMode protectionMode)
{
}

PXActionResult FileMapToVirtualMemoryW(const wchar_t* filePath, const MemoryProtectionMode protectionMode)
{

}

PXActionResult FileMapToVirtualMemory(const PXSize size, const MemoryProtectionMode protectionMode)
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

PXActionResult FileWriteToDisk(const short value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const unsigned short value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const int value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const unsigned int value, const Endian endian)
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

PXActionResult FileWriteToDisk(const unsigned long long& value, const Endian endian)
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
	const PXSize index = TextFindLastW(currnetPath, PathMaxSize, '/');
	const bool found = index != -1;

	if (found)
	{
		const PXSize copyedBytes = TextCopyW(currnetPath, index + 1, targetPath, index + 1);
		const PXSize toCopy = PathMaxSize - copyedBytes;

		TextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
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
	PXSize writtenBytes = TextCopyAW(folderPath, PathMaxSize, folderPathW, PathMaxSize);

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

		TextCopyW(filePathSource, length, newString, length);

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

