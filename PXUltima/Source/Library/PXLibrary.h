#ifndef PXLibraryINCLUDE
#define PXLibraryINCLUDE

#include <OS/OSVersion.h>

#if OSUnix
#include <sys/types.h>
#include <dlfcn.h>

typedef void* LibraryHandle;
typedef void* LibraryFunction;
typedef void* LibraryDirectoryID;

#elif OSWindows
#include <windows.h>

typedef HMODULE LibraryHandle;// HINSTANCE (semms is also okey)
typedef FARPROC LibraryFunction;
typedef DLL_DIRECTORY_COOKIE LibraryDirectoryID;
#endif

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct PXLibrary_
    {
        LibraryHandle ID;
    }
    PXLibrary;


    PXPublic PXBool LibraryOpenA(PXLibrary* const pxLibrary, const char* filePath); //  gain access to an executable object file. RTLD_LAZY
    PXPublic PXBool LibraryOpenW(PXLibrary* const pxLibrary, const wchar_t* filePath); //  gain access to an executable object file. RTLD_LAZY
    PXPublic PXBool LibraryClose(PXLibrary* const pxLibrary); // close a dlopen object
    PXPublic PXBool LibraryGetSymbol(PXLibrary* const pxLibrary, LibraryFunction* libraryFunction, const char* symbolName); // obtain the address of a symbol from a dlopen object

    PXPublic PXBool LibraryParseSymbols();

    // void SymbolVector(); // Programming interface to dynamic linking loader.

     //static ErrorCode SearchDirectoryAdd(const wchar_t* directoryPath, LibraryDirectoryID& libraryDirectoryID);
     //static ErrorCode SearchDirectoryRemove(LibraryDirectoryID& libraryDirectoryID);

#ifdef __cplusplus
}
#endif

#endif
