#pragma once

#ifndef PXLibraryIncluded
#define PXLibraryIncluded

#include <PX/Type/PXText.h>
#include <PX/OS/Async/PXProcess.h>

// Handle to the module used for library IDs
typedef struct PXHandleModule_
{
#if OSUnix
    void* Data;
#elif OSWindows
    // Note: Windows calls DLL files modules.
    HMODULE Data;// HINSTANCE(semms is also okey)
#else
    void* Data;
#endif
}
PXHandleModule;

typedef struct PXLibraryFunctionEntry_
{
    void** Function;
    const char* FunctionName;
}
PXLibraryFunctionEntry;

typedef struct PXLibrary_
{
    PXHandleModule Handle;
    PXProcessHandle ProcessHandle;
}
PXLibrary;

PXPublic PXResult PXAPI PXLibraryOpen(PXLibrary PXREF pxLibrary, const PXText PXREF filePath);
PXPublic PXResult PXAPI PXLibraryClose(PXLibrary PXREF pxLibrary);

PXPublic PXResult PXAPI PXLibraryGetSymbolBinding(PXLibrary PXREF pxLibrary, void* PXREF bindingObject, const char PXREF symbolList, const PXSize amount, const PXBool areAllImportant);
PXPublic PXResult PXAPI PXLibraryGetSymbolListA(PXLibrary PXREF pxLibrary, PXLibraryFunctionEntry PXREF pxLibraryFunctionEntry, const PXSize amount);
PXPublic PXResult PXAPI PXLibraryGetSymbolA(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const char PXREF symbolName, const PXBool isImportant);
PXPublic PXResult PXAPI PXLibraryGetSymbol(PXLibrary PXREF pxLibrary, void* PXREF libraryFunction, const PXText* symbolName); // obtain the address of a symbol from a dlopen object

PXPublic PXResult PXAPI PXLibraryCurrentlyLoaded(PXProcessHandle pxProcessHandle, PXLibrary* PXREF pxLibraryList, PXSize PXREF amount);


#define PXDebugModuleNameMask   0b11
#define PXDebugModuleNameFull   0b00
#define PXDebugModuleNameShort  0b01


PXPublic PXResult PXAPI PXLibraryFromAddress(PXLibrary PXREF pxLibrary, const void PXREF address);
PXPublic PXResult PXAPI PXLibraryName(PXLibrary PXREF pxLibrary, PXText PXREF pxTextLibraryName);

// Lib can be NULL, means we dont need the lib result
PXPublic PXResult PXAPI PXLibraryNameFromAddress
(
    PXLibrary* pxLibrary,
    PXText PXREF pxTextLibraryName,
    const void PXREF address
);

#endif
