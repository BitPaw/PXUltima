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
#include <shellapi.h>

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
    pxFileEntry->FilePathSize = PXTextCopyA(findData->cFileName, MAX_PATH, pxFileEntry->FilePathData, MAX_PATH);

    pxFileEntry->Type = PXFileTypeGet(findData);
    pxFileEntry->Size = (findData->nFileSizeHigh * (MAXDWORD + 1u)) + findData->nFileSizeLow;
    pxFileEntry->Depth = depth; //  pxDirectorySearchInfo->DepthCounter;

    // TODO: Time??


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
    PXTextReplaceByte(pxFileElementInfo->FullPath, pxFileElementInfo->FullPathOffset, '\\', '/');
    */
}

#endif

void PXAPI PXDirectoryEntryStore(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntryINPUT)
{
    pxFileEntryINPUT->ID = pxDirectorySearchCache->EntryList.AmountUsed + 100;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "FileDir",
        "Search-Register",
        "ID:%3i - %s",
        pxFileEntryINPUT->ID,
        pxFileEntryINPUT->FilePathData
    );
#endif

    // Hijack adress, create
    //pxFileEntryINPUT->FilePathData = PXListDynamicAdd(&pxDirectorySearchCache->FilePathCache, &pxFileEntryINPUT->ID, pxFileEntryINPUT->FilePathData, pxFileEntryINPUT->FilePathSize);

    PXListAdd(&pxDirectorySearchCache->EntryList, pxFileEntryINPUT);








    return;








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

    PXListDynamicInit(&pxDirectorySearchCache->FilePathCache, sizeof(PXInt32U), PXListDynamicSizeObject1Byte);
    PXListInitialize(&pxDirectorySearchCache->EntryList, sizeof(PXFileEntry), 25);

    PXFileEntry pxFileEntry;

    const PXActionResult open = PXDirectoryOpen(pxDirectorySearchCache, &pxFileEntry, directoryName);

    if(PXActionSuccessful != open)
    {
        return open;
    }

    while(PXDirectoryNext(pxDirectorySearchCache, &pxFileEntry));

    const PXBool close = PXDirectoryClose(pxDirectorySearchCache);


    // Fix stale references because an reallocation could have moved the data
    for(size_t i = 0; i < pxDirectorySearchCache->EntryList.AmountUsed; ++i)
    {
        PXFileEntry* const pxFileEntry = PXListEntyrGetT(PXFileEntry, &pxDirectorySearchCache->EntryList, i);

        PXInt32U key = i+100;

        PXListDynamicGet(&pxDirectorySearchCache->FilePathCache, &key, &pxFileEntry->FilePathData, &pxFileEntry->FilePathSize);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "FileDir",
            "Search",
            "ID:%3i - %s",
            key,
            pxFileEntry->FilePathData
        );
#endif
    }



    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectoryOpenA(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntry, const char* const directoryName)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, directoryName, PXTextLengthUnkown, PXTextLengthUnkown);

    return PXDirectoryOpen(pxDirectorySearchCache, pxFileEntry, &pxText);
}

PXActionResult PXAPI PXDirectoryOpen(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntry, const PXText* const directoryName)
{
    // PXClear(PXDirectorySearchCache, pxDirectorySearchCache);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "FileDir",
        "Open",
        "%s",
        directoryName->TextA
    );
#endif

#if OSUnix

    pxDirectorySearchCache->DirectoryHandleCurrent = opendir(directoryName->TextA); // dirent.h
    PXActionResult openResult = PXErrorCurrent(PXNull != pxDirectorySearchCache->DirectoryHandleCurrent);

    return openResult;

#elif OSWindows

    char seachDirectoryKey[PXPathSizeMax];
    char* address = seachDirectoryKey;

    PXBool terminatedBySlash = directoryName->TextA[directoryName->SizeUsed - 1] == '/';


    address += PXTextCopyA(directoryName->TextA, directoryName->SizeUsed, address, PXPathSizeMax);

    if(!terminatedBySlash)
    {
        address += PXTextCopyA("/", 1u, address, PXPathSizeMax); // Get all directory and files
    }

    if(PXTextFindFirstCharacterA(directoryName->TextA, directoryName->SizeUsed, '*') == (PXSize)-1)
    {
        address += PXTextCopyA("*", 1u, address, PXPathSizeMax); // Get all directory and files
    }

    WIN32_FIND_DATAA windowsDirectoryData;

    // First entry
    {
        pxDirectorySearchCache->DirectoryHandleCurrent = FindFirstFileA(seachDirectoryKey, &windowsDirectoryData); // FindFirstFileExW() has literally no additional functionality (for now)
        const PXActionResult pxActionResult = PXErrorCurrent(INVALID_HANDLE_VALUE != pxDirectorySearchCache->DirectoryHandleCurrent);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }
    }

    PXFileElementInfoCOnvertFrom(pxDirectorySearchCache, pxFileEntry, &windowsDirectoryData, 1);

    PXBool isDotFolder =
        pxFileEntry->Type == PXFileElementInfoTypeDictionaryRoot ||
        pxFileEntry->Type == PXFileElementInfoTypeDictionaryParent;

    if(isDotFolder)
    {
        do
        {
            PXBool succes = PXDirectoryNext(pxDirectorySearchCache, pxFileEntry);

            isDotFolder =
                pxFileEntry->Type == PXFileElementInfoTypeDictionaryRoot ||
                pxFileEntry->Type == PXFileElementInfoTypeDictionaryParent;
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

    while ((directoryEntry = readdir(pxDirectorySearchCache->DirectoryHandleCurrent)) != PXNull)
    {
        //const PXBool isSystemDottedFolder = PXDirectoryIsDotFolder(directoryEntry->d);
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

        //PXDirectoryEntryStore(pxDirectorySearchCache, pxFileEntry);

        break;
    }

    return PXTrue;

#endif
}

PXBool PXAPI PXDirectoryClose(PXDirectorySearchCache* const pxDirectorySearchCache)
{
#if OSUnix
    const int returnCode = closedir(pxDirectorySearchCache->DirectoryHandleCurrent);
    const PXActionResult pxActionResult = PXErrorCurrent(0 != returnCode);

    return pxActionResult;

#elif OSWindows
    const PXBool success = FindClose(pxDirectorySearchCache->DirectoryHandleCurrent);
    const PXActionResult pxActionResult = PXErrorCurrent(success);

    return pxActionResult;
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

        const PXActionResult pxActionResult = PXErrorCurrent(successCreate);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }

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
        const PXActionResult pxActionResult = PXErrorCurrent(successCreate);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }

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

PXActionResult PXAPI PXDirectoryCurrentA(char* const directoryCurrent, const PXSize bufferSize, PXSize* const sizeWritten)
{
#if OSUnix
    const char* const text = getcwd(workingDirectory->TextA, bufferSize);
    const PXActionResult pxActionResult = PXErrorCurrent(text);

    return pxActionResult;

#elif OSWindows
    const DWORD writtenBytes = GetCurrentDirectoryA(bufferSize, directoryCurrent); // _getcwd()
    const PXActionResult pxActionResult = PXErrorCurrent(writtenBytes > 0);

    if(sizeWritten)
    {
        *sizeWritten = writtenBytes;
    }

    return pxActionResult;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXDirectoryCurrentW(wchar_t* const directoryCurrent, const PXSize bufferSize, PXSize* const sizeWritten)
{
#if OSUnix
    return PXActionRefusedNotSupportedByLibrary;
#elif OSWindows
    const DWORD writtenBytes = GetCurrentDirectoryW(bufferSize, directoryCurrent); // _getcwd()
    const PXActionResult pxActionResult = PXErrorCurrent(writtenBytes > 0);

    if(sizeWritten)
    {
        *sizeWritten = writtenBytes;
    }

    return pxActionResult;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXDirectoryCurrentChange(const PXText* const directoryName)
{
    switch (directoryName->Format)
    {
    case TextFormatASCII:
    case TextFormatUTF8:
    {
#if OSUnix
        const int resultID = chdir(directoryName->TextA);
#elif OSWindows
        const BOOL resultID = SetCurrentDirectoryA(directoryName->TextA); // _chdir()
#endif

        const PXActionResult pxActionResult = PXErrorCurrent(resultID);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }

        return PXActionSuccessful;
    }
    case TextFormatUNICODE:
    {
#if OSUnix
        const int resultID = 0; // TODO: Add conversion?
#elif OSWindows
        const BOOL resultID = SetCurrentDirectoryW(directoryName->TextW); // _wchdir()
#endif

        const PXActionResult pxActionResult = PXErrorCurrent(resultID);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }

        return PXActionSuccessful;
    }
    }

    return PXActionInvalidStateImpossible;
}

PXActionResult PXAPI PXDirectoryCurrentGet(PXText* const workingDirectory)
{
    switch(workingDirectory->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            return PXDirectoryCurrentA(workingDirectory->TextA, workingDirectory->SizeAllocated, &workingDirectory->SizeUsed);
        }
        case TextFormatUNICODE:
        {
            return PXDirectoryCurrentW(workingDirectory->TextW, workingDirectory->SizeAllocated, &workingDirectory->SizeUsed);
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }
}

PXActionResult PXAPI PXDirectoryDelete(const PXText* const directoryName)
{
    switch(directoryName->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            return PXDirectoryDeleteA(directoryName->TextA);
        }
        case TextFormatUNICODE:
        {
            return PXDirectoryDeleteW(directoryName->TextW);
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }
}

PXActionResult PXAPI PXDirectoryDeleteA(const char* const directoryName)
{
#if OSUnix || OSForcePOSIXForWindows
    const int resultID = rmdir(directoryName->TextA);
    const PXActionResult removeResult = PXErrorCurrent(0 == resultID);

    return removeResult;

#elif OSWindows
    const PXBool removeResultID = RemoveDirectoryA(directoryName); // Windows XP, Kernel32.dll, fileapi.h
    PXActionResult removeResult = PXErrorCurrent(removeResultID);

    if(PXActionRefusedDirectoryNotEmpty == removeResult)
    {
        // If the director has content, this function will fail.
        // Itterating over everything is stupid, lets use a direct function

        char buffer[_MAX_PATH];
        PXTextPrintA(buffer, _MAX_PATH, "%s\0\0", directoryName);

        SHFILEOPSTRUCT fileOp;
        PXClear(SHFILEOPSTRUCT, &fileOp);
        fileOp.wFunc = FO_DELETE;
        fileOp.pFrom = buffer;
        fileOp.fFlags = FOF_NO_UI; // No user interface

        const int operationResultID = SHFileOperationA(&fileOp);
        removeResult = PXErrorCodeFromID(operationResultID);

        fileOp.pFrom = directoryName;
    }


  





    return removeResult;


#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXDirectoryDeleteW(const wchar_t* const directoryName)
{
#if OSUnix || OSForcePOSIXForWindows
    return PXActionRefusedNotSupportedByLibrary;

#elif OSWindows
    const PXBool removeResultID = RemoveDirectoryW(directoryName); // Windows XP, Kernel32.dll, fileapi.h
    const PXActionResult removeResult = PXErrorCurrent(removeResultID);

    return removeResult;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
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
    }
    while (FindNextFile(iteratorHandle, &findFileData) != 0);

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
    case PXDirectioySpecialFolder_ADMINTOOLS:
        folderID = CSIDL_ADMINTOOLS;
    case PXDirectioySpecialFolderAPPDATA:
        folderID = CSIDL_APPDATA ;
    case PXDirectioySpecialFolderCOMMON_ADMINTOOLS:
        folderID = CSIDL_COMMON_ADMINTOOLS;
    case PXDirectioySpecialFolderCOMMON_APPDATA:
        folderID = CSIDL_COMMON_APPDATA;
    case PXDirectioySpecialFolderCOMMON_DOCUMENTS:
        folderID = CSIDL_COMMON_DOCUMENTS;
    case PXDirectioySpecialFolderCOOKIES:
        folderID = CSIDL_COOKIES;
    case PXDirectioySpecialFolderHISTORY:
        folderID = CSIDL_HISTORY;
    case PXDirectioySpecialFolderINTERNET_CACHE:
        folderID = CSIDL_INTERNET_CACHE;
    case PXDirectioySpecialFolderLOCAL_APPDATA:
        folderID = CSIDL_LOCAL_APPDATA;
    case PXDirectioySpecialFolderPictures:
        folderID = CSIDL_MYPICTURES;
    case PXDirectioySpecialFolderPERSONAL:
        folderID = CSIDL_PERSONAL;
    case PXDirectioySpecialFolderPROGRAM_FILES:
        folderID = CSIDL_PROGRAM_FILES;
    case PXDirectioySpecialFolderPROGRAM_FILES_COMMON:
        folderID = CSIDL_PROGRAM_FILES_COMMON;
    case PXDirectioySpecialFolderSystem:
        folderID = CSIDL_SYSTEM;
    case PXDirectioySpecialFolderWinfows:
        folderID = CSIDL_WINDOWS;

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
