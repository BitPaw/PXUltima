#ifndef PXLibraryINCLUDE
#define PXLibraryINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXLibraryUSE OSDeviceToUse == OSDeviceDestop
#if PXLibraryUSE

#include <Media/PXResource.h>

#if OSUnix
#include <sys/types.h>
#include <dlfcn.h>

typedef void* LibraryFunction;
typedef void* LibraryDirectoryID;

#elif OSWindows
#include <windows.h>
#include <DbgHelp.h>

//typedef FARPROC LibraryFunction;

typedef int (PXAPI* LibraryFunction)() ;

//typedef DLL_DIRECTORY_COOKIE LibraryDirectoryID;
#endif

#include <Media/PXText.h>
#include <OS/Async/PXProcess.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum PXSymbolType_
    {
        PXSymbolTypeNull,
        PXSymbolTypeExe,
        PXSymbolTypeCompiland,
        PXSymbolTypeCompilandDetails,
        PXSymbolTypeCompilandEnv,
        PXSymbolTypeFunction,
        PXSymbolTypeBlock,
        PXSymbolTypeData,
        PXSymbolTypeAnnotation,
        PXSymbolTypeLabel,
        PXSymbolTypePublicSymbol,
        PXSymbolTypeUDT,
        PXSymbolTypeEnum,
        PXSymbolTypeFunctionType,
        PXSymbolTypePointerType,
        PXSymbolTypeArrayType,
        PXSymbolTypeBaseType,
        PXSymbolTypeTypedef,
        PXSymbolTypeBaseClass,
        PXSymbolTypeFriend,
        PXSymbolTypeFunctionArgType,
        PXSymbolTypeFuncDebugStart,
        PXSymbolTypeFuncDebugEnd,
        PXSymbolTypeUsingNamespace,
        PXSymbolTypeVTableShape,
        PXSymbolTypeVTable,
        PXSymbolTypeCustom,
        PXSymbolTypeThunk,
        PXSymbolTypeCustomType,
        PXSymbolTypeManagedType,
        PXSymbolTypeDimension
    }
    PXSymbolType;

    typedef struct PXSymbol_
    {
        PXSize TypeIndex;        // Type Index of symbol
        PXSize Index;
        PXSize Size;
        PXSize ModBase;          // Base Address of module comtaining this symbol
        PXSize Flags;
        PXSize Address;          // Address of symbol including base address of module
        PXSize Register;         // register holding value or pointer to value
        PXSize Scope;            // scope of the symbol

        PXSymbolType Type;// PDB classification
        PXText Name;
    } 
    PXSymbol;



    typedef void (PXAPI*PXSymbolDetectedEvent)(const PXSymbol* const pxSymbol);





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

    PXPublic PXBool PXAPI PXLibraryGetSymbolA(PXLibrary* const pxLibrary, LibraryFunction* const libraryFunction, const char* const symbolName);
    PXPublic PXBool PXAPI PXLibraryGetSymbol(PXLibrary* const pxLibrary, LibraryFunction* const libraryFunction, const PXText* symbolName); // obtain the address of a symbol from a dlopen object

    PXPublic PXActionResult PXAPI PXLibraryName(PXLibrary* const pxLibrary, PXText* const libraryName);

#if OSUnix
#elif PXOSWindowsDestop
    PXPrivate BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);

   // PXPrivate BOOL CALLBACK PXLibraryNameSymbolEnumerate(PCSTR SymbolName, DWORD64 SymbolAddress, ULONG SymbolSize, PVOID UserContext);
#endif

    PXPublic PXActionResult PXAPI PXLibraryParseSymbols(const PXText* const libraryFilePath, PXSymbolDetectedEvent pxSymbolDetectedEvent);

    // void SymbolVector(); // PXProgramming interface to dynamic linking loader.

     //static ErrorCode SearchDirectoryAdd(const wchar_t* directoryPath, LibraryDirectoryID& libraryDirectoryID);
     //static ErrorCode SearchDirectoryRemove(LibraryDirectoryID& libraryDirectoryID);

#ifdef __cplusplus
}
#endif

#endif
#endif
