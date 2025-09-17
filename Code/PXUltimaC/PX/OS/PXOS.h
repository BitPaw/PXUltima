// This file is a wrapper for the entiretry of the API to the kernel
// from the OS itself and more.
// It acts as a singleton to prevent common repetative boilerplate code 
// to keep the actual code space free of silly coding choices of the past.

#ifndef PXOSIncluded
#define PXOSIncluded

#include <PX/Engine/PXResource.h>
#include "Library/PXLibrary.h"
#include <PX/OS/Async/PXThread.h>

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

typedef struct PXSymbolStackWalkInfo_ PXSymbolStackWalkInfo;

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






typedef struct PXOS_
{
    PXLibrary LibraryNT;
    PXLibrary LibraryKernel;
    PXLibrary LibraryDebugHelp;

#if OSWindows
    // NT
    void* RtlGetVersion;



    // Kernel
    void* DebugEventContinue;
    void* DebugEventWait;
    void* DebugProcessActive;
    void* DebugBreak;
    void* DebugOutputDebugStringA;
    void* DebugOutputDebugStringW;
    void* DebugProcessBreak;
    void* DebugIsDebuggerPresent;
    void* DebugRemotePresentCheck;
    void* DebugActiveProcessStop;

    // Debug
    void* SymbolServerInitialize;
    void* SymbolServerCleanup;
    void* SymbolModuleLoad;
    void* SymbolStackWalk;
    void* SymbolUnDecorateName;
    void* SymbolFromAddress;
    void* SymbolEnumerate;
    void* SymbolEnumerateEx;
    void* SymbolFunctionTableAccess;
    void* SymbolModuleBaseGet;
#endif

    void* ApplicationAddressMinimum;
    void* ApplicationAddressMaximum;
    PXI64U ProcessorActiveMask;
    PXI32U ProcessorAmountPhysical;
    PXI32U ProcessorAmountLogical;

    PXI32U ProcessorAmountNUMA;


    PXI32U ProcessorCacheL1Size;
    PXI32U ProcessorCacheL2Size;
    PXI32U ProcessorCacheL3Size;

    PXSize CacheLineSize;
    PXSize PageSizeNormal;      // 2^12          4.096   4 KB (can be 2KB with PAE)
    PXSize PageSizePhysical;    // 2^16         65.536  64 KB
    PXSize PageSizeLarge;       // 2^21      2.097.152   2 MB
    PXSize PageSizeHuge;        // 2^30  1.073.741.824   1 GB
  

    PXI32U dwAllocationGranularity;

    PXBool Init;

    PXThread MainThread;
}
PXOS;


#define PXMemoryPageNormal      1
#define PXMemoryPagePhysical    2
#define PXMemoryPageLarge       3
#define PXMemoryPageHuge        4


//---------------------------------------------------------
//  Init
//---------------------------------------------------------
PXPublic PXResult PXAPI PXSystemPrelude();
PXPublic PXOS* PXAPI PXSystemGet();


typedef struct PXOSVersion_
{
    char NameVersion[16];
    char NameProduct[64]; // Windows 10 Enterprise

    PXI8U NameVersionLength;
    PXI8U NameProductLength;

    PXI16U Major;
    PXI16U Minor;
    PXI16U Build;
    PXI16U Patch;
}
PXOSVersion;

PXPublic PXResult PXAPI PXSystemVersionGetViaRegistry(PXOSVersion* const pxOSVersion);
PXPublic PXResult PXAPI PXSystemVersionGetViaKernel(PXOSVersion* const pxOSVersion);

PXPublic void PXAPI PXSystemVersionGet(PXOSVersion* const pxOSVersion);
//---------------------------------------------------------



//---------------------------------------------------------
// User
//---------------------------------------------------------
PXPublic PXResult PXAPI PXUserNameGet(char* const text, const PXSize textSizeMax, PXSize* const textSizeWritten);
PXPublic PXResult PXAPI PXComputerNameGet(char* const text, const PXSize textSizeMax, PXSize* const textSizeWritten);
//---------------------------------------------------------



// Cleanse the filepath from symbols like "\.", "\.." and "/"
PXPublic PXResult PXAPI PXFilePathCleanse(const char* pathInput, char* const pathOutput, const PXSize pathOutputSizeMAX, PXSize* const pathOutputSizeWritten);
PXPublic PXResult PXAPI PXFileNameViaHandleA(PXFile* const pxFile, char* const fileNameBuffer, const PXSize pathOutputSizeMAX, PXSize* const pathOutputSizeWritten);


//---------------------------------------------------------
// Text
//---------------------------------------------------------
PXPublic void PXAPI PXTextUTF8ToUNICODE(wchar_t* const textOutput, const char* const textInput);
PXPublic void PXAPI PXTextUNICODEToUTF8(char* const textOutput, const wchar_t* const textInput);




//---------------------------------------------------------
// Process and threads
//---------------------------------------------------------
PXPublic void PXAPI PXProcessCurrent(PXProcess* const pxProcess);


PXPublic PXResult PXAPI PXProcessMemoryWrite
(
    const PXProcessHandle pxProcessHandle,
    const void* baseAddress,
    const void* bufferData,
    const PXSize bufferSizeMax,
    PXSize* const bufferSizeWritten
);

PXPublic PXResult PXAPI PXProcessMemoryRead
(
    const PXProcessHandle pxProcessHandle,
    const void* baseAddress,
    void* const bufferData,
    const PXSize bufferSizeMax,
    PXSize* const bufferSizeWritten
);


PXPublic PXBool PXAPI PXThreadIsMain();
PXPublic PXResult PXAPI PXThreadCurrent(PXThread* const pxThread);
PXPublic PXResult PXAPI PXThreadResume(PXThread* const pxThread);
PXPublic PXResult PXAPI PXThreadSuspend(PXThread* const pxThread);
PXPublic PXResult PXAPI PXThreadWait(PXThread* const pxThread);

PXPublic PXResult PXAPI PXThreadCPUCoreAffinitySet(PXThread* const pxThread, const PXI16U coreIndex);


// Causes the calling thread to yield execution to another
// thread that is ready to run on the current processor.
// The operating system selects the next thread to be executed.
// The function returns true if a yield was caused, otherwise the
// current thread proceeds execution and false is returned.
PXPublic PXResult PXAPI PXThreadYieldToOtherThreads();
//---------------------------------------------------------


//---------------------------------------------------------
// Memory - Heap - Space to allocate dynamic memory
//---------------------------------------------------------
typedef struct PXMemoryHeap_
{
    void* HeapHandle;
    PXSize Allocated;
}
PXMemoryHeap;

PXPublic void PXAPI PXMemoryHeapGetGlobal(PXMemoryHeap* const pxMemoryHeap);
PXPublic PXResult PXAPI PXMemoryHeapCreate(PXMemoryHeap* const pxMemoryHeap);
PXPublic PXResult PXAPI PXMemoryHeapRelease(PXMemoryHeap* const pxMemoryHeap);

// Get the size of the allocated block
PXPublic PXSize PXAPI PXMemoryHeapBlockSize(PXMemoryHeap* pxMemoryHeap, const void* const adress);

PXPublic void* PXAPI PXMemoryHeapCalloc(PXMemoryHeap* pxMemoryHeap, const PXSize amount, const PXSize objectSize); // [POSIX]
PXPublic void* PXAPI PXMemoryHeapMalloc(PXMemoryHeap* pxMemoryHeap, const PXSize memorySize); // [POSIX]
PXPublic PXBool PXAPI PXMemoryHeapFree(PXMemoryHeap* pxMemoryHeap, const void* const adress); // Windows:HeapFree
PXPublic void* PXAPI PXMemoryHeapRealloc(PXMemoryHeap* pxMemoryHeap, const void* const adress, const PXSize memorySize); // Windows:HeapReAlloc


#define PXMemoryHeapCallocT(type, amount) (type*)PXMemoryHeapCalloc(PXNull, amount, sizeof(type))
#define PXMemoryHeapMallocT(type, amount) (type*)PXMemoryHeapMalloc(PXNull, sizeof(type) * amount)
#define PXMemoryHeapReallocT(type, adress, amount) (type*)PXMemoryHeapRealloc(PXNull, adress, sizeof(type) * (amount))
//---------------------------------------------------------





//---------------------------------------------------------
// Memory - Virtual
//---------------------------------------------------------
#define PXPAGE_NoAccess                 0
#define PXPAGE_Rxx                      1
#define PXPAGE_RWx                      2
#define PXPAGE_xWx                      3
#define PXPAGE_xxE                      4
#define PXPAGE_RxE                      5
#define PXPAGE_RWE                      6
#define PXPAGE_EXECUTE_WRITECOPY        7
#define PXPAGE_TARGETS_INVALID          8
#define PXPAGE_TARGETS_NO_UPDATE        9
#define PXPAGE_GUARD                    10
#define PXPAGE_NOCACHE                  11
#define PXPAGE_WRITECOMBINE             12
#define PXPAGE_ENCLAVE_DECOMMIT         13
#define PXPAGE_ENCLAVE_THREAD_CONTROL   14
#define PXPAGE_ENCLAVE_UNVALIDATED      15

#define PXPageStateFree     
#define PXPageStateReserved
#define PXPageStateCommitted

#define PXPageTypeImage    
#define PXPageTypeMapped
#define PXPageTypePrivate

typedef struct PXMemoryVirtualInfo_
{
    void* BaseAddress;
    void* AllocationBase;
    PXSize RegionSize;    
    PXI32U ProtectionAtCreation;
    PXI32U ProtectionCurrent;
    PXI32U State;
    PXI32U Type;
    PXI16U PartitionID;
}
PXMemoryVirtualInfo;


// Allocate memory in virtual memory space.
// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
// Only use for bigger datablocks as thic has very hi overhead.
PXPublic void* PXAPI PXMemoryVirtualAllocate(PXSize size, PXSize* const createdSize, const PXAccessMode pxAccessMode);
PXPublic void PXAPI PXMemoryVirtualPrefetch(const void* adress, const PXSize size);
PXPublic PXResult PXAPI PXMemoryVirtualRelease(const void* adress, const PXSize size);
PXPublic void* PXAPI PXMemoryVirtualReallocate(const void* adress, const PXSize size);
PXPublic PXResult PXAPI PXMemoryVirtualInfoViaAdress(PXMemoryVirtualInfo* const pxMemoryVirtualInfo, const void* adress);
//---------------------------------------------------------



//---------------------------------------------------------
// Debug - Proceess
//---------------------------------------------------------
PXPublic PXResult PXAPI PXDebugEventContinue(const PXI32U processID, const PXI32U threadID);
PXPublic PXResult PXAPI PXDebugEventWait(void* pxDebugEventInfo, const PXI32U time);
PXPublic PXResult PXAPI PXDebugProcessActive();
PXPublic PXResult PXAPI PXDebugBreak();
PXPublic PXResult PXAPI PXDebugOutputDebugStringA(const char* text);
PXPublic PXResult PXAPI PXDebugOutputDebugStringW(const wchar_t* text);
PXPublic PXResult PXAPI PXDebugProcessBreak();
PXPublic PXResult PXAPI PXDebugIsPresent(PXBool* const isPresent);
PXPublic PXResult PXAPI PXDebugIsPresentRemote(const PXProcessHandle processHandle, PXBool* const isPresent);
PXPublic PXResult PXAPI PXDebugActiveProcessStop(const PXI32U processID);



#define PXDebugModuleNameMask   0b11
#define PXDebugModuleNameFull   0b00
#define PXDebugModuleNameShort  0b01

// Retrieves the name from a module handle
PXPublic PXResult PXAPI PXDebugModuleNameGet
(
    const PXHandleModule pxHandleModule,
    char* moduleName,
    const PXSize moduleNameSize,
    PXSize* const sizeWritten,
    const PXI32U flags
);


//---------------------------------------------------------


//---------------------------------------------------------
// Debug - Symbol
//---------------------------------------------------------
PXPublic PXResult PXAPI PXSymbolServerInitialize(); // SymInitialize
PXPublic PXResult PXAPI PXSymbolServerCleanup();
PXPublic PXResult PXAPI PXSymbolServerOptionsSet();
PXPublic PXResult PXAPI PXSymbolListLoad(const PXProcessHandle processHandle, const void* baseAdress);
PXPublic PXResult PXAPI PXSymbolModuleLoad(const PXProcessHandle processHandle, const char* moduleName, void** baseAdress); // SymLoadModuleEx
PXPublic PXResult PXAPI PXSymbolStackWalk(PXSymbolStackWalkInfo* const pxSymbolStackWalkInfo);
PXPublic PXResult PXAPI PXSymbolStackTrace(PXSymbol* const pxSymbolList, const PXSize pxSymbolListAmount, const PXSize start, const PXSize depth);
PXPublic PXResult PXAPI PXSymbolUnDecorateName(const char* inputName, char* name, const PXSize nameLengthMax, PXSize* nameLengthWritten); // UnDecorateSymbolName
PXPublic PXResult PXAPI PXSymbolFromAddress(PXSymbol* const pxSymbol, const void* const adress);
PXPublic PXResult PXAPI PXSymbolEnumerate();
PXPublic PXResult PXAPI PXSymbolFunctionTableAccess();
PXPublic PXResult PXAPI PXSymbolModuleBaseGet();
// Adress to module HANDLE
PXPublic PXResult PXAPI PXSymbolModuleHandleFromAdress(PXHandleModule* const pxHandleModule, const void* const adress);
PXPublic PXResult PXAPI PXSymbolModuleName(const PXHandleModule pxHandleModule, char* const name);
//---------------------------------------------------------




//---------------------------------------------------------
// Thread - 
//---------------------------------------------------------
typedef PXActionResult (PXAPI* PXLockFunction)(PXLock* const pxLock);

PXPublic PXResult PXAPI PXSemaphorCreate(PXLock* const pxLock);
PXPublic PXResult PXAPI PXSemaphorDelete(PXLock* const pxLock);
PXPublic PXResult PXAPI PXSemaphorEnter(PXLock* const pxLock);
PXPublic PXResult PXAPI PXSemaphorLeave(PXLock* const pxLock);

PXPublic PXResult PXAPI PXCriticalSectionCreate(PXLock* const pxLock);
PXPublic PXResult PXAPI PXCriticalSectionDelete(PXLock* const pxLock);
PXPublic PXResult PXAPI PXCriticalSectionEnter(PXLock* const pxLock);
PXPublic PXResult PXAPI PXCriticalSectionLeave(PXLock* const pxLock);
//---------------------------------------------------------




//---------------------------------------------------------
// File - I/O
//---------------------------------------------------------
PXPublic PXResult PXAPI PXFileMapToMemoryEE(PXFile* const pxFile, const PXSize requestedSize, const PXAccessMode pxAccessMode, const PXBool prefetch);

//---------------------------------------------------------


typedef struct PXPageFaultInfo_
{
    void* ProgramCounter; // Instruction that caused the page fault.
    void* VirtualAdress; // Page that was added to the working set.
}
PXPageFaultInfo;


// Perfromance
typedef struct PXPerformanceInfo_
{
    PXI64U TimeStamp;
    PXF32 TimeDelta;

    PXI32U UpdateCounter; // Times we updated this stucture. To update delta values.

    PXI32U PageFaultCount;

    PXSize PeakWorkingSetSize;
    PXSize WorkingSetSize;
    PXSize QuotaPeakPagedPoolUsage;
    PXSize QuotaPagedPoolUsage;
    PXSize QuotaPeakNonPagedPoolUsage;
    PXSize QuotaNonPagedPoolUsage;
    PXSize PagefileUsage;
    PXSize PeakPagefileUsage;
    PXSize PrivateUsage;
    PXSize PrivateWorkingSetSize;
    PXSize SharedCommitUsage;

    PXSize CommitTotal;
    PXSize CommitLimit;
    PXSize CommitPeak;
    PXSize PhysicalTotal;
    PXSize PhysicalAvailable;
    PXSize SystemCache;
    PXSize KernelTotal;
    PXSize KernelPaged;
    PXSize KernelNonpaged;
    PXSize PageSize;

    PXI32U HandleCount;
    PXI32U ProcessCount;
    PXI32U ThreadCount;

    PXPageFaultInfo* PageFaultInfoList;
    PXSize PageFaultInfoListAmount;
}
PXPerformanceInfo;

// When UpdateCounter is 0, we get the current data.
// Every call after will get the delta.
PXPublic PXResult PXAPI PXPerformanceInfoGet(PXPerformanceInfo* const pxPerformanceInfo);



//---------------------------------------------------------
// VIDEO
//---------------------------------------------------------
PXPublic PXResult PXAPI PXGPUList(PXGPUPhysical* const pxGPUPhysicalList, PXSize* const pxGPUPhysicalListSize);
//---------------------------------------------------------



#endif