#pragma once

#ifndef PXLibraryIncluded
#define PXLibraryIncluded

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

typedef struct PXLibrary_ PXLibrary;

typedef struct PXLibrary_
{
#if OSUnix
    void* ID;
#elif OSWindows
    HMODULE ID; // HINSTANCE(semms is also okey)
#endif

    PXProcessHandle ProcessHandle;
}
PXLibrary;

PXPublic PXResult PXAPI PXLibraryOpen(PXLibrary PXREF pxLibrary, const PXText PXREF filePath); //  gain access to an executable object file. RTLD_LAZY
PXPublic PXResult PXAPI PXLibraryOpenA(PXLibrary PXREF pxLibrary, const char PXREF filePath);
PXPublic PXResult PXAPI PXLibraryOpenW(PXLibrary PXREF pxLibrary, const wchar_t PXREF filePath);
PXPublic PXResult PXAPI PXLibraryClose(PXLibrary PXREF pxLibrary); // close a dlopen object

PXPublic PXBool PXAPI PXLibraryGetSymbolBinding(PXLibrary PXREF pxLibrary, void* PXREF bindingObject, const char PXREF symbolList, const PXSize amount, const PXBool areAllImportant);
PXPublic PXBool PXAPI PXLibraryGetSymbolListA(PXLibrary PXREF pxLibrary, PXLibraryFuntionEntry PXREF pxLibraryFuntionEntryList, const PXSize amount);
PXPublic PXBool PXAPI PXLibraryGetSymbolA(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const char PXREF symbolName, const PXBool isImportant);
PXPublic PXBool PXAPI PXLibraryGetSymbol(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const PXText* symbolName); // obtain the address of a symbol from a dlopen object

PXPublic PXResult PXAPI PXLibraryCurrentlyLoaded(PXProcessHandle pxProcessHandle, PXLibrary* PXREF pxLibraryList, PXSize PXREF amount);

#endif
