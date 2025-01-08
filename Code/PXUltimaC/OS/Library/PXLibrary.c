#include "PXLibrary.h"

#include <OS/Error/PXActionResult.h>
#include <OS/Memory/PXMemory.h>
#include <OS/File/PXFile.h>
#include <OS/Console/PXConsole.h>

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
#include <OS/Debug/PXDebug.h>

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


PXActionResult PXAPI PXLibraryOpen(PXLibrary* const pxLibrary, const PXText* const filePath)
{
    PXClear(PXLibrary, pxLibrary);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingAllocation,
        "Library",
        "Open",
        "<%s>",
        filePath->TextA
    );
#endif

    switch(filePath->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
#if OSUnix
            dlerror(); // Clear any existing error

            const int mode = RTLD_NOW;
            pxLibrary->ID = dlopen(filePath->TextA, mode); // dlfcn.h   

#elif PXOSWindowsDestop
            SetLastError(0);

            pxLibrary->ID = LoadLibraryA(filePath->TextA); // Windows XP, Kernel32.dll, libloaderapi.h
#else
            return PXActionRefusedNotSupported;
#endif
            break;
        }

        case TextFormatUNICODE:
        {
#if OSUnix
            return 0;

#elif PXOSWindowsDestop
            SetLastError(0);

            pxLibrary->ID = LoadLibraryW(filePath->TextW); // Windows XP, Kernel32.dll, libloaderapi.h
#else
            return PXActionRefusedNotSupported;
#endif
            break;
        }
    }

    const PXActionResult pxActionResult = PXErrorCurrent(PXNull != pxLibrary->ID);

    if(PXActionSuccessful != pxActionResult)
    {
        // Library couln't be opened

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "Library",
            "Open",
            "Can't be opened <%s>",
            filePath->TextA
        );
#endif

        return pxActionResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Library",
        "Open",
        "Successful <%s> -> <0x%p>",
        filePath->TextA,
        pxLibrary->ID
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXLibraryOpenA(PXLibrary* const pxLibrary, const char* const filePath)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXPathSizeMax);

    return PXLibraryOpen(pxLibrary, &pxText);
}

PXActionResult PXAPI PXLibraryClose(PXLibrary* const pxLibrary)
{
#if PXLogEnable
    char moduleName[32];

    PXDebugModuleHandleToName(pxLibrary->ID, moduleName);

    PXLogPrint
    (
        PXLoggingDeallocation,
        "Library",
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

    const PXActionResult pxActionResult = PXErrorCurrent(result);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    pxLibrary->ID = PXNull;

    return PXActionSuccessful;
}

PXBool PXAPI PXLibraryGetSymbolBinding(PXLibrary* const pxLibrary, void** const bindingObject, const char* const symbolList, const PXSize amount, const PXBool areAllImportant)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Library",
        "Biding",
        "Load for <%p>...",
        pxLibrary->ID
    );
#endif

    PXSize position = 0;

    for(PXSize i = 0; ; ++i)
    {
        const char* const cursor = &symbolList[position];
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
        "Library",
        "Biding",
        "Done for <%p>...",
        pxLibrary->ID
    );
#endif

    return PXTrue;
}

PXBool PXAPI PXLibraryGetSymbolListA(PXLibrary* const pxLibrary, PXLibraryFuntionEntry* const pxLibraryFuntionEntryList, const PXSize amount)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingAllocation,
        "Library",
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

PXBool PXAPI PXLibraryGetSymbolA(PXLibrary* const pxLibrary, void** const libraryFunction, const char* const symbolName, const PXBool isImportant)
{
#if OSUnix
    *libraryFunction = (void*)dlsym(pxLibrary->ID, symbolName);
    //const char* errorString = dlerror();
#elif OSWindows
    *libraryFunction = (void*)GetProcAddress(pxLibrary->ID, symbolName); // Windows XP, Kernel32.dll, libloaderapi.h
#endif
    const PXActionResult pxActionResult = PXErrorCurrent(PXNull != *libraryFunction);

    // If the fetch is not important, we dont want to trigger a failure.
    {
        if(PXActionSuccessful != pxActionResult)
        {
            if(isImportant) // If it is not importendt, aka not an important/optional function/feature, we we dont send an error
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingWarning,
                    "Library",
                    "Symbol-Fetch",
                    "Missing <%s>",
                    symbolName
                );
#endif
            }

            return PXFalse;
        } 
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Library",
        "Symbol-Fetch",
        "<%p>::<%s>::<%p>",
        pxLibrary->ID,
        symbolName,
        *libraryFunction
    );
#endif

    return PXTrue;
}

PXBool PXAPI PXLibraryGetSymbol(PXLibrary* const pxLibrary, void** const libraryFunction, const PXText* symbolName)
{
    return PXLibraryGetSymbolA(pxLibrary, libraryFunction, symbolName->TextA, PXTrue);
}

PXActionResult PXAPI PXLibraryName(PXLibrary* const pxLibrary, PXText* const libraryName)
{
    PXMemoryClear(libraryName->TextA, libraryName->SizeAllocated);

    switch(libraryName->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
#if OSUnix
            return PXActionRefusedNotImplemented;

#elif OSWindows
            libraryName->SizeUsed = GetModuleFileNameExA // Windows XP, Kernel32.dll, psapi.h
            (
                pxLibrary->ProcessHandle,
                pxLibrary->ID,
                libraryName->TextA,
                libraryName->SizeAllocated
            );
            const PXActionResult pxActionResult = PXErrorCurrent(0 == libraryName->SizeUsed);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }
#endif

            break;
        }

        case TextFormatUNICODE:
        {
#if OSUnix
            return PXActionRefusedNotImplemented;

#elif OSWindows
            libraryName->SizeUsed = GetModuleFileNameExW // Windows XP, Kernel32.dll, psapi.h
            (
                pxLibrary->ProcessHandle,
                pxLibrary->ID,
                libraryName->TextW,
                libraryName->SizeAllocated
            );
            const PXActionResult pxActionResult = PXErrorCurrent(0 == libraryName->SizeUsed);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }
#endif
            break;
        }
    }

    return PXActionSuccessful;
}