#include "PXDirectory.h"

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Console/PXConsole.h>

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
#include <Shlobj.h>

PXFileElementInfoType PXAPI PXFileTypeGet(WIN32_FIND_DATA* windowsData)
{
    if(PXDirectoryIsRootFolder(windowsData->cFileName))
    {
        return PXFileElementInfoTypeDictionaryRoot;
    }
    else if(PXDirectoryIsCurrentFolder(windowsData->cFileName))
    {
        return PXFileElementInfoTypeDictionaryParent;
    }
    else
    {
        return 
            ((windowsData->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0) * PXFileElementInfoTypeFile +
            ((windowsData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) * PXFileElementInfoTypeDictionary;
    }
}

void PXAPI PXFileElementInfoCOnvertFrom(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntry, WIN32_FIND_DATA* const findData, PXInt8U depth)
{ 
    pxFileEntry->FilePathSize = PXTextLengthA(findData->cFileName, MAX_PATH);
    pxFileEntry->FilePathData = findData->cFileName;

    pxFileEntry->Type = PXFileTypeGet(findData);
    pxFileEntry->Size = (findData->nFileSizeHigh * (MAXDWORD + 1u)) + findData->nFileSizeLow;
    pxFileEntry->Depth = depth; //  pxDirectorySearchInfo->DepthCounter;

    /*

    // Make full path
    {
        pxFileElementInfo->FullPathOffset += PXTextCopyA(findData->cFileName, PXTextUnkownLength, pxFileElementInfo->FullPath + pxFileElementInfo->FullPathOffset, pxFileElementInfo->FullPathSize - pxFileElementInfo->FullPathOffset);
    }

    if(pxFileElementInfo->Type == PXFileElementInfoTypeDictionary)
    {
        pxFileElementInfo->FullPathOffset += PXTextCopyA("/", 1, pxFileElementInfo->FullPath + pxFileElementInfo->FullPathOffset, pxFileElementInfo->FullPathSize - pxFileElementInfo->FullPathOffset);
    }

    // Replace all \ with /
    PXTextReplaceA(pxFileElementInfo->FullPath, pxFileElementInfo->FullPathOffset, '\\', '/');
    */
}

#endif

void PXAPI PXDirectoryEntryStore(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntryINPUT)
{
    // Get new entry to write in
    pxDirectorySearchCache->EntryList = PXMemoryReallocT(PXFileEntry, pxDirectorySearchCache->EntryList, pxDirectorySearchCache->EntryAmount+1);

    PXFileEntry* pxFileEntryINSERT = &pxDirectorySearchCache->EntryList[pxDirectorySearchCache->EntryAmount];

    ++pxDirectorySearchCache->EntryAmount;

    PXCopy(PXFileEntry, pxFileEntryINPUT, pxFileEntryINSERT);
    
    pxFileEntryINPUT->ID = pxDirectorySearchCache->EntryAmount;
    pxFileEntryINSERT->FilePathData = PXFlexDataCacheAdd(&pxDirectorySearchCache->FilePathCache, &pxFileEntryINPUT->ID, pxFileEntryINPUT->FilePathData, pxFileEntryINPUT->FilePathSize);


    PXConsoleWriteF(0, "%s\n", pxFileEntryINPUT->FilePathData);
    


    /*

case PXFileElementInfoTypeFile:
{
#if 0
            const PXBool match = PXTextMatchW(pxDirectoryIterator->.Name, 230, extendedSearchFilter, extendedWriien);


            if (pxDirectorySearchInfo->Flags & 0x01 && match)
            {
                wchar_t seachDirectoryKey[PXPathSizeMax];
                wchar_t* address = seachDirectoryKey;

                address += PXTextCopyW(buffer, wriien, address, PXPathSizeMax);
                address += PXTextCopyW(findFileData.cFileName, 512, address, 512);

                fileElementInfoCurrent.FullPath = seachDirectoryKey;

                pxDirectorySearchInfo->Callback(&fileElementInfoCurrent);
            }
#endif

            break;
        }
        case PXFileElementInfoTypeDictionary:
        {
#if 0
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
#endif

            break;
        }
        */
}

PXActionResult PXAPI PXDirectorySearch(PXDirectorySearchCache* const pxDirectorySearchCache, const PXText* const directoryName)
{
    PXClear(PXDirectorySearchCache, pxDirectorySearchCache);

    PXFlexDataCacheInit(&pxDirectorySearchCache->FilePathCache, sizeof(PXInt32U));

    const PXActionResult open = PXDirectoryOpen(pxDirectorySearchCache, directoryName);

    if(PXActionSuccessful != open)
    {
        return open;
    }

    PXFileEntry pxFileEntry;

    while(PXDirectoryNext(pxDirectorySearchCache, &pxFileEntry));

    const PXBool close = PXDirectoryClose(pxDirectorySearchCache);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectoryOpen(PXDirectorySearchCache* const pxDirectorySearchCache, const PXText* const directoryName)
{
   // PXClear(PXDirectorySearchCache, pxDirectorySearchCache);

#if OSUnix

    pxDirectoryIterator->ID = opendir(directoryName->TextA); // dirent.h

    const PXBool success = pxDirectoryIterator->ID != PXNull;

    PXActionOnErrorFetchAndReturn(!success);

#elif OSWindows

    char seachDirectoryKey[PXPathSizeMax];
    char* address = seachDirectoryKey;

    address += PXTextCopyA(directoryName->TextA, directoryName->SizeUsed, address, PXPathSizeMax);
    address += PXTextCopyA("*", 1u, address, PXPathSizeMax); // Get all directory and files

    WIN32_FIND_DATAA windowsDirectoryData;

    pxDirectorySearchCache->DirectoryHandleCurrent = FindFirstFileA(seachDirectoryKey, &windowsDirectoryData); // FindFirstFileExW() has literally no additional functionality (for now)

    {
        const PXBool failed = INVALID_HANDLE_VALUE == pxDirectorySearchCache->DirectoryHandleCurrent;

        PXActionOnErrorFetchAndReturn(failed);
    }


    PXFileEntry pxFileEntry;

    PXFileElementInfoCOnvertFrom(pxDirectorySearchCache, &pxFileEntry, &windowsDirectoryData, 1);

    PXBool isDotFolder =
        pxFileEntry.Type == PXFileElementInfoTypeDictionaryRoot ||
        pxFileEntry.Type == PXFileElementInfoTypeDictionaryParent;

    if(isDotFolder)
    {
        do
        {
            PXBool succes = PXDirectoryNext(pxDirectorySearchCache, &pxFileEntry);

            isDotFolder =
                pxFileEntry.Type == PXFileElementInfoTypeDictionaryRoot ||
                pxFileEntry.Type == PXFileElementInfoTypeDictionaryParent;
        } 
        while(isDotFolder);
    }

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif
}

PXBool PXAPI PXDirectoryNext(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* pxFileEntry)
{
#if OSUnix
    struct dirent* directoryEntry;

    while ((directoryEntry = readdir(pxDirectoryIterator->ID)) != PXNull)
    {
        const PXBool isSystemDottedFolder = PXDirectoryIsDotFolder(directoryEntry->d_name);
        //const PXInt32U directoryLength = PXTextLength(directory);
        //const PXInt32U FileNameLength = strlen(directoryEntry->d_name);
    }
#elif OSWindows

    for(;;)
    {
        WIN32_FIND_DATAA seachResult;
        const PXBool fetchSuccessful = FindNextFileA(pxDirectorySearchCache->DirectoryHandleCurrent, &seachResult);

        if(!fetchSuccessful)
        {
            return PXFalse;
        }

        PXFileElementInfoCOnvertFrom(pxDirectorySearchCache, pxFileEntry, &seachResult, 0);

        const PXBool isDotFodler =
            PXFileElementInfoTypeDictionaryRoot == pxFileEntry->Type ||
            PXFileElementInfoTypeDictionaryParent == pxFileEntry->Type;

        if(isDotFodler)
        {
            continue;
        }

        PXDirectoryEntryStore(pxDirectorySearchCache, pxFileEntry);
        
        break;
    }

    return PXTrue;

#endif
}

PXBool PXAPI PXDirectoryClose(PXDirectorySearchCache* const pxDirectorySearchCache)
{
#if OSUnix
    const int returnCode = closedir(pxDirectoryIterator->ID);
    const PXBool success = -1 != returnCode;

    PXActionOnErrorFetchAndReturn(!success);

#elif OSWindows
    const PXBool success = FindClose(pxDirectorySearchCache->DirectoryHandleCurrent);

    PXActionOnErrorFetchAndReturn(!success);
#endif
}

PXActionResult PXAPI PXDirectoryCreate(const PXText* const directoryName)
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

            PXActionOnErrorFetchAndReturn(!successCreate);

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
            PXActionOnErrorFetchAndReturn(!successCreate);

            break;
        }
    }

    /*
    wchar_t directoryNameSegment[PXPathSizeMax];
    PXSize starPos = 0;
    PXSize successful = 0;

    do
    {
        PXSize offset = PXTextFindFirstW(directoryName + starPos, PXPathSizeMax - starPos, '/');

        if (offset == -1)
        {
            break;
        }

        PXTextCopyW(directoryName + starPos, offset - 1, directoryNameSegment, PXPathSizeMax);

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

PXActionResult PXAPI PXWorkingDirectoryChange(const PXText* const directoryName)
{
    switch (directoryName->Format)
    {
    case TextFormatASCII:
    case TextFormatUTF8:
    {
#if OSUnix
        const int resultID = chdir(directoryName->TextA);
#elif OSWindows
        const PXBool resultID = SetCurrentDirectoryA(directoryName->TextA); // _chdir()
#endif

        PXActionOnErrorFetchAndReturn(!resultID);

        return PXActionSuccessful;
    }
    case TextFormatUNICODE:
    {
#if OSUnix
        const int resultID = 0; // TODO: Add conversion?
#elif OSWindows
        const PXBool resultID = SetCurrentDirectoryW(directoryName->TextW); // _wchdir()
#endif

        PXActionOnErrorFetchAndReturn(!resultID);

        return PXActionSuccessful;
    }
    }

    return PXActionInvalidStateImpossible;
}

PXActionResult PXAPI PXWorkingDirectoryGet(PXText* const workingDirectory)
{
    switch (workingDirectory->Format)
    {
    case TextFormatASCII:
    case TextFormatUTF8:
    {
#if OSUnix
        const char* const text = getcwd(workingDirectory->TextA, workingDirectory->SizeAllocated);
#elif OSWindows
        workingDirectory->SizeUsed = GetCurrentDirectoryA(workingDirectory->SizeAllocated, workingDirectory->TextA); // _getcwd()
#endif
        const PXBool successful = workingDirectory->SizeUsed > 0;

        PXActionOnErrorFetchAndReturn(!successful);

        return PXActionSuccessful;
    }
    case TextFormatUNICODE:
    {
#if OSUnix
        const char* const text = 0; // TODO: Add conversion?
#elif OSWindows
        workingDirectory->SizeUsed = GetCurrentDirectoryW(workingDirectory->SizeAllocated, workingDirectory->TextW); // _wgetcwd()
#endif
        const PXBool successful = workingDirectory->SizeUsed > 0;

        PXActionOnErrorFetchAndReturn(!successful);

        return PXActionSuccessful;
    }
    }

    return PXActionInvalidStateImpossible;
}

PXActionResult PXAPI PXDirectoryDelete(const PXText* const directoryName)
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
        PXActionOnErrorFetchAndReturn(!successul);


        return PXActionSuccessful;
    }
    case TextFormatUNICODE:
    {
#if OSUnix
        const PXBool successul = 1; // TODO: Add conversion?
#elif OSWindows
        const PXBool successul = RemoveDirectoryW(directoryName->TextW); // Windows XP, Kernel32.dll, fileapi.h
#endif

        PXActionOnErrorFetchAndReturn(!successul);

        return PXActionSuccessful;
    }
    }

    return PXActionInvalidStateImpossible;
}

PXActionResult PXAPI PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize)
{
    return PXActionInvalid;
}

PXActionResult PXAPI PXDirectoryFilesInFolderW()
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

PXActionResult PXAPI PXDirectorySpecialFolderGet(const PXDirectioySpecialFolder pxDirectioySpecialFolder, PXText* const pxTextSpecialFolder, PXText* const pxTextFileName, const PXBool create)
{
#if OSUnix
#elif OSWindows
    int folderID;

    //CSIDL_FLAG_DONT_VERIFY

    switch (pxDirectioySpecialFolder)
    {
        case PXDirectioySpecialFolder_ADMINTOOLS: folderID = CSIDL_ADMINTOOLS;
        case PXDirectioySpecialFolderAPPDATA: folderID = CSIDL_APPDATA ;
        case PXDirectioySpecialFolderCOMMON_ADMINTOOLS: folderID = CSIDL_COMMON_ADMINTOOLS;
        case PXDirectioySpecialFolderCOMMON_APPDATA: folderID = CSIDL_COMMON_APPDATA;
        case PXDirectioySpecialFolderCOMMON_DOCUMENTS: folderID = CSIDL_COMMON_DOCUMENTS;
        case PXDirectioySpecialFolderCOOKIES: folderID = CSIDL_COOKIES;
        case PXDirectioySpecialFolderHISTORY: folderID = CSIDL_HISTORY;
        case PXDirectioySpecialFolderINTERNET_CACHE: folderID = CSIDL_INTERNET_CACHE;
        case PXDirectioySpecialFolderLOCAL_APPDATA: folderID = CSIDL_LOCAL_APPDATA;
        case PXDirectioySpecialFolderPictures: folderID = CSIDL_MYPICTURES;
        case PXDirectioySpecialFolderPERSONAL: folderID = CSIDL_PERSONAL;
        case PXDirectioySpecialFolderPROGRAM_FILES: folderID = CSIDL_PROGRAM_FILES;
        case PXDirectioySpecialFolderPROGRAM_FILES_COMMON: folderID = CSIDL_PROGRAM_FILES_COMMON;
        case PXDirectioySpecialFolderSystem: folderID = CSIDL_SYSTEM;
        case PXDirectioySpecialFolderWinfows: folderID = CSIDL_WINDOWS;

        default:
            folderID = 0;
            break;
    }

    if (create)
    {
        folderID |= CSIDL_FLAG_CREATE;
    }

    const HRESULT resultErrorCodeID = SHGetFolderPathA(PXNull, folderID, 0, 0, pxTextSpecialFolder->TextA); // Windows 2000, Shell32.lib, Shlobj.h (SHGetSpecialFolderPathA might not be supported?)

    if (resultErrorCodeID != S_OK)
    {
        return PXActionInvalid;
    }

    pxTextSpecialFolder->SizeUsed = PXTextLengthA(pxTextSpecialFolder->TextA, PXPathSizeMax);

    if (!pxTextFileName)
    {
        return PXActionSuccessful;
    }

    const PXSize fullSize = PXTextAppend(pxTextSpecialFolder, pxTextFileName);

    if (fullSize == 0)
    {
        return PXActionInvalid;
    }

    return PXActionSuccessful;
#elif
    return PXActionRefusedNotSupported;
#endif
}