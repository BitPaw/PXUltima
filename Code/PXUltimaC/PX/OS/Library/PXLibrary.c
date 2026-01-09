#include "PXLibrary.h"

#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

#if OSUnix
#include <sys/types.h>
#include <dlfcn.h>


typedef void* LibraryFunction;
typedef void* LibraryDirectoryID;

#elif OSWindows
//#include <dbghelp.h> // MISSING
#include <windows.h>
#include <DbgHelp.h>
#include <stdio.h>
#include <Psapi.h> // Psapi.lib
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/PXOS.h>

#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "Psapi.lib " )


//typedef FARPROC LibraryFunction;

typedef int (PXAPI* LibraryFunction)();

//typedef DLL_DIRECTORY_COOKIE LibraryDirectoryID;

/* PSYMBOL_INFO:MISSING

BOOL CALLBACK EnumSymProc(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
    UNREFERENCED_PARAMETER(UserContext);

    printf("%08X %4u %s\n",    pSymInfo->Address, SymbolSize, pSymInfo->Name);

    return TRUE;
}*/

#endif


const char PXLibraryText[] = "Library";
const char PXLibraryLoadText[] = "Load";







// SymGetModuleInfo64



/*
dlopen() - gain access to an executable object file
dclose() - close a dlopen object
dlsym() - obtain the address of a symbol from a dlopen object
dlvsym() - PXProgramming interface to dynamic linking loader.
dlerror() - get diagnostic information
*/

/*
BF::ErrorCode BF::Library::SearchDirectoryAdd(const wchar_t* directoryPath, LibraryDirectoryID& libraryDirectoryID)
{
#if OSUnix
#elif OSWindows
    const DLL_DIRECTORY_COOKIE cookie = AddDllDirectory(directoryPath);
    const bool successful = cookie != 0;

    if(!successful)
    {
        const ErrorCode errorCode = GetCurrentError();

        return errorCode;
    }

    libraryDirectoryID = cookie;
#endif

    return ErrorCode::Successful;
}

BF::ErrorCode BF::Library::SearchDirectoryRemove(LibraryDirectoryID& libraryDirectoryID)
{
#if OSUnix
#elif OSWindows
    const bool successful = RemoveDllDirectory(libraryDirectoryID);

    if(!successful)
    {
        const ErrorCode errorCode = GetCurrentError();

        return errorCode;
    }

    libraryDirectoryID = 0;
#endif

    return ErrorCode::Successful;
}*/


PXResult PXAPI PXLibraryOpen(PXLibrary PXREF pxLibrary, const PXText PXREF filePath)
{
    //  gain access to an executable object file. RTLD_LAZY

    if(!(pxLibrary && filePath))
    {
        return PXResultRefusedParameterNull;
    }

    PXClear(PXLibrary, pxLibrary);

    switch(filePath->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingAllocation,
                PXLibraryText,
                PXLibraryLoadText,
                "<%s>",
                filePath->A
            );
#endif

#if OSUnix
            dlerror(); // Clear any existing error

            const int mode = RTLD_NOW;
            pxLibrary->ModuleHandle = dlopen(filePath->A, mode); // dlfcn.h   

#elif PXOSWindowsDestop
            SetLastError(0);

            pxLibrary->ModuleHandle = LoadLibraryA(filePath->A); // Windows XP, Kernel32.dll, libloaderapi.h
#else
            return PXActionRefusedNotSupported;
#endif
            break;
        }

        case TextFormatUNICODE:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingAllocation,
                PXLibraryText,
                PXLibraryLoadText,
                "<%ls>",
                filePath->W
            );
#endif

#if OSUnix
            return 0;

#elif PXOSWindowsDestop
            SetLastError(0);

            pxLibrary->ModuleHandle = LoadLibraryW(filePath->W); // Windows XP, Kernel32.dll, libloaderapi.h
#else
            return PXActionRefusedNotSupported;
#endif
            break;
        }
    }

    const PXResult pxActionResult = PXErrorCurrent(PXNull != pxLibrary->ModuleHandle);

    if(PXResultOK != pxActionResult)
    {
        // Library couln't be opened

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXLibraryText,
            "Open",
            "Can't be opened <%s>",
            filePath->A
        );
#endif

        return pxActionResult;
    }

#if PXLogEnable

    char libraryPathFull[PXPathSizeMax];

    PXText pxText;
    PXTextFromAdressA(&pxText, libraryPathFull, 0, sizeof(libraryPathFull));
    PXLibraryName(pxLibrary->ModuleHandle, &pxText); // PXDebugModuleNameFull

    PXLogPrint
    (
        PXLoggingInfo,
        PXLibraryText,
        "Open",
        "<%p> - <%s>",
        pxLibrary->ModuleHandle,
        libraryPathFull
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXLibraryClose(PXLibrary PXREF pxLibrary)
{
#if PXLogEnable
    char libraryPathFull[PXPathSizeMax];

    PXText pxText;
    PXTextFromAdressA(&pxText, libraryPathFull, 0, sizeof(libraryPathFull));
    PXLibraryName(pxLibrary->ModuleHandle, &pxText); // PXDebugModuleNameFull

    PXLogPrint
    (
        PXLoggingDeallocation,
        PXLibraryText,
        "Release",
        "%s",
        libraryPathFull
    );
#endif

    const PXBool result =
#if OSUnix
        dlclose(pxLibrary->ModuleHandle);
#elif OSWindows
        FreeLibrary(pxLibrary->ModuleHandle); // Windows XP (+UWP), Kernel32.dll, libloaderapi.h
#endif

    const PXResult pxActionResult = PXErrorCurrent(result);

    if(PXResultOK != pxActionResult)
    {
        return pxActionResult;
    }

    pxLibrary->ModuleHandle = PXNull;

    return PXResultOK;
}

PXResult PXAPI PXLibraryGetSymbolBinding(PXLibrary PXREF pxLibrary, void* PXREF bindingObject, const char PXREF symbolList, const PXSize amount, const PXBool areAllImportant)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLibraryText,
        "Biding",
        "Load for <%p>...",
        pxLibrary->ModuleHandle
    );
#endif

    PXSize position = 0;

    for(PXSize i = 0; ; ++i)
    {
        const char PXREF cursor = &symbolList[position];
        const PXSize length = PXTextLengthA(cursor, PXTextUnkownLength);
        void** function = &bindingObject[i];

        const PXBool invalidToProceed = !cursor || (i >= amount) || length == 0;

        if(invalidToProceed)
        {
            break;
        }

        position += length + 1;

        PXLibraryGetSymbolA(pxLibrary, function, cursor, areAllImportant);
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLibraryText,
        "Biding",
        "Done for <%p>...",
        pxLibrary->ModuleHandle
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXLibraryGetSymbolListA(PXLibrary PXREF pxLibrary, PXLibraryFuntionEntry PXREF pxLibraryFuntionEntryList, const PXSize amount)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingAllocation,
        PXLibraryText,
        "Load",
        "Symbol batch (%i)",
        amount
    );
#endif

    for(PXSize i = 0; i < amount; ++i)
    {
        PXLibraryFuntionEntry* pxLibraryFuntionEntry = &pxLibraryFuntionEntryList[i];

        PXLibraryGetSymbolA(pxLibrary, pxLibraryFuntionEntry->Function, pxLibraryFuntionEntry->FuntionName, PXTrue);
    }

    return PXResultOK;
}

PXResult PXAPI PXLibraryGetSymbolA(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const char PXREF symbolName, const PXBool isImportant)
{
#if PXLogEnable
    char libraryName[64];

    PXText pxText;
    PXTextFromAdressA(&pxText, libraryName, 0, sizeof(libraryName));
    PXLibraryName(pxLibrary, &pxText); // PXDebugModuleNameShort
#endif

#if OSUnix
    *libraryFunction = (void*)dlsym(pxLibrary->ModuleHandle, symbolName);
    //const char* errorString = dlerror();
#elif OSWindows
    *libraryFunction = (void*)GetProcAddress(pxLibrary->ModuleHandle, symbolName); // Windows XP, Kernel32.dll, libloaderapi.h
#endif
    const PXResult pxActionResult = PXErrorCurrent(PXNull != *libraryFunction);

    // If the fetch is not important, we dont want to trigger a failure.
    {
        if(PXResultOK != pxActionResult)
        {
            if(isImportant) // If it is not importendt, aka not an important/optional function/feature, we we dont send an error
            {
#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingWarning,
                    PXLibraryText,
                    "Symbol-Fetch",
                    "<%s::%s()> does not exist!",
                    libraryName,
                    symbolName
                );
#endif
            }

            return pxActionResult;
        } 
    }

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXLibraryText,
        "Symbol-Fetch",
        "<%p> - <%s::%s>",
        *libraryFunction,
        libraryName,
        symbolName
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXLibraryGetSymbol(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const PXText* symbolName)
{
    return PXLibraryGetSymbolA(pxLibrary, libraryFunction, symbolName->A, PXTrue);
}



#if OSWindows
typedef struct PXWindowsEnumCallBackRope_
{
    void** ListObject;
    PXSize Amount;
}
PXWindowsEnumCallBackRope;

static BOOL CALLBACK PXWindowsLibraryLoadedEnumCallback
(
    PCSTR moduleName,
    ULONG moduleBase, // DWORD64
    ULONG moduleSize,
    PXWindowsEnumCallBackRope* userContext
) 
{
    PXLibrary pxLibrary;

    ++userContext->Amount;

    char bufferName[128];

    pxLibrary.ModuleHandle =  GetModuleHandleA(moduleName);

    PXText pxText;
    PXTextFromAdressA(&pxText, bufferName, 0, sizeof(bufferName));
    PXLibraryName(&pxLibrary, &pxText); // PXDebugModuleNameShort

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLibraryText,
        "Module-Detect",
        "[%3i] <%p> %8i %s",
        userContext->Amount,
        moduleBase,
        moduleSize,
        bufferName
    );
#endif
}
#endif


PXResult PXAPI PXLibraryCurrentlyLoaded(PXProcessHandle pxProcessHandle, PXLibrary* PXREF pxLibraryList, PXSize PXREF amount)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    if(!pxProcessHandle)
    {
        pxProcessHandle = GetCurrentProcess();
    }

    PXWindowsEnumCallBackRope pxWindowsEnumCallBackRope;
    pxWindowsEnumCallBackRope.Amount = 0;
    pxWindowsEnumCallBackRope.ListObject = pxLibraryList;
        
    EnumerateLoadedModulesEx(pxProcessHandle, PXWindowsLibraryLoadedEnumCallback, &pxWindowsEnumCallBackRope);

    if(amount)
    {
        *amount = pxWindowsEnumCallBackRope.Amount;
    }

    return PXResultOK;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXLibraryName(PXLibrary PXREF pxLibrary, PXText PXREF pxTextLibraryName)
{
#if OSUnix
#elif OSWindows
    switch(pxTextLibraryName->Format)
    {
        case TextFormatASCII:
        {
            pxTextLibraryName->SizeUsed = GetModuleFileNameA
            (
                pxLibrary->ModuleHandle,
                pxTextLibraryName->A,
                pxTextLibraryName->SizeAllocated
            );
            break;
        }
        case TextFormatUNICODE:
        {
            pxTextLibraryName->SizeUsed = GetModuleFileNameW
            (
                pxLibrary->ModuleHandle,
                pxTextLibraryName->W,
                pxTextLibraryName->SizeAllocated
            );
            break;
        }
        default:
            return PXResultRefusedParameterInvalid;
    }

    PXResult pxResult = PXErrorCurrent(0 < pxTextLibraryName->SizeUsed);

    if(PXResultOK != pxResult)
    {
        PXTextPrint(pxTextLibraryName, "???");
        return pxResult;
    }

    const PXSize index = PXTextFindLastCharacterA(pxTextLibraryName->A, -1, '\\');

    if(-1 != index)
    {
        char* source = &pxTextLibraryName->A[index + 1];
        DWORD sourceSize = pxTextLibraryName->SizeUsed - index - 1;

        pxTextLibraryName->SizeUsed = PXTextCopyA(source, sourceSize, pxTextLibraryName->A, sourceSize);
    }

    return PXResultOK;
#endif
}

PXResult PXAPI PXLibraryNameFromAdress
(
    PXLibrary* pxLibraryREF, 
    PXText PXREF pxTextLibraryName, 
    const void PXREF adress
)
{
    PXLibrary placeHolder;

    if(!pxLibraryREF)
    {
        PXClear(PXLibrary, &placeHolder);
        pxLibraryREF = &placeHolder;
    }

    PXResult pxResult = PXLibraryFromAdress(pxLibraryREF, adress);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxResult = PXLibraryName(pxLibraryREF, pxTextLibraryName);

    return pxResult;
}

PXResult PXAPI PXLibraryFromAdress(PXLibrary PXREF pxLibrary, const void PXREF adress)
{
#if OSUnix && 0
    Dl_info info;

    const int resultID = dladdr(adress, &info); // dlfcn.h
    const PXResult moduleFetchResult = PXErrorCurrent(0 != resultID);

    return moduleFetchResult;
#elif OSWindows
    const PXBool moduleFetchSuccess = GetModuleHandleEx
    (
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCTSTR)adress,
        &pxLibrary->ModuleHandle
    );
    const PXResult moduleFetchResult = PXErrorCurrent(moduleFetchSuccess);

    return moduleFetchResult;
#else
    pxLibrary->ModuleHandle = PXNull;

    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}


/*



  char moduleNameBuffer[260];
    PXClearList(char, moduleNameBuffer, 260);
    PXSize moduleNameLength = 0;

    PXActionResult moduleFetchResult;

    // Stage 1) Module to string
#if OSUnix

    if(pxHandleModule)
    {
        /*
        Dl_info info;

        const int resultID = dladdr(current_module, &info);
        const PXResult moduleFetchResult = PXErrorCurrent((0 != resultID));

        if(info.dli_fname)
        {
            printf("Current shared object: %s\n", info.dli_fname);
        }
        else {
            printf("Could not determine the current shared object.\n");
            * /
    }
    else
    {
        // The HANDLE is NULL. So we want the path of the current executable
        moduleNameLength = readlink("/proc/self/exe", moduleName, PXPathSizeMax); // unistd.h
        moduleFetchResult = PXErrorCurrent(-1 != count);
    }

#elif OSWindows

moduleNameLength = GetModuleFileNameA(pxHandleModule, moduleNameBuffer, 260);
moduleFetchResult = PXErrorCurrent(0 != moduleNameLength);

#else
return PXActionRefusedNotSupportedByLibrary;
#endif


if(PXResultOK != moduleFetchResult)
{
    moduleName[0] = '?';
    moduleName[1] = '?';
    moduleName[2] = '?';
    moduleName[3] = '\0';

    if(sizeWritten)
    {
        *sizeWritten = 3;
    }

    return moduleFetchResult;
}


// Stage 2) Truncate name
if(!(PXDebugModuleNameShort & flags))
{
    PXTextReplaceByte(moduleNameBuffer, moduleNameLength, '\\', '/');
    PXTextCopyA(moduleNameBuffer, moduleNameLength, moduleName, moduleNameSize);


    if(sizeWritten)
    {
        *sizeWritten = moduleNameLength;
    }

    return PXResultOK; // We dont do any further processing
}




#if OSUnix

moduleName[count] = '\0';
*sizeWritten = count;

// Truncate
const PXSize lastSlashPosition = PXTextFindLastCharacterA(moduleName, count, '/');

if(lastSlashPosition != -1)
{
    PXSize sourceSize = count - lastSlashPosition;
    char* source = &moduleName[lastSlashPosition];

    *sizeWritten = PXMemoryMove(source, sourceSize, moduleName, moduleNameSize);
}


#elif OSWindows


// net to get moduleinto, but we have those
// IMAGEHLP_MODULE64 mMAGEHLP_MODULE64;
// mMAGEHLP_MODULE64.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
//  const PXBool moduleFetchSuccessaSAS = SymGetModuleInfo64(processHandle, moduleHandle, &mMAGEHLP_MODULE64);


// Extract names
{
    char* molduleNamefixed = moduleNameBuffer;
    PXSize actualSize = moduleNameLength;

    const char systemPath[] = "C:\\WINDOWS\\SYSTEM32\\";
    const PXSize systemPathSize = sizeof(systemPath);
    const PXBool isSystem = PXTextCompareIgnoreCaseA(systemPath, systemPathSize, moduleNameBuffer, systemPathSize);

    if(isSystem)
    {
        molduleNamefixed += systemPathSize - 1;
        actualSize -= systemPathSize - 1;

        // Check if its a driver
        const char driverStoreText[] = "DriverStore";
        const PXSize driverStoreTextSize = sizeof(driverStoreText);

        const PXBool isDriver = PXTextCompareA(driverStoreText, driverStoreTextSize, molduleNamefixed, driverStoreTextSize, 0);

        if(isDriver)
        {
            // The path is very long and useless, only get the DLL name.
            const PXSize lastSlash = PXTextFindLastCharacterA(molduleNamefixed, actualSize, '\\');

            if(lastSlash != -1)
            {
                molduleNamefixed += lastSlash + 1;
                actualSize -= lastSlash + 1;
            }
        }
    }
    else
    {
        char currentWorkPath[MAX_PATH];
        PXClearList(char, currentWorkPath, MAX_PATH);
        PXSize currentWorkPathSize = GetModuleFileName(NULL, currentWorkPath, MAX_PATH);


        PXSize lastSlashA = PXTextFindLastCharacterA(currentWorkPath, currentWorkPathSize, '\\');
        PXSize lastSlashB = PXTextFindLastCharacterA(moduleNameBuffer, currentWorkPathSize, '\\');
        PXBool isRelativePath = PXTextCompareA(currentWorkPath, lastSlashA, moduleNameBuffer, lastSlashB, 0);

        if(isRelativePath)
        {
            molduleNamefixed += lastSlashA + 1;
            actualSize -= lastSlashA + 1;
        }
    }

    moduleNameLength = PXTextCopyA(molduleNamefixed, actualSize, moduleName, 260);
}
#endif

if(sizeWritten)
{
    *sizeWritten = moduleNameLength;
}

return PXResultOK;












*/
