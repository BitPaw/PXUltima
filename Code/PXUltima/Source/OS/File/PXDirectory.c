#include "PXDirectory.h"

#if PXDirectoryUSE

#include <OS/File/PXFile.h>
#include <Media/PXText.h>

#if OSUnix
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//#include <sys/io.h>
#include <dirent.h>

#define OSWorkingDirectoryChangeA chdir
#define OSWorkingDirectoryChangeW(string) OSWorkingDirectoryChangeA((const char*)string)

#elif OSWindows
#include <io.h>

#define OSWorkingDirectoryChangeA _chdir
#define OSWorkingDirectoryChangeW _wchdir

#endif

void PXDirectoryUpdateEntry(PXDirectoryIterator* const pxDirectoryIterator)
{

}

PXActionResult PXDirectoryOpenA(PXDirectoryIterator* const pxDirectoryIterator, const PXText* const directoryName)
{
	return PXActionInvalid;
}

PXBool PXDirectoryNext(PXDirectoryIterator* const pxDirectoryIterator)
{
	return PXActionInvalid;
}

PXBool PXDirectoryClose(PXDirectoryIterator* const pxDirectoryIterator)
{
	return PXActionInvalid;
}

PXActionResult PXDirectoryCreate(const PXText* const directoryName)
{
	switch (directoryName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
#if OSUnix
			const int creationResult = mkdir(directoryName->TextA, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			const PXBool successCreate = creationResult == 0;
#elif OSWindows

#if OSForcePOSIXForWindows
			const int resultID = _mkdir(directoryName->TextA);
			const PXBool successCreate = 0 == resultID;
#else
			const PXBool successCreate = CreateDirectoryA(directoryName->TextA, PXNull); // Windows XP, Kernel32.dll, fileapi.h
#endif

#endif

			PXActionOnErrorFetchAndExit(!successCreate);

			break;
		}

			// 2 Bytes per character, range from 0 to 65535
		case TextFormatUNICODE:
		{
#if OSUnix
			// CONVERT

			const PXBool successCreate = PXFalse;

#elif OSWindows

#if OSForcePOSIXForWindows
			const int resultID = _wmkdir(directoryName->TextW);
			const PXBool successCreate = 0 == resultID;
#else
			const PXBool successCreate = CreateDirectoryW(directoryName->TextW, PXNull); // Windows XP, Kernel32.dll, fileapi.h
#endif

#endif
			PXActionOnErrorFetchAndExit(!successCreate);

			break;
		}
	}

	/*
	wchar_t directoryNameSegment[PathMaxSize];
	PXSize starPos = 0;
	PXSize successful = 0;

	do
	{
		PXSize offset = PXTextFindFirstW(directoryName + starPos, PathMaxSize - starPos, '/');

		if (offset == -1)
		{
			break;
		}

		PXTextCopyW(directoryName + starPos, offset - 1, directoryNameSegment, PathMaxSize);

		const int creationResult = OSFileDirectoryCreateW(directoryNameSegment);
		const PXBool wasSuccesful = creationResult == 0;

		PXActionOnErrorFetchAndExit(!wasSuccesful);

		starPos += offset;
		++successful;
	} while (1);

	return PXActionSuccessful;
	*/

	return PXActionSuccessful;
}

PXActionResult PXWorkingDirectoryChange(const PXText* const directoryName)
{
	switch (directoryName->Format)
	{
	case TextFormatASCII:
	case TextFormatUTF8:
	{
#if OSUnix
		const char* const text = chdir(directoryName->TextA);
#elif OSWindows
		const char* const text = _chdir(directoryName->TextA);
#endif
		const PXBool successful = text != 0;

		PXActionOnErrorFetchAndExit(!successful);


		return PXActionSuccessful;
	}
	case TextFormatUNICODE:
	{
#if OSUnix
		const char* const text = 0; // TODO: Add conversion?
#elif OSWindows
		const char* const text = _wchdir(directoryName->TextW);
#endif
		const PXBool successful = text != 0;

		PXActionOnErrorFetchAndExit(!successful);

		return PXActionSuccessful;
	}
	}

	return PXActionInvalidStateImpossible;
}

PXActionResult PXWorkingDirectoryGet(PXText* const workingDirectory)
{
	switch (workingDirectory->Format)
	{
	case TextFormatASCII:
	case TextFormatUTF8:
	{
#if OSUnix
		const char* const text = getcwd(workingDirectory->TextA, workingDirectory->SizeAllocated);
#elif OSWindows
		const char* const text = _getcwd(workingDirectory->TextA, workingDirectory->SizeAllocated);
#endif
		const PXBool successful = text != 0;

		PXActionOnErrorFetchAndExit(!successful);


		return PXActionSuccessful;
	}
	case TextFormatUNICODE:
	{
#if OSUnix
		const char* const text = 0; // TODO: Add conversion?
#elif OSWindows
		const char* const text = _wgetcwd(workingDirectory->TextW, workingDirectory->SizeAllocated);
#endif
		const PXBool successful = text != 0;

		PXActionOnErrorFetchAndExit(!successful);

		return PXActionSuccessful;
	}
	}

	return PXActionInvalidStateImpossible;
}

PXActionResult PXDirectoryDelete(const PXText* const directoryName)
{
	switch (directoryName->Format)
	{
	case TextFormatASCII:
	case TextFormatUTF8:
	{
#if OSUnix || OSForcePOSIXForWindows
		const int resultID = rmdir(directoryName->TextA);
		const PXBool successul = 0 == resultID;
#elif OSWindows
		const PXBool successul = RemoveDirectoryA(directoryName->TextA); // Windows XP, Kernel32.dll, fileapi.h
#endif
		PXActionOnErrorFetchAndExit(!successul);


		return PXActionSuccessful;
	}
	case TextFormatUNICODE:
	{
#if OSUnix
		const PXBool successul = 1; // TODO: Add conversion?
#elif OSWindows
		const PXBool successul = RemoveDirectoryW(directoryName->TextW); // Windows XP, Kernel32.dll, fileapi.h
#endif

		PXActionOnErrorFetchAndExit(!successul);

		return PXActionSuccessful;
	}
	}

	return PXActionInvalidStateImpossible;
}

PXActionResult PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize)
{
	return PXActionInvalid;
}

PXActionResult PXDirectoryFilesInFolderW(const PXDirectorySearchInfo* const pxDirectorySearchInfo)
{
    #if 0
	wchar_t buffer[300];
	wchar_t extendedSearchFilter[50];
	PXSize wriien = PXFileDirectoryPathExtract(pxDirectorySearchInfo->FolderPath, 512, buffer, 512);
	PXSize extendedWriien = PXTextCopyW(pxDirectorySearchInfo->FolderPath + wriien, 512, extendedSearchFilter, 50);

	PXSize dictionaryCounter = 0;

#if 0//OSUnix

	struct dirent* directoryEntry;
	auto directory = opendir(directory);
	PXBool sucess = directory != PXNull;

	if (!sucess)
	{
		return 0;
	}

	while ((directoryEntry = readdir(directory)) != PXNull)
	{
		const PXBool isSystemDottedFolder = PXDirectoryIsDotFolder(directoryEntry->);
		const unsigned int directoryLength = strlen(directory);
		const unsigned int FileNameLength = strlen(directoryEntry->d_name);
	}

	closedir(directory);


#elif OSWindows

	WIN32_FIND_DATA findFileData;

	wchar_t seachDirectoryKey[300];
	wchar_t* address = seachDirectoryKey;

	address += PXTextCopyW(buffer, wriien, address, 300);
	address += PXTextCopyW(L"*", 1u, address, 300); // Get all directory and files

	const HANDLE iteratorHandle = FindFirstFileW(seachDirectoryKey, &findFileData); // FindFirstFileExW() has literally no additional functionality (for now)

	{
		const PXBool failed = INVALID_HANDLE_VALUE == iteratorHandle;

		PXActionOnErrorFetchAndExit(failed);
	}

	do
	{
		PXFileElementInfo fileElementInfoCurrent;
		fileElementInfoCurrent.Type =
			((findFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0) * PXFileElementInfoTypeFile +
			((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) * PXFileElementInfoTypeDictionary;


		fileElementInfoCurrent.Name = findFileData.cFileName;
		fileElementInfoCurrent.Size = (findFileData.nFileSizeHigh * (MAXDWORD + 1u)) + findFileData.nFileSizeLow;
		fileElementInfoCurrent.Depth = pxDirectorySearchInfo->DepthCounter;


		// Skip?
		const PXBool isSystemDottedFolder = PXDirectoryIsDotFolder(findFileData.cFileName);

		if (isSystemDottedFolder)
		{
			continue; // Root folder, as we go top->down, this is not needed
		}

		switch (fileElementInfoCurrent.Type)
		{
			case PXFileElementInfoTypeFile:
			{
				const PXBool match = PXTextMatchW(fileElementInfoCurrent.Name, 230, extendedSearchFilter, extendedWriien);

				if (pxDirectorySearchInfo->Flags & 0x01 && match)
				{
					wchar_t seachDirectoryKey[300];
					wchar_t* address = seachDirectoryKey;

					address += PXTextCopyW(buffer, wriien, address, 300);
					address += PXTextCopyW(findFileData.cFileName, 512, address, 512);

					fileElementInfoCurrent.FullPath = seachDirectoryKey;

					pxDirectorySearchInfo->Callback(&fileElementInfoCurrent);
				}

				break;
			}
			case PXFileElementInfoTypeDictionary:
			{
				wchar_t seachDirectoryKey[300];
				wchar_t* address = seachDirectoryKey;

				address += PXTextCopyW(buffer, wriien, address, 300);
				address += PXTextCopyW(findFileData.cFileName, 512, address, 512);
				address += PXTextCopyW(L"/", 1, address, 512);
				address += PXTextCopyW(extendedSearchFilter, extendedWriien, address, 512);

				if (pxDirectorySearchInfo->Flags & 0x02)
				{
					pxDirectorySearchInfo->Callback(&fileElementInfoCurrent);
				}

				if (pxDirectorySearchInfo->Recursion)
				{
					PXDirectorySearchInfo recusiveCallInfo;
					recusiveCallInfo.Callback = pxDirectorySearchInfo->Callback;
					recusiveCallInfo.FolderPath = seachDirectoryKey;
					recusiveCallInfo.FolderPathSize = 0;
					recusiveCallInfo.FileFilter = pxDirectorySearchInfo->FileFilter;
					recusiveCallInfo.FileFilterSize = pxDirectorySearchInfo->FileFilterSize;
					recusiveCallInfo.Flags = pxDirectorySearchInfo->Flags;
					recusiveCallInfo.DepthCounter = fileElementInfoCurrent.Depth + 1;
					recusiveCallInfo.Recursion = PXYes;

					PXDirectoryFilesInFolderW(&recusiveCallInfo);
				}

				break;
			}
		}
	} while (FindNextFile(iteratorHandle, &findFileData) != 0);

	FindClose(iteratorHandle);
#endif
#endif // 0

	return PXActionSuccessful;
}
#endif