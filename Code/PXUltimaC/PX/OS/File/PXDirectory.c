#include "PXDirectory.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Container/ListDynamic/PXListDynamic.h>
#include <PX/OS/Memory/PXMemory.h>

const char PXDirectoryText[] = "Directory";

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

void PXAPI PXFileElementInfoConvertFrom
(
    PXDirectorySearchCache PXREF pxDirectorySearchCache,
    PXFileEntry PXREF pxFileEntry,
    WIN32_FIND_DATAW PXREF findData,
    PXI8U depth
)
{
    PXText pxTextTemp;
    PXTextFromAdressW(&pxTextTemp, findData->cFileName, PXTextUnkownLength, MAX_PATH);
    PXTextCreateCopy(&pxFileEntry->FilePath, &pxTextTemp);

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

void PXAPI PXDirectoryEntryStore(PXDirectorySearchCache PXREF pxDirectorySearchCache, PXFileEntry PXREF pxFileEntryINPUT)
{
    pxFileEntryINPUT->ID = pxDirectorySearchCache->EntryList.EntryAmountUsed + 100;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectoryText,
        "Search",
        "ID:%3i - %s",
        pxFileEntryINPUT->ID,
        pxFileEntryINPUT->FilePath.A
    );
#endif

    // String is owned by stack, we need to realloc it!
    // Hijack adress, create
    pxFileEntryINPUT->FilePath.Data = PXListDynamicAdd
    (
        &pxDirectorySearchCache->FilePathCache,
        &pxFileEntryINPUT->ID, 
        pxFileEntryINPUT->FilePath.Data,
        pxFileEntryINPUT->FilePath.SizeUsed
    );

    PXListAdd(&pxDirectorySearchCache->EntryList, pxFileEntryINPUT);


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

PXResult PXAPI PXDirectorySearch(PXDirectorySearchCache PXREF pxDirectorySearchCache, const PXText PXREF directoryName)
{
    PXClear(PXDirectorySearchCache, pxDirectorySearchCache);

    PXListDynamicInit(&pxDirectorySearchCache->FilePathCache, sizeof(PXI32U), PXListDynamicSizeObject1Byte);
    PXListInitialize(&pxDirectorySearchCache->EntryList, sizeof(PXFileEntry), 40);

    return PXResultInvalid;

    PXFileEntry pxFileEntry;

    const PXResult open = PXDirectoryOpen(pxDirectorySearchCache, &pxFileEntry, directoryName);

    if(PXResultOK != open)
    {
        return open;
    }

    for(;;)
    {
        PXBool hasEntry = PXDirectoryNext(pxDirectorySearchCache, &pxFileEntry);

        if(!hasEntry)
        {
            break;
        }

        PXDirectoryEntryStore(pxDirectorySearchCache, &pxFileEntry);
    }

    const PXBool close = PXDirectoryClose(pxDirectorySearchCache);


    // Fix stale references because an reallocation could have moved the data
    for(PXSize i = 0; i < pxDirectorySearchCache->EntryList.EntryAmountUsed; ++i)
    {
        PXFileEntry PXREF pxFileEntry = PXListItemAtIndexGetT(PXFileEntry, &pxDirectorySearchCache->EntryList, i);

        PXI32U key = i+100;

        PXListDynamicGet
        (
            &pxDirectorySearchCache->FilePathCache, 
            &key, 
            &pxFileEntry->FilePath.A,
            &pxFileEntry->FilePath.SizeUsed
        );

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectoryText,
            "Search",
            "ID:%3i - %s",
            key,
            pxFileEntry->FilePath.A
        );
#endif
    }

    return PXResultOK;
}

PXResult PXAPI PXDirectoryOpenA(PXDirectorySearchCache PXREF pxDirectorySearchCache, PXFileEntry PXREF pxFileEntry, const char PXREF directoryName)
{
    PXText pxText;
    PXTextFromAdressA(&pxText, directoryName, PXTextLengthUnkown, PXTextLengthUnkown);

    return PXDirectoryOpen(pxDirectorySearchCache, pxFileEntry, &pxText);
}

PXResult PXAPI PXDirectoryOpen(PXDirectorySearchCache PXREF pxDirectorySearchCache, PXFileEntry PXREF pxFileEntry, const PXText PXREF directoryName)
{
    // PXClear(PXDirectorySearchCache, pxDirectorySearchCache);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectoryText,
        "Open",
        "%s",
        directoryName->A
    );
#endif

#if OSUnix

    pxDirectorySearchCache->DirectoryHandleCurrent = opendir(directoryName->A); // dirent.h
    PXResult openResult = PXErrorCurrent(PXNull != pxDirectorySearchCache->DirectoryHandleCurrent);

    return openResult;

#elif OSWindows

    char seachDirectoryKey[PXPathSizeMax];
    char* address = seachDirectoryKey;

    PXBool terminatedBySlash = directoryName->A[directoryName->SizeUsed - 1] == '/';


    address += PXTextCopyA(directoryName->A, directoryName->SizeUsed, address, PXPathSizeMax);

    if(!terminatedBySlash)
    {
        address += PXTextCopyA("/", 1u, address, PXPathSizeMax); // Get all directory and files
    }

    if(PXTextFindFirstCharacterA(directoryName->A, directoryName->SizeUsed, '*') == (PXSize)-1)
    {
        address += PXTextCopyA("*", 1u, address, PXPathSizeMax); // Get all directory and files
    }

    WIN32_FIND_DATAA windowsDirectoryData;

    // First entry
    {
        pxDirectorySearchCache->DirectoryHandleCurrent = FindFirstFileA(seachDirectoryKey, &windowsDirectoryData); // FindFirstFileExW() has literally no additional functionality (for now)
        const PXResult pxResult = PXErrorCurrent(INVALID_HANDLE_VALUE != pxDirectorySearchCache->DirectoryHandleCurrent);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }
    }

    PXFileElementInfoConvertFrom(pxDirectorySearchCache, pxFileEntry, &windowsDirectoryData, 1);

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

    return PXResultOK;

#else
    return PXActionRefusedNotSupported;
#endif
}

PXBool PXAPI PXDirectoryNext(PXDirectorySearchCache PXREF pxDirectorySearchCache, PXFileEntry* pxFileEntry)
{
#if OSUnix
    struct dirent* directoryEntry;

    while ((directoryEntry = readdir(pxDirectorySearchCache->DirectoryHandleCurrent)) != PXNull)
    {
        //const PXBool isSystemDottedFolder = PXDirectoryIsDotFolder(directoryEntry->d);
        //const PXI32U directoryLength = PXTextLength(directory);
        //const PXI32U FileNameLength = strlen(directoryEntry->d_name);
    }
#elif OSWindows

    for(;;)
    {
        // We will always use the UNICODE version, we cant assume ASCII.
        WIN32_FIND_DATAW searchResult;
        const PXBool fetchSuccessful = FindNextFileW(pxDirectorySearchCache->DirectoryHandleCurrent, &searchResult);

        if(!fetchSuccessful)
        {
            return PXFalse;
        }

        PXFileElementInfoConvertFrom(pxDirectorySearchCache, pxFileEntry, &searchResult, 0);

        const PXBool isDotFodler =
            PXFileElementInfoTypeDictionaryRoot == pxFileEntry->Type ||
            PXFileElementInfoTypeDictionaryParent == pxFileEntry->Type;

        if(isDotFodler)
        {
            continue;
        }

        break;
    }

    return PXTrue;

#endif
}

PXBool PXAPI PXDirectoryClose(PXDirectorySearchCache PXREF pxDirectorySearchCache)
{
#if OSUnix
    const int returnCode = closedir(pxDirectorySearchCache->DirectoryHandleCurrent);
    const PXResult pxResult = PXErrorCurrent(0 != returnCode);

    return pxResult;

#elif OSWindows
    const PXBool success = FindClose(pxDirectorySearchCache->DirectoryHandleCurrent);
    const PXResult pxResult = PXErrorCurrent(success);

    return pxResult;
#endif
}

PXResult PXAPI PXDirectoryCreate(const PXText PXREF directoryName)
{
    switch (directoryName->Format)
    {
    case TextFormatASCII:
    case TextFormatUTF8:
    {
#if OSUnix
        const int creationResult = mkdir(directoryName->A, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        const PXBool successCreate = creationResult == 0;
#elif OSWindows

#if OSForcePOSIXForWindows
        const int resultID = _mkdir(directoryName->A);
        const PXBool successCreate = 0 == resultID;
#else
        const PXBool successCreate = CreateDirectoryA(directoryName->A, PXNull); // Windows XP, Kernel32.dll, fileapi.h
#endif

#endif

        const PXResult pxResult = PXErrorCurrent(successCreate);

        if(PXResultOK != pxResult)
        {
            return pxResult;
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
        const int resultID = _wmkdir(directoryName->W);
        const PXBool successCreate = 0 == resultID;
#else
        const PXBool successCreate = CreateDirectoryW(directoryName->W, PXNull); // Windows XP, Kernel32.dll, fileapi.h
#endif

#endif
        const PXResult pxResult = PXErrorCurrent(successCreate);

        if(PXResultOK != pxResult)
        {
            return pxResult;
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

    return PXResultOK;
    */

    return PXResultOK;
}

PXResult PXAPI PXDirectoryCurrentA(char PXREF directoryCurrent, const PXSize bufferSize, PXSize PXREF sizeWritten)
{
#if OSUnix
    const char PXREF text = getcwd(workingDirectory->A, bufferSize);
    const PXResult pxResult = PXErrorCurrent(text);

    return pxResult;

#elif OSWindows
    const DWORD writtenBytes = GetCurrentDirectoryA(bufferSize, directoryCurrent); // _getcwd()
    const PXResult pxResult = PXErrorCurrent(writtenBytes > 0);

    if(sizeWritten)
    {
        *sizeWritten = writtenBytes;
    }

    return pxResult;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXDirectoryCurrentW(wchar_t PXREF directoryCurrent, const PXSize bufferSize, PXSize PXREF sizeWritten)
{
#if OSUnix
    return PXActionRefusedNotSupportedByLibrary;
#elif OSWindows
    const DWORD writtenBytes = GetCurrentDirectoryW(bufferSize, directoryCurrent); // _getcwd()
    const PXResult pxResult = PXErrorCurrent(writtenBytes > 0);

    if(sizeWritten)
    {
        *sizeWritten = writtenBytes;
    }

    return pxResult;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXDirectoryCurrentChange(const PXText PXREF directoryName)
{
    switch (directoryName->Format)
    {
    case TextFormatASCII:
    case TextFormatUTF8:
    {
#if OSUnix
        const int resultID = chdir(directoryName->A);
#elif OSWindows
        const BOOL resultID = SetCurrentDirectoryA(directoryName->A); // _chdir()
#endif

        const PXResult pxResult = PXErrorCurrent(resultID);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }

        return PXResultOK;
    }
    case TextFormatUNICODE:
    {
#if OSUnix
        const int resultID = 0; // TODO: Add conversion?
#elif OSWindows
        const BOOL resultID = SetCurrentDirectoryW(directoryName->W); // _wchdir()
#endif

        const PXResult pxResult = PXErrorCurrent(resultID);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }

        return PXResultOK;
    }
    }

    return PXActionInvalidStateImpossible;
}

PXResult PXAPI PXDirectoryCurrentGet(PXText PXREF workingDirectory)
{
    switch(workingDirectory->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            return PXDirectoryCurrentA(workingDirectory->A, workingDirectory->SizeAllocated, &workingDirectory->SizeUsed);
        }
        case TextFormatUNICODE:
        {
            return PXDirectoryCurrentW(workingDirectory->W, workingDirectory->SizeAllocated, &workingDirectory->SizeUsed);
        }
        default:
            return PXResultRefusedParameterInvalid;
    }
}

PXResult PXAPI PXDirectoryDelete(const PXText PXREF directoryName)
{
    switch(directoryName->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            return PXDirectoryDeleteA(directoryName->A);
        }
        case TextFormatUNICODE:
        {
            return PXDirectoryDeleteW(directoryName->W);
        }
        default:
            return PXResultRefusedParameterInvalid;
    }
}

PXResult PXAPI PXDirectoryDeleteA(const char PXREF directoryName)
{
#if OSUnix || OSForcePOSIXForWindows
    const int resultID = rmdir(directoryName);
    const PXResult removeResult = PXErrorCurrent(0 == resultID);

    return removeResult;

#elif OSWindows
    const PXBool removeResultID = RemoveDirectoryA(directoryName); // Windows XP, Kernel32.dll, fileapi.h
    PXResult removeResult = PXErrorCurrent(removeResultID);

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

PXResult PXAPI PXDirectoryDeleteW(const wchar_t PXREF directoryName)
{
#if OSUnix || OSForcePOSIXForWindows
    return PXActionRefusedNotSupportedByLibrary;

#elif OSWindows
    const PXBool removeResultID = RemoveDirectoryW(directoryName); // Windows XP, Kernel32.dll, fileapi.h
    const PXResult removeResult = PXErrorCurrent(removeResultID);

    return removeResult;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize)
{
    return PXResultInvalid;
}

PXResult PXAPI PXDirectoryFilesInFolderW()
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

    return PXResultOK;
}

PXResult PXAPI PXDirectorySpecialFolderGet(const PXDirectioySpecialFolder pxDirectioySpecialFolder, PXText PXREF pxTextSpecialFolder, PXText PXREF pxTextFileName, const PXBool create)
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

    const HRESULT resultErrorCodeID = SHGetFolderPathA(PXNull, folderID, 0, 0, pxTextSpecialFolder->A); // Windows 2000, Shell32.lib, Shlobj.h (SHGetSpecialFolderPathA might not be supported?)

    if (resultErrorCodeID != S_OK)
    {
        return PXResultInvalid;
    }

    pxTextSpecialFolder->SizeUsed = PXTextLengthA(pxTextSpecialFolder->A, PXPathSizeMax);

    if (!pxTextFileName)
    {
        return PXResultOK;
    }

    const PXSize fullSize = PXAppend(pxTextSpecialFolder, pxTextFileName);

    if (fullSize == 0)
    {
        return PXResultInvalid;
    }

    return PXResultOK;
#elif
    return PXActionRefusedNotSupported;
#endif
}
