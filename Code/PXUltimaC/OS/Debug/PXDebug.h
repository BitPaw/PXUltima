#ifndef PXDebugINLCUDE
#define PXDebugINLCUDE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>
#include <OS/Error/PXActionResult.h>
#include <OS/GUI/PXGUI.h>
#include <Media/PXResource.h>

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
    PXSize Flags;
    PXSize Address;          // Address of symbol including base address of module
    PXSize Register;         // register holding value or pointer to value
    PXSize Scope;            // scope of the symbol

    PXSymbolType Type;// PDB classification

    void* ModuleAdress; // Base Address of module comtaining this symbol

    PXHandleModule ModuleHandle; // OS depended handle to the module

    void* SymbolAdress;

    PXSize LineNumber;
    PXSize Amount;
    PXSize ObjectSize;

    char NameUndecorated[64];
    char NameSymbol[64];        // MyFunction()
    char NameFile[64];          // MyCode.c
    char NameModule[64];        // MyLibrary.dll / MyLibrary.so
}
PXSymbol;

typedef struct PXSymbolEnumerator_
{
    PXSymbol* SymbolList;
    PXSize Amount;
    PXSize Size;
}
PXSymbolEnumerator;

typedef struct PXDebug_
{
    PXLibrary LibraryKernel;
    PXLibrary LibraryDebugHelp;

    PXBool IsRunning;

    PXProcess Process;

    PXThread EventListenLoop;

    PXByte ApplicatioNameBuffer[260];
    PXText ApplicatioName;

#if OSWindows
    // Kernel
    void* XContinueDebugEvent;
    void* XWaitForDebugEvent;
    void* XDebugActiveProcess;
    void* XDebugBreak;
    void* WOutputDebugStringA;
    void* WOutputDebugStringW;
    void* XDebugBreakProcess;
    void* XIsDebuggerPresent;
    void* XCheckRemoteDebuggerPresent;
    void* XDebugActiveProcessStop;

    // Debug
    void* SymbolServerInitialize;
    void* SymbolServerCleanup;
    void* SymbolModuleLoad;
    void* DBGStackWalk;
    void* XUnDecorateSymbolName;
    void* SymbolFromAddress;
    void* SymbolEnumerate;
    void* SymbolEnumerateEx;
    void* SymbolFunctionTableAccess;
    void* SymbolGetModuleBase;
#endif
}
PXDebug;











PXPublic PXDebug* PXAPI PXDebugInstanceGet();
PXPublic void PXAPI PXDebugInstanceRelease(PXDebug* const pxDebug);


// VS Debugger functions-- maybe
PXPublic PXActionResult PXAPI PXDebugProcessBeeingDebugged(PXDebug* const pxDebug, PXBool* const isPresent);
PXPublic PXBool PXAPI PXDebugProcessBeeingDebuggedCurrent(PXDebug* const pxDebug);

PXPublic void PXAPI PXDebugDebuggerSendMessage(PXDebug* const pxDebug, PXText* const message);


PXPublic PXActionResult PXAPI PXDebugDebuggerInitialize(PXDebug* const pxDebug);


PXPublic PXActionResult PXAPI PXDebugStartProcess(PXDebug* const pxDebug, const PXText* const applicationName);

// Enables a debugger to continue a thread that previously reported a debugging event.
PXPublic PXActionResult PXAPI PXDebugContinue(PXDebug* const pxDebug);
PXPublic PXActionResult PXAPI PXDebugPause(PXDebug* const pxDebug);
PXPublic PXBool PXAPI PXDebugPauseOther(PXDebug* const pxDebug, const PXProcessHandle pxProcessHandle);
PXPublic PXActionResult PXAPI PXDebugAttach(PXDebug* const pxDebug);
//PXPublic void PXDebugAttachToDebugger(PXDebug* const pxDebug);
PXPublic PXActionResult PXAPI PXDebugDetach(PXDebug* const pxDebug);

PXPublic void PXAPI PXDebugStackTrace(PXDebug* const pxDebug, PXSymbol* const pxSymbolList, const PXSize pxSymbolListAmount, const PXSize start, const PXSize depth);

PXPublic PXSize PXAPI PXDebugMemoryRead(PXDebug* const pxDebug, const void* const adress, void* const outputData, const PXSize outputDataSize);
PXPublic PXSize PXAPI PXDebugMemoryWrite(PXDebug* const pxDebug, const void* const adress, const void* const inputData, const PXSize inputDataSize);

// Only the thread that created the process being debugged can call this function.
PXPublic PXActionResult PXAPI PXDebugWaitForEvent(PXDebug* const pxDebug);


PXPublic PXActionResult PXAPI PXDebugLibrarySymbolsFetch(PXDebug* const pxDebug, const PXText* const libraryFilePath, PXSymbol* const pxSymbolList, PXSize* const amount);

PXPublic void PXAPI PXDebugLogMessage(PXText* const pxText);


PXPublic PXActionResult PXAPI PXDebugSymbolReadFromAdress(PXDebug* const pxDebug, struct PXSymbol_* const pxSymbol, void* adress);

PXPublic PXActionResult PXAPI PXDebugHeapMemoryList(PXDebug* const pxDebug);

PXPublic PXActionResult PXAPI PXDebugFetchSymbolThread(PXDebug* const pxDebug, PXSymbol* const pxSymbol, PXThread* pxThread);
PXPublic PXActionResult PXAPI PXDebugFetchSymbolFromRougeAdress(PXDebug* const pxDebug, PXSymbol* const pxSymbol, void* adress);

PXPrivate PXThreadResult PXAPI PXDebugLoop(PXDebug* const pxDebug);

PXPublic PXActionResult PXAPI PXDebugDumpCreate(PXDebug* const pxDebug);


// Adress to module HANDLE
PXPublic PXActionResult PXAPI PXDebugModuleHandleFromAdress(PXHandleModule* const pxHandleModule, const void* const adress);



#define PXDebugModuleNameMask   0b11
#define PXDebugModuleNameFull   0b00
#define PXDebugModuleNameShort  0b01


// Retrieves the name from a module handle
PXPublic PXActionResult PXAPI PXDebugModuleNameGet
(
    const PXHandleModule pxHandleModule, 
    char* moduleName,
    const PXSize moduleNameSize, 
    PXSize* const sizeWritten,
    const PXInt32U flags
);

#endif
