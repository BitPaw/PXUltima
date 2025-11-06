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
    if(!(pxLibrary && filePath))
    {
        return PXActionRefusedArgumentNull;
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
                "Open",
                "<%s>",
                filePath->A
            );
#endif

#if OSUnix
            dlerror(); // Clear any existing error

            const int mode = RTLD_NOW;
            pxLibrary->ID = dlopen(filePath->A, mode); // dlfcn.h   

#elif PXOSWindowsDestop
            SetLastError(0);

            pxLibrary->ID = LoadLibraryA(filePath->A); // Windows XP, Kernel32.dll, libloaderapi.h
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
                "Open",
                "<%ls>",
                filePath->A
            );
#endif

#if OSUnix
            return 0;

#elif PXOSWindowsDestop
            SetLastError(0);

            pxLibrary->ID = LoadLibraryW(filePath->W); // Windows XP, Kernel32.dll, libloaderapi.h
#else
            return PXActionRefusedNotSupported;
#endif
            break;
        }
    }

    const PXResult pxActionResult = PXErrorCurrent(PXNull != pxLibrary->ID);

    if(PXActionSuccessful != pxActionResult)
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

    PXDebugModuleNameGet(pxLibrary->ID, libraryPathFull, PXPathSizeMax, PXNull, PXDebugModuleNameFull);

    PXLogPrint
    (
        PXLoggingInfo,
        PXLibraryText,
        "Open",
        "<%p> - <%s>",
        pxLibrary->ID,
        libraryPathFull
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXLibraryOpenA(PXLibrary PXREF pxLibrary, const char PXREF filePath)
{
    PXText pxText;
    PXTextFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXPathSizeMax);

    return PXLibraryOpen(pxLibrary, &pxText);
}

PXResult PXAPI PXLibraryOpenW(PXLibrary PXREF pxLibrary, const wchar_t PXREF filePath)
{
    PXText pxText;
    PXTextFromAdressW(&pxText, filePath, PXTextLengthUnkown, PXPathSizeMax);

    return PXLibraryOpen(pxLibrary, &pxText);
}

PXResult PXAPI PXLibraryClose(PXLibrary PXREF pxLibrary)
{
#if PXLogEnable
    char moduleName[97];

    PXDebugModuleNameGet(pxLibrary->ID, moduleName, 64, PXNull, PXDebugModuleNameShort);

    PXLogPrint
    (
        PXLoggingDeallocation,
        PXLibraryText,
        "Release",
        "%s",
        moduleName
    );
#endif

    const PXBool result =
#if OSUnix
        dlclose(pxLibrary->ID);
#elif OSWindows
        FreeLibrary(pxLibrary->ID); // Windows XP (+UWP), Kernel32.dll, libloaderapi.h
#endif

    const PXResult pxActionResult = PXErrorCurrent(result);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    pxLibrary->ID = PXNull;

    return PXActionSuccessful;
}

PXBool PXAPI PXLibraryGetSymbolBinding(PXLibrary PXREF pxLibrary, void* PXREF bindingObject, const char PXREF symbolList, const PXSize amount, const PXBool areAllImportant)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLibraryText,
        "Biding",
        "Load for <%p>...",
        pxLibrary->ID
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
        pxLibrary->ID
    );
#endif

    return PXTrue;
}

PXBool PXAPI PXLibraryGetSymbolListA(PXLibrary PXREF pxLibrary, PXLibraryFuntionEntry PXREF pxLibraryFuntionEntryList, const PXSize amount)
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

    return PXTrue;
}

PXBool PXAPI PXLibraryGetSymbolA(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const char PXREF symbolName, const PXBool isImportant)
{
#if PXLogEnable
    char libraryName[64];

    PXDebugModuleNameGet(pxLibrary->ID, libraryName, 64, PXNull, PXDebugModuleNameShort);
#endif

#if OSUnix
    *libraryFunction = (void*)dlsym(pxLibrary->ID, symbolName);
    //const char* errorString = dlerror();
#elif OSWindows
    *libraryFunction = (void*)GetProcAddress(pxLibrary->ID, symbolName); // Windows XP, Kernel32.dll, libloaderapi.h
#endif
    const PXResult pxActionResult = PXErrorCurrent(PXNull != *libraryFunction);

    // If the fetch is not important, we dont want to trigger a failure.
    {
        if(PXActionSuccessful != pxActionResult)
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

            return PXFalse;
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

    return PXTrue;
}

PXBool PXAPI PXLibraryGetSymbol(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const PXText* symbolName)
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
    ULONG moduleBase,
    ULONG moduleSize,
    PXWindowsEnumCallBackRope* userContext
) 
{
    ++userContext->Amount;

    char bufferName[128];

    HMODULE moduleHanlde =  GetModuleHandleA(moduleName);
    PXDebugModuleNameGet(moduleHanlde, bufferName, 128, PXNull, PXDebugModuleNameShort);

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

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}