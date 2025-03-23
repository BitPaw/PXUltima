#ifndef PXLibraryINCLUDE
#define PXLibraryINCLUDE

#include <PX/Media/PXResource.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Async/PXProcess.h>


// Handle to the module used for libraray IDs
typedef
#if OSUnix
void*
#elif OSWindows
HMODULE // HINSTANCE(semms is also okey)
#else
void*
#endif
PXHandleModule;


typedef struct PXLibraryFuntionEntry_
{
    void** Function;
    const char* FuntionName;
}
PXLibraryFuntionEntry;

typedef struct PXLibrary_
{
    PXHandleModule ID;
    PXProcessHandle ProcessHandle;
}
PXLibrary;

PXPublic PXActionResult PXAPI PXLibraryOpen(PXLibrary* const pxLibrary, const PXText* const filePath); //  gain access to an executable object file. RTLD_LAZY
PXPublic PXActionResult PXAPI PXLibraryOpenA(PXLibrary* const pxLibrary, const char* const filePath);
PXPublic PXActionResult PXAPI PXLibraryOpenW(PXLibrary* const pxLibrary, const wchar_t* const filePath);
PXPublic PXActionResult PXAPI PXLibraryClose(PXLibrary* const pxLibrary); // close a dlopen object

PXPublic PXBool PXAPI PXLibraryGetSymbolBinding(PXLibrary* const pxLibrary, void** const bindingObject, const char* const symbolList, const PXSize amount, const PXBool areAllImportant);
PXPublic PXBool PXAPI PXLibraryGetSymbolListA(PXLibrary* const pxLibrary, PXLibraryFuntionEntry* const pxLibraryFuntionEntryList, const PXSize amount);
PXPublic PXBool PXAPI PXLibraryGetSymbolA(PXLibrary* const pxLibrary, void** const libraryFunction, const char* const symbolName, const PXBool isImportant);
PXPublic PXBool PXAPI PXLibraryGetSymbol(PXLibrary* const pxLibrary, void** const libraryFunction, const PXText* symbolName); // obtain the address of a symbol from a dlopen object

PXPublic PXActionResult PXAPI PXLibraryCurrentlyLoaded(PXProcessHandle pxProcessHandle, PXLibrary** const pxLibraryList, PXSize* const amount);

#endif
