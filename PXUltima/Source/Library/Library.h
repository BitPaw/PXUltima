#ifndef LibraryInclude
#define LibraryInclude

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
    PXPublic PXBool LibraryOpenA(LibraryHandle* handle, const char* filePath); //  gain access to an executable object file. RTLD_LAZY
    PXPublic PXBool LibraryOpenW(LibraryHandle* handle, const wchar_t* filePath); //  gain access to an executable object file. RTLD_LAZY
    PXPublic PXBool LibraryClose(LibraryHandle* handle); // close a dlopen object
    PXPublic PXBool LibraryGetSymbol(LibraryHandle* handle, LibraryFunction* libraryFunction, const char* symbolName); // obtain the address of a symbol from a dlopen object

    PXPublic PXBool LibraryParseSymbols();

   // void SymbolVector(); // Programming interface to dynamic linking loader.

    //static ErrorCode SearchDirectoryAdd(const wchar_t* directoryPath, LibraryDirectoryID& libraryDirectoryID);
    //static ErrorCode SearchDirectoryRemove(LibraryDirectoryID& libraryDirectoryID);

#ifdef __cplusplus
}
#endif

#endif
