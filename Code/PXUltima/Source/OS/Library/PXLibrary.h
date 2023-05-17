#ifndef PXLibraryINCLUDE
#define PXLibraryINCLUDE

#include <OS/System/OSVersion.h>

#if OSUnix
#include <sys/types.h>
#include <dlfcn.h>

typedef void* LibraryFunction;
typedef void* LibraryDirectoryID;

#elif OSWindows
#include <windows.h>

typedef FARPROC LibraryFunction;
//typedef DLL_DIRECTORY_COOKIE LibraryDirectoryID;
#endif

#include <Media/PXText.h>
#include <OS/Process/PXProcess.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct PXLibrary_
    {
        PXProcessHandle ProcessHandle;

#if OSUnix
        void* ID;
#elif OSWindows
        HMODULE ID; // LibraryHandle, HINSTANCE (semms is also okey)
#endif
    }
    PXLibrary;

    PXPublic PXBool PXLibraryOpen(PXLibrary* const pxLibrary, const PXText* const filePath); //  gain access to an executable object file. RTLD_LAZY
    PXPublic PXBool PXLibraryClose(PXLibrary* const pxLibrary); // close a dlopen object
    PXPublic PXBool PXLibraryGetSymbol(PXLibrary* const pxLibrary, LibraryFunction* libraryFunction, const char* symbolName); // obtain the address of a symbol from a dlopen object

    PXPublic PXActionResult PXLibraryName(PXLibrary* const pxLibrary, PXText* const libraryName);

    PXPublic PXBool PXLibraryParseSymbols();

    // void SymbolVector(); // PXProgramming interface to dynamic linking loader.

     //static ErrorCode SearchDirectoryAdd(const wchar_t* directoryPath, LibraryDirectoryID& libraryDirectoryID);
     //static ErrorCode SearchDirectoryRemove(LibraryDirectoryID& libraryDirectoryID);

#ifdef __cplusplus
}
#endif

#endif
