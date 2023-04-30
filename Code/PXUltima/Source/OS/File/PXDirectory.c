#include "PXDirectory.h"

#include <OS/File/PXFile.h>
#include <Media/PXText.h>

#if OSUnix
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//#include <sys/io.h>
#include <dirent.h>

#define OSFileDirectoryCreateA(string) mkdir(string, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define OSFileDirectoryCreateW(string) OSFileDirectoryCreateA((const char*)string)
#define OSWorkingDirectoryCurrentA getcwd
#define OSWorkingDirectoryCurrentW(string, size) (wchar_t*)OSWorkingDirectoryCurrentA((char*)string, size)
#define OSWorkingDirectoryChangeA chdir
#define OSWorkingDirectoryChangeW(string) OSWorkingDirectoryChangeA((const char*)string)

#elif OSWindows
#include <io.h>

#define OSFileDirectoryCreateA _mkdir
#define OSFileDirectoryCreateW _wmkdir
#define OSWorkingDirectoryCurrentA _getcwd
#define OSWorkingDirectoryCurrentW _wgetcwd
#define OSWorkingDirectoryChangeA _chdir
#define OSWorkingDirectoryChangeW _wchdir

#endif

void PXDirectoryUpdateEntry(PXDirectoryIterator* const pxDirectoryIterator)
{

}

PXActionResult PXDirectoryOpenA(PXDirectoryIterator* const pxDirectoryIterator, const PXTextASCII directoryName)
{
	return PXActionInvalid;
}

PXActionResult PXDirectoryOpenW(PXDirectoryIterator* const pxDirectoryIterator, const PXTextUNICODE directoryName)
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

PXActionResult PXDirectoryCreateA(const char* directoryName)
{
	const int creationResult = OSFileDirectoryCreateA(directoryName);
	const PXBool wasSuccesful = creationResult == 0;

	PXActionOnErrorFetchAndExit(!wasSuccesful);

	return PXActionSuccessful;
}

PXActionResult PXDirectoryCreateW(const wchar_t* directoryName)
{
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
}

PXActionResult PXWorkingDirectoryChange(const char* directoryName)
{
	return PXActionInvalid;
}

PXActionResult PXWorkingDirectoryGetA(char* workingDirectory, PXSize workingDirectorySize)
{
	char* workingDirectoryResult = OSWorkingDirectoryCurrentA(workingDirectory, workingDirectorySize);
	const PXBool wasSuccesful = workingDirectoryResult;

	PXActionOnErrorFetchAndExit(!wasSuccesful);

	return PXActionSuccessful;
}

PXActionResult PXWorkingDirectoryGetW(wchar_t* workingDirectory, PXSize workingDirectorySize)
{
	wchar_t* workingDirectoryResult = OSWorkingDirectoryCurrentW(workingDirectory, workingDirectorySize);
	const PXBool wasSuccesful = workingDirectoryResult;

	PXActionOnErrorFetchAndExit(!wasSuccesful);

	return PXActionSuccessful;
}

PXActionResult PXWorkingDirectoryChangeW(const wchar_t* directoryName)
{
	return PXActionInvalid;
}

PXActionResult PXDirectoryDeleteA(const char* directoryName)
{
	return PXActionInvalid;
}

PXActionResult PXDirectoryDeleteW(const wchar_t* directoryName)
{
	return PXActionInvalid;
}

PXActionResult PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize)
{
	return PXActionInvalid;
}

PXActionResult PXDirectoryFilesInFolderW(const PXDirectorySearchInfo* const pxDirectorySearchInfo)
{
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

	return PXActionSuccessful;
}