#ifndef PXLibraryINCLUDE
#define PXLibraryINCLUDE

#include <Media/PXResource.h>
#include <Media/PXText.h>
#include <OS/Async/PXProcess.h>

typedef struct PXLibraryFuntionEntry_
{
    void** Function;
    const char* FuntionName;
}
PXLibraryFuntionEntry;

typedef struct PXLibrary_
{
#if OSUnix
    void* ID;
#elif OSWindows
    HMODULE ID; // LibraryHandle, HINSTANCE (semms is also okey)
#endif

    PXProcessHandle ProcessHandle;
}
PXLibrary;

PXPublic PXActionResult PXAPI PXLibraryOpen(PXLibrary* const pxLibrary, const PXText* const filePath); //  gain access to an executable object file. RTLD_LAZY
PXPublic PXActionResult PXAPI PXLibraryOpenA(PXLibrary* const pxLibrary, const char* const filePath);
PXPublic PXActionResult PXAPI PXLibraryClose(PXLibrary* const pxLibrary); // close a dlopen object

PXPublic PXBool PXAPI PXLibraryGetSymbolListA(PXLibrary* const pxLibrary, PXLibraryFuntionEntry* const pxLibraryFuntionEntryList, const PXSize amount);
PXPublic PXBool PXAPI PXLibraryGetSymbolA(PXLibrary* const pxLibrary, void** const libraryFunction, const char* const symbolName);
PXPublic PXBool PXAPI PXLibraryGetSymbol(PXLibrary* const pxLibrary, void** const libraryFunction, const PXText* symbolName); // obtain the address of a symbol from a dlopen object

PXPublic PXActionResult PXAPI PXLibraryName(PXLibrary* const pxLibrary, PXText* const libraryName);

#endif