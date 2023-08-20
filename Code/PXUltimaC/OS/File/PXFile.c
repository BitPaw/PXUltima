#include "PXFile.h"

#if PXFileUSE

#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>
#include <Media/PXResource.h>

#include <stdarg.h>
#include <fcntl.h>

#if OSUnix
#define PXPrintfvs vsnprintf
#elif OSWindows
// _O_RDONLY, _O_RDWR, _O_RDWR
#define PXPrintfvs vsprintf_s
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

void PXFilePathSplitt(const PXText* const fullPath, PXText* const drive, PXText* const directory, PXText* const fileName, PXText* const extension)
{
	switch (fullPath->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
#if OSUnix

#if 0
			char directoryNameCache[PathMaxSize];
			char baseNameCache[FileNameMaxSize];

			PXTextCopyA(fullPath, FileNameMaxSize, directoryNameCache, FileNameMaxSize);
			PXTextCopyA(fullPath, FileNameMaxSize, baseNameCache, FileNameMaxSize);

			char* dirNameResult = dirname(directoryNameCache);
			char* baseNameResult = basename(baseNameCache);

			PXSize directoryLength = PXTextCopyA(dirNameResult, DirectoryMaxSize, directory, DirectoryMaxSize);
			PXSize fileNameLength = PXTextCopyA(baseNameResult, FileNameMaxSize, fileName, FileNameMaxSize);

			for (PXSize i = fileNameLength - 1; i > 0; --i)
			{
				const PXBool isDot = fileName[i] == '.';

				if (isDot)
				{
					PXTextCopyA(fileName + i + 1, ExtensionMaxSize - i, extension, extensionMaxSize);
					break;
				}
			}

#endif // 0

#elif OSWindows
			char fileNameCache[FileNameMaxSize];

			_splitpath_s
			(
				fullPath->TextA,
				drive->TextA, drive->SizeAllocated,
				directory->TextA, directory->SizeAllocated,
				fileName->TextA, fileName->SizeAllocated,
				extension->TextA, extension->SizeAllocated
			);

			for (PXSize i = 0; fileName->TextA[i] != '\0'; i++)
			{
				const PXBool isDot = fileName->TextA[i] == '.';

				if (isDot)
				{
					PXTextCopyA(extension->TextA + i, extension->SizeAllocated, fileNameCache, FileNameMaxSize);
					PXTextCopyA(fileNameCache, FileNameMaxSize, extension->TextA, extension->SizeAllocated);
					break;
				}
			}
#endif

			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix
			char fullPathA[PathMaxSize];
			char driveA[DriveMaxSize];
			char directoryA[DirectoryMaxSize];
			char fileNameA[FileNameMaxSize];
			char extensionA[ExtensionMaxSize];

			PXTextCopyWA(fullPath, PathMaxSize, fullPathA, PathMaxSize);
			
			/* ???? 
			PXFilePathSplitt
			(
				fullPathA, PathMaxSize,
				driveA, DriveMaxSize,
				directoryA, DirectoryMaxSize,
				fileNameA, FileNameMaxSize,
				extensionA, ExtensionMaxSize
			);*/

			PXTextCopyAW(driveA, DriveMaxSize, drive, DriveMaxSize);
			PXTextCopyAW(directoryA, DirectoryMaxSize, directory, DirectoryMaxSize);
			PXTextCopyAW(fileNameA, FileNameMaxSize, fileName, FileNameMaxSize);
			PXTextCopyAW(extensionA, ExtensionMaxSize, extension, ExtensionMaxSize);
#elif OSWindows
			wchar_t extensionCache[FileNameMaxSize];

			_wsplitpath_s
			(
				fullPath,
				drive->TextW, drive->SizeAllocated,
				directory->TextW, directory->SizeAllocated,
				fileName->TextW, fileName->SizeAllocated,
				extension->TextW, extension->SizeAllocated
			);

			for (PXSize i = 0; extension->TextW[i] != '\0'; i++)
			{
				const unsigned char isDot = extension->TextW[i] == '.';

				if (isDot)
				{
					PXTextCopyW(extension + i + 1, extension->SizeAllocated, extensionCache, FileNameMaxSize);
					PXTextCopyW(extensionCache, FileNameMaxSize, extension, extension->SizeAllocated);
					break;
				}
			}
#endif

			break;
		}
	}
}

void PXFilePathSplittPositionW(const wchar_t* fullPath, PXSize fullPathMaxSize, PXSize* drivePos, PXSize driveSize, PXSize* directory, PXSize directorySize, PXSize* fileName, PXSize fileNameSize, PXSize* extension, PXSize extensionSize)
{
}

PXSize PXFilePathExtensionGet(const PXText* const filePath, PXText* const extension)
{
	PXText stringResult;
	PXText pxTarget;
	PXTextMakeFixedC(&pxTarget, '.');

	const PXSize index = PXTextFindLast(filePath, &pxTarget, &stringResult);
	const PXBool hasExtension = index != PXTextIndexNotFound;

	if (!hasExtension)
	{
		PXTextClear(extension);
		return 0;
	}

	PXTextMoveByOffset(&stringResult, 1u);

	const PXSize writtenBytes = PXTextCopy(&stringResult, extension);

	return writtenBytes;
}

PXFileFormat PXFilePathExtensionDetectTry(const PXText* const filePath)
{
	PXText pxText;
	PXTextConstructNamedBufferA(&pxText, extensionBuffer, ExtensionMaxSize);

	const PXSize writtenBytes = PXFilePathExtensionGet(filePath, &pxText);

	PXTextToUpperCase(&pxText, &pxText);

	switch (writtenBytes)
	{
		case 0:
			return PXFileFormatInvalid;

		case 1u:
		{
			switch (*pxText.TextA)
			{
				case 'O': return PXFileFormatLinuxExecutableAndLinkable;
			}

			break;
		}
		case 2u:
		{
			const PXInt32U list = PXInt16FromAdress(pxText.TextA);

			switch (list)
			{
				case PXInt16Make('S', 'O'): return PXFileFormatLinuxExecutableAndLinkable;
			}

			break;
		}
		case 3u:
		{
			const PXInt32U list = PXInt24FromAdress(pxText.TextA);

			switch (list)
			{
				case PXInt24Make('E', 'L', 'F'): return PXFileFormatLinuxExecutableAndLinkable;
				case PXInt24Make('O', 'U', 'T'): return PXFileFormatLinuxExecutableAndLinkable;
				case PXInt24Make('F', 'N', 'T'): return PXFileFormatSpriteFont;
				case PXInt24Make('G', 'I', 'F'): return PXFileFormatGIF;
				case PXInt24Make('H', 'T', 'M'): return PXFileFormatHTML;
				case PXInt24Make('I', 'N', 'I'): return PXFileFormatINI;
				case PXInt24Make('M', '4', 'A'): return PXFileFormatM4A;
				case PXInt24Make('3', 'D', 'S'): return PXFileFormatA3DS;
				case PXInt24Make('A', 'C', 'C'): return PXFileFormatAAC;
				case PXInt24Make('A', 'V', 'I'): return PXFileFormatAVI;
				case PXInt24Make('B', 'M', 'P'): return PXFileFormatBitMap;
				case PXInt24Make('C', 'S', 'S'): return PXFileFormatCSS;
				case PXInt24Make('D', 'L', 'L'): return PXFileFormatWindowsDynamicLinkedLibrary;
				case PXInt24Make('E', 'M', 'L'): return PXFileFormatEML;
				case PXInt24Make('E', 'X', 'E'): return PXFileFormatWindowsExecutable;
				case PXInt24Make('F', 'B', 'X'): return PXFileFormatFilmBox;
				case PXInt24Make('M', 'P', '3'): return PXFileFormatMP3;
				case PXInt24Make('M', 'P', '4'): return PXFileFormatMP4;
				case PXInt24Make('M', 'S', 'I'): return PXFileFormatMSI;
				case PXInt24Make('M', 'T', 'L'): return PXFileFormatMTL;
				case PXInt24Make('O', 'B', 'J'): return PXFileFormatWavefront;
				case PXInt24Make('O', 'G', 'G'): return PXFileFormatOGG;
				case PXInt24Make('P', 'D', 'F'): return PXFileFormatPDF;
				case PXInt24Make('P', 'H', 'P'): return PXFileFormatPHP;
				case PXInt24Make('P', 'L', 'Y'): return PXFileFormatPLY;
				case PXInt24Make('P', 'N', 'G'): return PXFileFormatPNG;
				case PXInt24Make('Q', 'U', 'I'): return PXFileFormatQOI;
				case PXInt24Make('S', 'T', 'L'): return PXFileFormatSTL;
				case PXInt24Make('S', 'V', 'G'): return PXFileFormatSVG;
				case PXInt24Make('T', 'I', 'F'): return PXFileFormatTagImage;
				case PXInt24Make('T', 'G', 'A'): return PXFileFormatTGA;
				case PXInt24Make('T', 'T', 'F'): return PXFileFormatTrueTypeFont;
				case PXInt24Make('W', 'A', 'V'): return PXFileFormatWave;
				case PXInt24Make('W', 'M', 'A'): return PXFileFormatWMA;
				case PXInt24Make('X', 'M', 'L'): return PXFileFormatXML;
				case PXInt24Make('Y', 'M', 'L'): return PXFileFormatYAML;
			}

			break;
		}
		case 4u:
		{
			const PXInt32U list = PXInt32FromAdress(pxText.TextA);

			switch (list)
			{
				case PXInt32Make('F', 'L', 'A', 'C'): return PXFileFormatFLAC;
				case PXInt32Make('M', 'I', 'D', 'I'): return PXFileFormatMIDI;
				case PXInt32Make('S', 'T', 'E', 'P'): return PXFileFormatSTEP;
				case PXInt32Make('T', 'I', 'F', 'F'): return PXFileFormatTagImage;
				case PXInt32Make('J', 'P', 'E', 'G'): return PXFileFormatJPEG;
				case PXInt32Make('J', 'S', 'O', 'N'): return PXFileFormatJSON;
				case PXInt32Make('V', 'R', 'M', 'L'): return PXFileFormatVRML;
				case PXInt32Make('W', 'E', 'B', 'M'): return PXFileFormatWEBM;
				case PXInt32Make('W', 'E', 'B', 'P'): return PXFileFormatWEBP;
			}

			break;
		}
	}

	return PXFileFormatUnkown;
}

PXBool PXFileDoesExist(const PXText* const filePath)
{
	if (!filePath)
	{
		return PXFalse;
	}

	switch (filePath->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix

#elif OSWindows
			const DWORD dwAttrib = GetFileAttributesA(filePath->TextA); // Windows XP, Kernel32.dll, fileapi.h
			const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
			const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

			if (doesFileExists && ifFile)
			{
				return PXTrue;
			}
#endif

			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix

#elif OSWindows
			const DWORD dwAttrib = GetFileAttributesW(filePath->TextW); // Windows XP, Kernel32.dll, fileapi.h
			const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
			const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

			if (doesFileExists && ifFile)
			{
				return PXTrue;
			}

#endif

			break;
		}
		default:
		{
			return PXActionRefusedInvalidFilePath;
		}
	}

	return PXFalse;
}

PXActionResult PXFileRemove(const PXText* const filePath)
{
	if (!filePath)
	{
		return PXActionRefusedEmptyFilePath;
	}

	switch (filePath->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix || OSForcePOSIXForWindows
			const int resultID = remove(filePath->TextA);
			const PXBool success = 0 == resultID;
#elif OSWindows
			const PXBool success = DeleteFileA(filePath->TextA);
#endif

			PXActionOnErrorFetchAndReturn(!success);

			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix

#elif OSWindows

#if OSForcePOSIXForWindows
			const int resultID = _wremove(filePath->TextW);
			const PXBool success = 0 == resultID;
#else
			const PXBool success = DeleteFileW(filePath->TextW);
#endif

			PXActionOnErrorFetchAndReturn(!success);

#endif

			break;
		}
		default:
		{
			return PXActionRefusedInvalidFilePath;
		}
	}

	return PXActionSuccessful;
}

PXActionResult PXFileRename(const PXText* const oldName, const PXText* const newName)
{
	if (!(oldName && newName))
	{
		return PXActionRefusedEmptyFilePath;
	}

	switch (oldName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix || OSForcePOSIXForWindows || PXOSWindowsUseUWP
			const int resultID = rename(oldName->TextA, newName->TextA); // [POSIX]
			const PXBool success = 0 == resultID;
#elif OSWindows
			const PXBool success = MoveFileA(oldName->TextA, newName->TextA); // Windows XP, Kernel32.dll, winbase.h
#endif

			PXActionOnErrorFetchAndReturn(!success);

			return PXActionSuccessful;
		}
		case TextFormatUNICODE:
		{
#if OSUnix

#elif OSWindows

#if OSForcePOSIXForWindows || PXOSWindowsUseUWP
			const int resultID = _wrename(oldName->TextW, newName->TextW); // [POSIX]
			const PXBool success = 0 == resultID;
#else
			const PXBool success = MoveFileW(oldName->TextW, newName->TextW); // Windows XP, Kernel32.dll, winbase.h
#endif

			PXActionOnErrorFetchAndReturn(!success);

			return PXActionSuccessful;

#endif
		}
		default:
		{
			return PXActionRefusedInvalidFilePath;
		}
	}
}

PXActionResult PXFileCopy(const PXText* const sourceFilePath, const PXText* const destinationFilePath, const PXBool overrideIfExists)
{
	if (!sourceFilePath) return PXActionRefuedInputBufferNull;
	if (!destinationFilePath) return PXActionRefuedOutputBufferNull;

	switch (sourceFilePath->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix || OSForcePOSIXForWindows
			const FILE* const fileSource = fopen(sourceFilePath, FileReadMode);
			const FILE* const fileDestination = fopen(destinationFilePath, FileWriteMode);
			const PXBool fileOpenSuccesful = fileSource && fileDestination;

			const PXSize swapBufferSize = 1024;
			PXByte swapBuffer[1024];

			if (!fileOpenSuccesful)
			{
				return PXActionFailedFileOpen;
			}

			while (!feof(fileSource))
			{
				const PXSize readBytes = fread(swapBuffer, sizeof(char), swapBufferSize, fileSource);
				const PXSize writtenBytes = fwrite(swapBuffer, sizeof(char), readBytes, fileDestination);
			}

			const int closeA = fclose(fileSource);
			const int closeB = fclose(fileDestination);
#elif OSWindows
			const PXBool succesfull = CopyFileA(sourceFilePath->TextA, destinationFilePath->TextA, overrideIfExists); // Windows XP (+UWP), Kernel32.dll, winbase.h

			PXActionOnErrorFetchAndReturn(!succesfull);

			return PXActionSuccessful;
#endif

			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix
			// Convert
			return PXActionRefusedNotImplemented;

#elif OSWindows
			const PXBool succesfull = CopyFileW(sourceFilePath->TextW, destinationFilePath->TextW, overrideIfExists); // Windows XP (+UWP), Kernel32.dll, winbase.h

			PXActionOnErrorFetchAndReturn(!succesfull);

			return PXActionSuccessful;
#endif
		}
		default:
		{
			return PXActionRefuedTextFormatUnsupported;
		}
	}
}

/*
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
}*/

void PXFilePathSwapFileName(const PXText* const inputPath, PXText* const exportPath, const PXText* const fileName)
{
	switch (inputPath->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
			PXText drive;
			PXTextConstructNamedBufferA(&drive, driveBuffer, DriveMaxSize);

			PXText directory;
			PXTextConstructNamedBufferA(&directory, directoryBuffer, DirectoryMaxSize);

			{
				PXText fileName;
				PXTextConstructNamedBufferA(&fileName, fileNameBuffer, FileNameMaxSize);

				PXText extension;
				PXTextConstructNamedBufferA(&extension, extensionBuffer, ExtensionMaxSize);

				PXFilePathSplitt(inputPath, &drive, &directory, &fileName, &extension);
			}

			{
				exportPath->SizeUsed = 0;
				exportPath->SizeUsed += PXTextCopyA(drive.TextA, drive.SizeAllocated, exportPath->TextA, exportPath->SizeAllocated - exportPath->SizeUsed);
				exportPath->SizeUsed += PXTextCopyA(directory.TextA, directory.SizeAllocated, exportPath->TextA + exportPath->SizeUsed, exportPath->SizeAllocated - exportPath->SizeUsed);
				exportPath->SizeUsed += PXTextCopyA(fileName->TextA, fileName->SizeAllocated, exportPath->TextA + exportPath->SizeUsed, exportPath->SizeAllocated - exportPath->SizeUsed);

				//PXTextCopyA(fileName->TextA, offset, exportPath, PathMaxSize);
			}

			break;
		}

		case TextFormatUNICODE:
		{
			/*
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
			}*/

			break;
		}
	}
}

void PXFilePathSwapExtension(const PXText* const inputPath, PXText* const exportPath)
{
	PXText pxResult;
	PXText pxText;

	PXTextMakeFixedC(&pxText, '.');

	const PXSize index = PXTextFindLast(inputPath, &pxText, &pxResult); // Find last dot
	const PXBool found = index != -1;

	if (!found)
	{
		return;
	}

	PXTextMoveByOffset(&pxResult, 1u);

	switch (inputPath->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
			const PXSize written = PXTextCopyA(inputPath->TextA, index, exportPath->TextA, PathMaxSize); // Copy filePath without extension
			const PXSize writtenFull = PXTextCopyA(pxResult.TextA, PathMaxSize, &exportPath->TextA[written], PathMaxSize); // Copy extension on top

			break;
		}

		case TextFormatUNICODE:
		{
			const PXSize written = PXTextCopyW(inputPath->TextW, index + 1, exportPath->TextW, PathMaxSize); // Copy filePath without extension
			const PXSize writtenFull = PXTextCopyW(pxResult.TextW, PathMaxSize, &exportPath->TextW[written], PathMaxSize); // Copy extension on top

			break;
		}
	}
}

PXActionResult PXFileName(const PXFile* const pxFile, PXText* const fileName)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows

#if WindowsAtleastVista

	const DWORD flags = FILE_NAME_NORMALIZED | VOLUME_NAME_DOS; // FILE_NAME_NORMALIZED

	const DWORD result = GetFinalPathNameByHandleA(pxFile->ID, fileName->TextA, fileName->SizeAllocated, flags);

	const char dosDriveTag[] = "\\\\?\\";

	const PXByte isSS = PXTextCompareA(fileName->TextA, 4u, dosDriveTag, 4u);

	if (isSS)
	{
		// Move my atleast 4.

		const char wind[] = "C:\\Windows\\System32\\";

		const PXByte isSSEE = PXTextCompareA(fileName->TextA + 4u, 20u, wind, 20u);


		char* texxxx = fileName->TextA + 4u;
		PXSize xxxxxy = result - 4u;

		if (isSSEE)
		{
			texxxx = fileName->TextA + 24u;
			xxxxxy = result - 24u;
		}
		else
		{
			texxxx = fileName->TextA + 4u;
			xxxxxy = result - 4u;
		}

		const PXSize rerere = PXMemoryMove(texxxx, xxxxxy, fileName, result);


		fileName->SizeUsed = rerere;

		fileName->TextW[rerere] = '\0';

	}
	else
	{
		fileName->SizeUsed = result;
	}
#else
	return PXActionRefusedNotSupported;
#endif

#endif

	return PXActionSuccessful;
}

PXBool PXFileDirectoryPathExtract(const PXFile* const path, PXFile* const directoryPath)
{
	PXText stringTarget;
	PXSize size;
	PXBool found;

	{
		PXTextMakeFixedC(&stringTarget, '/');

		size = PXTextFindLast(path, &stringTarget, &directoryPath);
		found = size != -1;

		if (found)
		{
			return PXTrue;
		}
	}

	{
		PXTextMakeFixedC(&stringTarget, '\\');

		size = PXTextFindLast(path, &stringTarget, &directoryPath);
		found = size != -1;
	}

	return found;
}

void PXFileConstruct(PXFile* const pxFile)
{
	PXMemoryClear(pxFile, sizeof(PXFile));

	pxFile->ID = PXHandleNotSet;
}

void PXFileDestruct(PXFile* const pxFile)
{
	switch (pxFile->LocationMode)
	{
		case PXFileLocationModeMappedFromDisk:
			PXFileUnmapFromMemory(pxFile);
			break;

		case PXFileLocationModeInternal:
			PXMemoryRelease(pxFile->Data, pxFile->DataSize);
			break;

		case PXFileLocationModeMappedVirtual:
			PXMemoryVirtualRelease(pxFile->Data, pxFile->DataSize);
			break;

		case PXFileLocationModeDirectCached:
			PXMemoryRelease(pxFile->Data, pxFile->DataSize);
			PXFileClose(pxFile);
			break;

		case PXFileLocationModeDirectUncached:
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
			return 0;//POSIX_FADV_NORMAL;
#elif OSWindows
			return 0;
#endif
		case PXMemoryCachingModeRandom:
#if OSUnix
			return 0;//POSIX_FADV_RANDOM;
#elif OSWindows
			return FILE_FLAG_RANDOM_ACCESS;
#endif
		case PXMemoryCachingModeSequential:
#if OSUnix
			return 0;//POSIX_FADV_SEQUENTIAL;
#elif OSWindows
			return FILE_FLAG_SEQUENTIAL_SCAN;
#endif

#if OSWindows

		case PXMemoryCachingModeTemporary:
		case PXMemoryCachingModeUseOnce:
			return FILE_FLAG_DELETE_ON_CLOSE;

			// Windows only
		case PXMemoryCachingModeWriteThrough:
			return FILE_FLAG_WRITE_THROUGH;

		case PXMemoryCachingModeNoBuffering:
			return FILE_FLAG_NO_BUFFERING;
#endif // OSWindows

#if OSUnix
			// UNIX only
		case PXMemoryCachingModeNeedLater:
			return 0;//POSIX_FADV_WILLNEED;

		case PXMemoryCachingModeDontNeedNow:
			return 0;//POSIX_FADV_DONTNEED;
#endif
	}
}

void PXFileBufferAllocate(PXFile* const pxFile, const PXSize dataSize)
{
	PXFileConstruct(pxFile);

	pxFile->Data = PXMemoryAllocate(dataSize);
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

PXActionResult PXFileOpenFromPath(PXFile* const pxFile, const PXFileOpenFromPathInfo* const pxFileOpenFromPathInfo)
{
	PXFileConstruct(pxFile);

	// Analyse type
	{
		PXFileTypeInfoProbe(&pxFile->TypeInfo, &pxFileOpenFromPathInfo->Text);
	}


	// Does file even exist?
	{
		const PXBool doesFileExists = PXFileDoesExist(&pxFileOpenFromPathInfo->Text);

		if (!doesFileExists)
		{
			return PXActionFailedFileNotFound;
		}
	}


#if OSUnix || OSForcePOSIXForWindows || PXOSWindowsUseUWP
	const char* readMode = 0u;

	switch (pxFileOpenFromPathInfo->AccessMode)
	{
		case PXMemoryAccessModeReadOnly:
			readMode = "rb";
			break;

		case PXMemoryAccessModeWriteOnly:
			readMode = "wb";
			break;
		default:
			return PXFalse;
	}

	// Use this somewhere here
	// int posix_fadvise(int fd, off_t offset, off_t len, int advice);
	// int posix_fadvise64(int fd, off_t offset, off_t len, int advice);

#if CVersionNewerThen2011 && OSWindows
	const auto result = fopen_s(&pxFile->ID, pxFileOpenFromPathInfo->Text.TextA, readMode); // errno_t

	return result == 0;

#else
	pxFile->ID = fopen(pxFileOpenFromPathInfo->Text.TextA, readMode);

	return pxFile->ID ? PXActionSuccessful : PXActionFailedFileOpen;
#endif




#elif OSWindows
	DWORD desiredAccess = 0;
	DWORD shareMode = 0;
	DWORD creationDisposition = 0;
	DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE templateFile = PXNull;

	switch (pxFileOpenFromPathInfo->AccessMode)
	{
		case PXMemoryAccessModeReadOnly:
		{
			shareMode = FILE_SHARE_READ;
			creationDisposition = OPEN_EXISTING;
			desiredAccess = GENERIC_READ;
			break;
		}
		case PXMemoryAccessModeWriteOnly:
		{
			creationDisposition = CREATE_ALWAYS;
			desiredAccess = GENERIC_READ | GENERIC_WRITE;
			break;
		}
		case PXMemoryAccessModeReadAndWrite:
		{
			creationDisposition = CREATE_ALWAYS;
			desiredAccess = GENERIC_READ | GENERIC_WRITE;
			break;
		}
	}

	//creationDisposition |= PXFileMemoryCachingModeConvertToID(pxFileOpenFromPathInfo->MemoryCachingMode);


	// Make directory if needed
	// FilePathExtensionGetW
	if (pxFileOpenFromPathInfo->AccessMode == PXMemoryAccessModeWriteOnly || pxFileOpenFromPathInfo->AccessMode == PXMemoryAccessModeReadAndWrite)
	{
		//const PXActionResult directoryCreateResult = DirectoryCreateA(filePath);

		//PXActionExitOnError(directoryCreateResult);
	}




	// Open file
	{
		HANDLE fileHandle = PXNull;
		SECURITY_ATTRIBUTES* securityAttributes = PXNull;

		switch (pxFileOpenFromPathInfo->Text.Format)
		{
			case TextFormatASCII:
			case TextFormatUTF8:
			{
				const DWORD dwAttrib = GetFileAttributesA(pxFileOpenFromPathInfo->Text.TextA); // Windows XP (+UWP), Kernel32.dll, fileapi.h
				const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
				const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

				if (!doesFileExists)
				{
					return PXActionFailedFileNotFound;
				}

				if (!ifFile)
				{
					return PXActionFailedNotAFile;
				}

				fileHandle = CreateFileA // Windows XP, Kernel32.dll, fileapi.h
				(
					pxFileOpenFromPathInfo->Text.TextA,
					desiredAccess,
					shareMode,
					securityAttributes,
					creationDisposition,
					flagsAndAttributes,
					templateFile
				);
				break;
			}
			case TextFormatUNICODE:
			{
				const DWORD dwAttrib = GetFileAttributesW(pxFileOpenFromPathInfo->Text.TextW); // Windows XP (+UWP), Kernel32.dll, fileapi.h
				const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
				const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

				if (!doesFileExists)
				{
					return PXActionFailedFileNotFound;
				}

				if (!ifFile)
				{
					return PXActionFailedNotAFile;
				}

				fileHandle = CreateFileW // Windows XP, Kernel32.dll, fileapi.h
				(
					pxFileOpenFromPathInfo->Text.TextW,
					desiredAccess,
					shareMode,
					securityAttributes,
					creationDisposition,
					flagsAndAttributes,
					templateFile
				);
				break;
			}
			default:
			{
				return PXActionRefusedInvalidFilePath;
			}
		}

		const PXBool successful = fileHandle != INVALID_HANDLE_VALUE;

		PXActionOnErrorFetchAndReturn(!successful);

		pxFile->ID = fileHandle;
		pxFile->AccessMode = pxFileOpenFromPathInfo->AccessMode;
		pxFile->CachingMode = pxFileOpenFromPathInfo->MemoryCachingMode;
		pxFile->LocationMode = PXFileLocationModeDirectUncached;
	}

	// Get file size
	{
		LARGE_INTEGER largeInt;

		const BOOL sizeResult = GetFileSizeEx(pxFile->ID, &largeInt); // Windows XP, Kernel32.dll, fileapi.h

		pxFile->DataSize = largeInt.QuadPart;
	}


	// File is now opened.
	// Can we map the whole file into memory?

	if (!pxFileOpenFromPathInfo->AllowMapping)
	{
		return PXActionSuccessful; // No mapping attempt, we are done
	}



	// Attempt mappiung

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

	// Create mapping
	{
		DWORD flProtect = SEC_COMMIT;
		DWORD dwMaximumSizeHigh = 0;
		DWORD dwMaximumSizeLow = 0; // Problem if file is 0 Length

#if OS32Bit
		dwMaximumSizeHigh = 0;
		dwMaximumSizeLow = pxFile->DataSize;
#elif OS64Bit
		dwMaximumSizeHigh = (pxFile->DataSize & 0xFFFFFFFF00000000) >> 32u;
		dwMaximumSizeLow = pxFile->DataSize & 0x00000000FFFFFFFF;
#endif

		switch (pxFileOpenFromPathInfo->AccessMode)
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
			PXNull, // No security attributes
			flProtect,
			dwMaximumSizeHigh,
			dwMaximumSizeLow,
			PXNull // No Name
		);

		// check if successful
		{
			const PXBool successful = fileMappingHandleResult != PXNull;

			if (successful)
			{
				pxFile->MappingID = fileMappingHandleResult; // Mapping [OK]

				{
					DWORD desiredAccess = 0;
					DWORD fileOffsetHigh = 0;
					DWORD fileOffsetLow = 0;
					PXSize numberOfBytesToMap = 0;
					void* baseAddressTarget = 0;
					//DWORD  numaNodePreferred = -1; // (NUMA_NO_PREFERRED_NODE)

					switch (pxFileOpenFromPathInfo->AccessMode)
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
#if WindowsAtleastVista && 0
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
					pxFile->DataAllocated = pxFile->DataSize;

					PXMemoryVirtualPrefetch(fileMapped, pxFile->DataSize);
				}

#endif

				pxFile->LocationMode = PXFileLocationModeMappedFromDisk;

#if MemoryDebugOutput
				printf("[#][Memory] 0x%p (%10zi B) MMAP %ls\n", pxFile->Data, pxFile->DataSize, filePath);
#endif

				return PXActionSuccessful;
			}
		}
	}
#endif

	// Filemapping failed, maybe due to lack of memory. File exists and is loaded, so we proceed normally.


	// Try allocating cache. To fetch data into
#if 0
	pxFile->DataAllocated = 420;
	pxFile->Data = MemoryAllocate(420);
	pxFile->LocationMode = PXFileLocationModeDirectCached;
#endif

	// Mapping and allocating failed, we now read/write direct from/to file.

	// We dont do anything



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
}

PXActionResult PXFileOpenTemporal(PXFile* const pxFile, const PXSize expectedFileSize)
{
	PXFileConstruct(pxFile);

	{
		// Use virual memory if possible
		void* virtualAdress = PXMemoryVirtualAllocate(expectedFileSize, PXMemoryAccessModeReadAndWrite);

		if (virtualAdress)
		{
			// Allocation successful

			pxFile->Data = virtualAdress;
			pxFile->DataSize = expectedFileSize;
			pxFile->DataAllocated = expectedFileSize;
			pxFile->AccessMode = PXMemoryAccessModeReadAndWrite;
			pxFile->CachingMode = PXMemoryCachingModeSequential;
			pxFile->LocationMode = PXFileLocationModeMappedVirtual;

			return PXActionSuccessful;
		}
	}

	// If no virual memory can or should be used, we need to make a temp-file

#if OSUnix
#elif OSWindows
	
	PXText tempFileFullPath;
	PXTextConstructNamedBufferW(&tempFileFullPath, tempFileFullPathBuffer, MAX_PATH);
	
	{
		PXText tempPath;
		PXTextConstructNamedBufferW(&tempPath, tempPathBuffer, MAX_PATH);

		// Gets the temp path env string (no guarantee it's a valid path).
		tempPath.SizeUsed = GetTempPathW(tempPath.SizeAllocated/2, tempPath.TextW); // Windows XP (+UWP), Kernel32.dll, fileapi.h

		const PXBool successfulTempPathFetch = tempPath.SizeUsed > 0;


		// Generates a temporary file name. 
		tempFileFullPath.SizeUsed = GetTempFileNameW
		(
			tempPath.TextW, // directory for tmp files
			L"PXUltima",     // temp file name prefix 
			0,                // create unique name 
			tempFileFullPath.TextW // buffer for name 
		);



		const PXBool successfulTempPathCreate = tempFileFullPath.SizeUsed > 0;
	}
	
	pxFile->ID = CreateFileW  // Windows XP, Kernel32.dll, fileapi.h
	(
		tempFileFullPath.TextW, // file name 
		GENERIC_ALL,			// open for write 
		0,						// do not share 
		NULL,					// default security 
		CREATE_ALWAYS,			// overwrite existing
		FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,// normal file 
		NULL					// no template 
	);              


	pxFile->AccessMode = PXMemoryAccessModeReadAndWrite;
	pxFile->CachingMode = PXMemoryCachingModeTemporary;
	pxFile->LocationMode = PXFileLocationModeDirectCached;


#else
#endif

	return PXActionSuccessful;
}

PXActionResult PXFileClose(PXFile* const pxFile)
{
#if OSUnix
	const int closeResult = fclose(pxFile->ID);

	switch (closeResult)
	{
		case 0:
			pxFile->ID = 0;
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

	if (pxFile->ID != PXHandleNotSet)
	{
		const PXBool successful = CloseHandle(pxFile->ID); // Windows 2000 (+UWP), Kernel32.dll, handleapi.h

		if (!successful)
		{
			return PXActionFailedFileClose;
		}

		pxFile->ID = PXHandleNotSet;
	}

	return PXActionSuccessful;
#endif
}

PXActionResult PXFileMapToMemory(PXFile* const pxFile, const PXSize size, const PXMemoryAccessMode protectionMode)
{
	void* const data = PXMemoryVirtualAllocate(size, protectionMode);
	const PXBool successful = data != 0;

	if (!successful)
	{
		return PXActionFailedMemoryAllocation;
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

	PXActionOnErrorFetchAndReturn(!sucessful);

	pxFile->Data = PXNull;
	pxFile->DataSize = 0;

	return PXActionSuccessful;

#elif OSWindows

	// Write pending data
	{
		if (isWriteMapped)
		{
			const BOOL flushSuccessful = FlushViewOfFile(pxFile->Data, pxFile->DataCursor);

			PXActionOnErrorFetchAndReturn(flushSuccessful);
		}
	}

	{
		const PXBool unmappingSucessful = UnmapViewOfFile(pxFile->Data);

		PXActionOnErrorFetchAndReturn(unmappingSucessful);

		pxFile->Data = PXNull;
	}

	{
		const PXBool closeMappingSucessful = CloseHandle(pxFile->MappingID);

		PXActionOnErrorFetchAndReturn(closeMappingSucessful);

		pxFile->MappingID = PXHandleNotSet;
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

		PXActionReturnOnError(closeFile);

		pxFile->ID = PXHandleNotSet;
	}

	return PXActionSuccessful;
#endif
}

PXSize PXFileRemainingSize(const PXFile* const pxFile)
{
	return pxFile->DataSize - pxFile->DataCursor;
}

PXSize PXFileRemainingSizeRelativeFromAddress(const PXFile* const pxFile, const void* const adress)
{
	return ((PXSize)pxFile->Data - (PXSize)adress) - pxFile->DataSize;
}

PXBool PXFileIsAtEnd(const PXFile* const pxFile)
{
	return pxFile->DataCursor >= pxFile->DataSize;
}

void* PXFileCursorPosition(PXFile* const pxFile)
{
	switch (pxFile->LocationMode)
	{
		case PXFileLocationModeInternal: // Memory is handled internally.
		case PXFileLocationModeExternal: // Memory is stored outside this object
		case PXFileLocationModeMappedVirtual: // Used 'VirtalAlloc()' / 'mmap()'
		case PXFileLocationModeMappedFromDisk: // Used 'FileView()' / 'fmap()'
			return (PXAdress)pxFile->Data + pxFile->DataCursor;

		case PXFileLocationModeDirectCached: // Read & Write operations are cached into a buffer first.
		case PXFileLocationModeDirectUncached: // Read & Write operations are directly put into
		{
#if OSUnix
			pxFile->DataCursor = ftell(pxFile->ID);
#elif OSWindows
			// Compile-ERROR: This seems to upset the compiler as its limited to 32-Bit value
			//pxFile->DataCursor = SetFilePointer(pxFile->ID, 0, 0, FILE_CURRENT); // Windows XP (+UWP), Kernel32.dll, fileapi.h
			const LARGE_INTEGER distanceToMove = { 0,0 }; // We do not move
			LARGE_INTEGER newFilePointer;

			const PXBool result = SetFilePointerEx(pxFile->ID, distanceToMove, &newFilePointer, FILE_CURRENT); // Windows XP (+UWP), Kernel32.dll, fileapi.h

			if (result)
			{
				pxFile->DataCursor = newFilePointer.QuadPart;
			}
#endif

		}
		default:
			return PXFileCursorPositionInvalid;
	}
}

void PXFileCursorMoveTo(PXFile* const pxFile, const PXSize position)
{
	const PXSize minimalInBoundsPosition = PXMathMinimumIU(pxFile->DataSize, position); // Prevent cursor from going out of bounce

	switch (pxFile->LocationMode)
	{
		case PXFileLocationModeInternal: // Memory is handled internally.
		case PXFileLocationModeExternal: // Memory is stored outside this object
		case PXFileLocationModeMappedVirtual: // Used 'VirtalAlloc()' / 'mmap()'
		case PXFileLocationModeMappedFromDisk: // Used 'FileView()' / 'fmap()'
		{
			pxFile->DataCursor = minimalInBoundsPosition;
			break;
		}

		case PXFileLocationModeDirectCached: // Read & Write operations are cached into a buffer first.
		case PXFileLocationModeDirectUncached: // Read & Write operations are directly put into
		{
#if OSUnix
			pxFile->DataCursor = ftell(pxFile->ID);
#elif OSWindows

#if OS32Bit
			LONG sizeLow = position;
			LONG sizeHigh = 0;
#else OS64Bit

			LONG sizeLow = position & 0x00000000FFFFFFFF;
			LONG sizeHigh = (position & 0xFFFFFFFF00000000) >> 32;
#endif

			pxFile->DataCursor = SetFilePointer(pxFile->ID, sizeLow, sizeHigh, FILE_BEGIN); // Windows XP, Kernel32.dll, fileapi.h

#endif
			break;
		}
	}
}

void PXFileCursorToBeginning(PXFile* const pxFile)
{
	FlushFileBuffers(pxFile->ID);

	PXFileCursorMoveTo(pxFile, 0);
}

PXSize PXFileCursorAdvance(PXFile* const pxFile, const PXSize steps)
{
	const PXSize before = pxFile->DataCursor;
	PXFileCursorMoveTo(pxFile, pxFile->DataCursor + steps);
	const PXSize delta = pxFile->DataCursor - before;

	return delta;
}

void PXFileCursorRewind(PXFile* const pxFile, const PXSize steps)
{
	if (pxFile->DataCursor <= steps)
	{
		PXFileCursorMoveTo(pxFile, 0);
		return;
	}

	PXFileCursorMoveTo(pxFile, pxFile->DataCursor - steps); // Check underflow
}

void PXFileCursorToEnd(PXFile* const pxFile)
{
	PXFileCursorMoveTo(pxFile, pxFile->DataCursor);
}

PXSize PXFileSkipEndOfLineCharacters(PXFile* const pxFile)
{
	const PXSize oldPosition = pxFile->DataCursor;

	while (!PXFileIsAtEnd(pxFile))
	{
		const char* data = (char*)PXFileCursorPosition(pxFile);
		const PXBool advance = IsEmptySpace(*data) || IsTab(*data) || IsEndOfLineCharacter(*data);

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
		const char* data = (char*)PXFileCursorPosition(pxFile);
		const PXBool isEmtpySpace = IsEmptySpace(*data) && !IsEndOfString(*data);

		if (!isEmtpySpace)
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
		const char* data = (char*)PXFileCursorPosition(pxFile);
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

	PXTextCopyA(dataPoint, length, (char*)exportBuffer, length);

	PXFileSkipEndOfLineCharacters(pxFile);

	return length;
}

PXSize PXFileSkipBlock(PXFile* const pxFile)
{
	const PXSize oldPosition = pxFile->DataCursor;

	while (!PXFileIsAtEnd(pxFile))
	{
		const char* data = (char*)PXFileCursorPosition(pxFile);
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
		const char* data = (char*)PXFileCursorPosition(pxFile);
		const PXBool advance = !(IsEndOfLineCharacter(*data) || IsEndOfString(*data));

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
	PXText pxText;
	pxText.SizeAllocated = PXFileRemainingSize(pxFile);
	pxText.SizeUsed = PXFileRemainingSize(pxFile);
	pxText.NumberOfCharacters = PXFileRemainingSize(pxFile);
	pxText.TextA = (char*)PXFileCursorPosition(pxFile);
	pxText.Format = TextFormatASCII;

	pxText.SizeUsed = PXTextToInt(&pxText, number);

	PXFileCursorAdvance(pxFile, pxText.SizeUsed);

	return pxText.SizeUsed;
}

PXSize PXFileReadIXXE(PXFile* const pxFile, void* const valueAdress, const PXSize valueSize, const PXEndian pxEndian)
{
	const PXSize writtenBytes = PXFileReadB(pxFile, valueAdress, valueSize);

	PXEndianSwap(valueAdress, valueSize, pxEndian, EndianCurrentSystem);

	return writtenBytes;
}

PXSize PXFileReadIXXVE(PXFile* const pxFile, void** const valueList, const PXSize valueListSize, const PXSize valueSizeSingle, const PXEndian pxEndian)
{	
	PXSize writtenBytes = 0;

	if (!valueList)
	{
		return 0;
	}

	for (PXSize i = 0; i < valueListSize; i++)
	{
		void* const adress = valueList[i];

		writtenBytes = PXFileReadB(pxFile, adress, valueSizeSingle);
	}

	PXEndianSwapV(valueList, valueListSize * valueSizeSingle, valueSizeSingle, pxEndian, EndianCurrentSystem);

	return writtenBytes;
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
	return PXFileReadIXXE(pxFile, value, sizeof(PXInt16S), pxEndian);
}

PXSize PXFileReadI16SVE(PXFile* const pxFile, PXInt16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXInt16S), pxEndian);
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
	return PXFileReadIXXE(pxFile, value, sizeof(PXInt16U), pxEndian);
}

PXSize PXFileReadI16UVE(PXFile* const pxFile, PXInt16U** const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXInt16U), pxEndian);
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
	return PXFileReadIXXE(pxFile, value, sizeof(PXInt32S), pxEndian);
}

PXSize PXFileReadI32SVE(PXFile* const pxFile, PXInt32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXInt32S), pxEndian);
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
	return PXFileReadIXXE(pxFile, value, sizeof(PXInt32U), pxEndian);
}

PXSize PXFileReadI32UVE(PXFile* const pxFile, PXInt32U** const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXInt32U), pxEndian);
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
	return PXFileReadIXXE(pxFile, value, sizeof(PXInt64S), pxEndian);
}

PXSize PXFileReadI64VE(PXFile* const pxFile, PXInt64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXInt64S), pxEndian);
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
	return PXFileReadIXXE(pxFile, value, sizeof(PXInt64U), pxEndian);
}

PXSize PXFileReadI64UVE(PXFile* const pxFile, PXInt64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
	return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXInt64U), pxEndian);
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

PXSize PXFileReadMultible(PXFile* const pxFile, const PXFileDataElementType* const pxFileElementList, const PXSize pxFileElementListSize)
{
	PXSize totalReadBytes = 0;

	for (PXSize i = 0; i < pxFileElementListSize; ++i)
	{
		const PXFileDataElementType* const pxFileDataElementType = &pxFileElementList[i];

		switch (pxFileDataElementType->Type)
		{
			case PXDataTypeInt8S:
				totalReadBytes += PXFileReadI8S(pxFile, (PXInt8S*)pxFileDataElementType->Adress);
				break;

			case PXDataTypeInt8U:
				totalReadBytes += PXFileReadI8U(pxFile, (PXInt8U*)pxFileDataElementType->Adress);
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
	switch (pxFile->LocationMode)
	{
		case PXFileLocationModeInternal:
		case PXFileLocationModeExternal:
		case PXFileLocationModeMappedVirtual:
		case PXFileLocationModeMappedFromDisk:
		{
			const void* currentPosition = PXFileCursorPosition(pxFile);
			const PXSize moveSize = PXFileCursorAdvance(pxFile, length);

			PXMemoryCopy(currentPosition, moveSize, value, moveSize);

			return moveSize;
		}
		case PXFileLocationModeDirectCached:
		case PXFileLocationModeDirectUncached:
		{
#if OSUnix
			const PXSize writtenBytes = fread(value, sizeof(PXByte), length, pxFile->ID);

			return writtenBytes;

#elif OSWindows
			DWORD writtenBytes = 0;

			if (pxFile->DataCursor >= pxFile->DataSize)
			{
				return 0;
			}

			const PXBool success = ReadFile(pxFile->ID, value, length, &writtenBytes, PXNull);

			if (!success)
			{
				PXActionResult result = PXErrorCurrent();

				return 0;
			}

			pxFile->DataCursor += writtenBytes;

			return writtenBytes;
#endif
		}
	}

	return 0;
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
	const char* currentPosition = (char*)PXFileCursorPosition(pxFile);

	PXSize lengthCopy = 0;

	while (!PXFileIsAtEnd(pxFile))
	{
		const char* data = (char*)PXFileCursorPosition(pxFile);
		const PXBool advance = *data != character && length <= lengthCopy;

		if (!advance)
		{
			break;
		}

		++lengthCopy;

		PXFileCursorAdvance(pxFile, 1u);
	}

	const PXSize readableSize = PXFileRemainingSize(pxFile);

	PXMemoryCopy(currentPosition, readableSize, value, lengthCopy);
}

PXBool PXFileReadAndCompareI64U(PXFile* const pxFile, const PXInt64U value)
{
	PXInt64U valueA = 0;
	const PXInt64U valueSize = sizeof(PXInt64U);
	const PXSize readBytes = PXFileReadB(pxFile, &valueA, valueSize);
	const PXBool successful = readBytes == valueSize;

	return successful;
}

PXBool PXFileReadAndCompare(PXFile* const pxFile, const void* value, const PXSize length)
{
	const void* currentPosition = PXFileCursorPosition(pxFile);
	const PXSize readableSize = PXFileRemainingSize(pxFile);

	const PXBool result = PXMemoryCompare(currentPosition, readableSize, value, length);

	if (result)
	{
		PXFileCursorAdvance(pxFile, length);
	}

	return result;
}

PXBool PXFileReadAndCompareV(PXFile* const pxFile, const void** const value, const PXSize* const valueElementSizeList, const PXSize valueLength)
{
	const void* const currentPosition = PXFileCursorPosition(pxFile);
	const PXSize readableSize = PXFileRemainingSize(pxFile);

	for (size_t i = 0; i < valueLength; ++i)
	{
		const void* text = value[i];
		const PXSize size = valueElementSizeList[i];
		const PXBool result = PXMemoryCompare(currentPosition, readableSize, text, size);

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

PXSize PXFileWriteI16S(PXFile* const pxFile, const PXInt16S value)
{
	return PXFileWriteB(pxFile, &value, sizeof(PXInt16S));
}

PXSize PXFileWriteI16SV(PXFile* const pxFile, const PXInt16S* const valueList, const PXSize valueListSize)
{
	return PXFileWriteB(pxFile, valueList, sizeof(PXInt16S) * valueListSize);
}

PXSize PXFileWriteI16SE(PXFile* const pxFile, const PXInt16S value, const PXEndian pxEndian)
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
		accumulator += PXFileWriteI16SE(pxFile, valueList[i], pxEndian);
	}

	return accumulator;
}

PXSize PXFileWriteI16U(PXFile* const pxFile, const PXInt16U value)
{
	return PXFileWriteB(pxFile, &value, sizeof(PXInt16U));
}

PXSize PXFileWriteAtI16U(PXFile* const pxFile, const PXInt16U value, const PXSize index)
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

PXSize PXFileWriteI16UE(PXFile* const pxFile, const PXInt16U value, const PXEndian pxEndian)
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
		accumulator += PXFileWriteI16UE(pxFile, valueList[i], pxEndian);
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
		accumulator += PXFileWriteI32SE(pxFile, valueList[i], pxEndian);
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
		accumulator += PXFileWriteI32UE(pxFile, valueList[i], pxEndian);
	}

	return accumulator;
}

PXSize PXFileWriteI64S(PXFile* const pxFile, const PXInt64S value)
{
	return PXFileWriteB(pxFile, &value, sizeof(PXInt64S));
}

PXSize PXFileWriteI64SV(PXFile* const pxFile, const PXInt64S* const valueList, const PXSize valueListSize)
{
	return PXFileWriteB(pxFile, valueList, sizeof(PXInt64S) * valueListSize);
}

PXSize PXFileWriteI64SE(PXFile* const pxFile, const PXInt64S value, const PXEndian pxEndian)
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
		accumulator += PXFileWriteI64SE(pxFile, valueList[i], pxEndian);
	}

	return accumulator;
}

PXSize PXFileWriteI64U(PXFile* const pxFile, const PXInt64U value)
{
	return PXFileWriteB(pxFile, &value, sizeof(PXInt64U));
}

PXSize PXFileWriteI64UV(PXFile* const pxFile, const PXInt64U* const valueList, const PXSize valueListSize)
{
	return PXFileWriteB(pxFile, valueList, sizeof(PXInt64U) * valueListSize);
}

PXSize PXFileWriteI64UE(PXFile* const pxFile, const PXInt64U value, const PXEndian pxEndian)
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
		accumulator += PXFileWriteI64UE(pxFile, valueList[i], pxEndian);
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
	switch (pxFile->LocationMode)
	{
		case PXFileLocationModeInternal:
		case PXFileLocationModeExternal:
		case PXFileLocationModeMappedVirtual:
		case PXFileLocationModeMappedFromDisk:
		{
			const PXSize writableSize = PXFileRemainingSize(pxFile);
			void* const currentPosition = PXFileCursorPosition(pxFile);
			const PXSize copyedBytes = PXMemoryCopy(value, length, currentPosition, writableSize);

			PXFileCursorAdvance(pxFile, copyedBytes);

			return copyedBytes;
		}

		case PXFileLocationModeDirectCached:
		case PXFileLocationModeDirectUncached:
		{
			DWORD writtenBytes = 0;

			const PXBool result = WriteFile(pxFile->ID, value, length, &writtenBytes, PXNull); // Windows XP (+UWP), Kernel32.dll, fileapi.h

			if (!result)
			{
				return 0;
			}

			pxFile->DataCursor += writtenBytes;
			pxFile->DataSize += writtenBytes;

			return writtenBytes;
		}

		default:
			return 0;
	}
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

	PXMemorySet(beforePosition, value, write);

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
	const void* currentPosition = PXFileCursorPosition(pxFile);

	va_list args;
	va_start(args, format);

	const PXSize writableSize = PXFileRemainingSize(pxFile);
	const int writtenBytes = PXPrintfvs(currentPosition, writableSize, format, args);

	va_end(args);

	{
		const PXBool sucessful = writtenBytes >= 0;

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
	unsigned char* a = (unsigned char*)PXFileCursorPosition(pxFile);
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

PXActionResult PXFilePathGet(const PXFile* const pxFile, PXText* const filePath)
{
#if 1

#if OSUnix
	return 0; // TODO

#elif OSWindows


#if WindowsAtleastVista

	const PXSize length = GetFinalPathNameByHandleA(pxFile->ID, filePath->TextA, filePath->SizeAllocated, VOLUME_NAME_DOS); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool successful = 0u != length;

	if (!successful)
	{
		return PXFalse;
	}

	filePath->SizeUsed = length - 4;
	filePath->TextA += 4;
	filePath->Format = TextFormatASCII;
	filePath->SizeUsed = length;

	PXTextReplace(filePath, '\\', '/');

	return PXActionSuccessful;
#else
	return PXActionRefusedNotSupported;
#endif


#endif

#else

#if OSUnix
	return 0; // TODO

#elif OSWindows
	const DWORD flags = VOLUME_NAME_DOS | FILE_NAME_NORMALIZED;
	const DWORD length = GetFinalPathNameByHandleW(pxFile->ID, filePath->TextW, filePath->SizeAllocated, flags); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool successful = 0u != length;

	if (!successful)
	{
		return PXFalse;
	}

	filePath->Format = TextFormatUNICODE;
	filePath->SizeUsed = length;

	wchar_t buffer[PathMaxSize];

	if (filePath->TextW[0] == '\\')
	{
		PXTextCopyW(filePath + 4, length - 4, buffer, PathMaxSize);
		PXTextCopyW(buffer, PathMaxSize, filePath, length - 4);
	}

	return PXActionSuccessful;
#endif

#endif // 0
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

#endif