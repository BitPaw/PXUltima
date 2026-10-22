#pragma once

// This file is a wrapper for the entiretry of the API to the kernel
// from the OS itself and more.
// It acts as a singleton to prevent common repetative boilerplate code 
// to keep the actual code space free of silly coding choices of the past.

#ifndef PXOSIncluded
#define PXOSIncluded

#include <PX/OS/Library/PXLibrary.h>
#include <PX/OS/Async/PXThread.h>
#include "Async/PXLock.h"
#include <PX/Reflection/PXReflection.h>
#include "Signal/PXSignal.h"

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



    PXHandleModule ModuleHandle; // OS depended handle to the module

   

    PXSize LineNumber;
    PXSize Amount;
    PXSize ObjectSize;


    void* AddressModule; // EXE / DLL / SO containing this symbol 
    void* AddressSymbol; // Function or class/struct
    PXSize Offset; // Offset of symbol to its address. Example: Member of class.

    char NameUndecorated[64];
    char NameSymbol[64];        // MyFunction()
    char NameFile[64];          // MyCode.c
    char NameModule[256];        // MyLibrary.dll / MyLibrary.so
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

    PXThreadID MainThreadID;
}
PXOS;


#define PXMemoryPageNone        0
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

PXPublic PXResult PXAPI PXSystemVersionGetViaRegistry(PXOSVersion PXREF pxOSVersion);
PXPublic PXResult PXAPI PXSystemVersionGetViaKernel(PXOSVersion PXREF pxOSVersion);

PXPublic void PXAPI PXSystemVersionGet(PXOSVersion PXREF pxOSVersion);
//---------------------------------------------------------



//---------------------------------------------------------
// General utility??
//---------------------------------------------------------
PXPublic PXI8U PXAPI PXBitsCount(const PXSize bitMask);



//---------------------------------------------------------
// User
//---------------------------------------------------------
PXPublic PXResult PXAPI PXUserNameGet(PXText PXREF pxText);
PXPublic PXResult PXAPI PXComputerNameGet(char PXREF text, const PXSize textSizeMax, PXSize PXREF textSizeWritten);
//---------------------------------------------------------



// Cleanse the filepath from symbols like "\.", "\.." and "/"
PXPublic PXResult PXAPI PXFilePathCleanse(const char* pathInput, char PXREF pathOutput, const PXSize pathOutputSizeMAX, PXSize PXREF pathOutputSizeWritten);



//---------------------------------------------------------
// Text
//---------------------------------------------------------
PXPublic void PXAPI PXTextUTF8ToUNICODE(wchar_t PXREF textOutput, const char PXREF textInput);
PXPublic void PXAPI PXTextUNICODEToUTF8(char PXREF textOutput, const wchar_t PXREF textInput);




//---------------------------------------------------------
// Process and threads
//---------------------------------------------------------

PXPublic PXResult PXAPI PXProcessMemoryWrite
(
    const PXProcessHandle pxProcessHandle,
    void* baseAddress,
    const void* bufferData,
    const PXSize bufferSizeMax,
    PXSize PXREF bufferSizeWritten
);

PXPublic PXResult PXAPI PXProcessMemoryRead
(
    const PXProcessHandle pxProcessHandle,
    const void* baseAddress,
    void PXREF bufferData,
    const PXSize bufferSizeMax,
    PXSize PXREF bufferSizeWritten
);

PXPublic PXResult PXAPI PXThreadCPUCoreAffinitySet(PXThread PXREF pxThread, const PXI16U coreIndex);


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

PXPublic void PXAPI PXMemoryHeapGetGlobal(PXMemoryHeap PXREF pxMemoryHeap);
PXPublic PXResult PXAPI PXMemoryHeapCreate(PXMemoryHeap PXREF pxMemoryHeap);
PXPublic PXResult PXAPI PXMemoryHeapRelease(PXMemoryHeap PXREF pxMemoryHeap);

// Get the size of the allocated block
PXPublic PXSize PXAPI PXMemoryHeapBlockSize(PXMemoryHeap* pxMemoryHeap, const void PXREF address);

PXPublic void* PXAPI PXMemoryHeapCalloc(PXMemoryHeap* pxMemoryHeap, const PXSize amount, const PXSize objectSize); // [POSIX]
PXPublic void* PXAPI PXMemoryHeapMalloc(PXMemoryHeap* pxMemoryHeap, const PXSize memorySize); // [POSIX]
PXPublic PXBool PXAPI PXMemoryHeapFree(PXMemoryHeap* pxMemoryHeap, const void PXREF address); // Windows:HeapFree
PXPublic void* PXAPI PXMemoryHeapRealloc(PXMemoryHeap* pxMemoryHeap, const void PXREF address, const PXSize memorySize); // Windows:HeapReAlloc


#define PXMemoryHeapCallocT(type, amount) (type*)PXMemoryHeapCalloc(PXNull, amount, sizeof(type))
#define PXMemoryHeapMallocT(type, amount) (type*)PXMemoryHeapMalloc(PXNull, sizeof(type) * amount)
#define PXMemoryHeapReallocT(type, address, amount) (type*)PXMemoryHeapRealloc(PXNull, address, sizeof(type) * (amount))
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
PXPublic void* PXAPI PXMemoryVirtualAllocate(const PXSize size, PXSize PXREF createdSize, const PXAccessMode pxAccessMode);
PXPrivate void PXAPI PXMemoryVirtualAllocateNormal(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo);
PXPrivate void PXAPI PXMemoryVirtualAllocatePhysical(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo);
PXPrivate void PXAPI PXMemoryVirtualAllocateLarge(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo);
PXPrivate void PXAPI PXMemoryVirtualAllocateHuge(PXMemoryPageAllocInfo PXREF pxMemoryPageAllocInfo);


PXPublic void PXAPI PXMemoryVirtualPrefetch(const void* address, const PXSize size);
PXPublic PXResult PXAPI PXMemoryVirtualRelease(const void* address, const PXSize size);
PXPublic void* PXAPI PXMemoryVirtualReallocate(const void* address, const PXSize size);
PXPublic PXResult PXAPI PXMemoryVirtualInfoViaAddress(PXMemoryVirtualInfo PXREF pxMemoryVirtualInfo, const void* address);
//---------------------------------------------------------


//---------------------------------------------------------
// On fault
//---------------------------------------------------------

#define PXStackFrameAmountMax 16


PXPublic void PXAPI PXDebugStackTrace(void);
PXPublic void PXAPI PXDebugStackTraceWithSignal(PXSignalInfo PXREF pxSignalInfo);
//---------------------------------------------------------



//---------------------------------------------------------
// Debug - Proceess
//---------------------------------------------------------
PXPublic PXResult PXAPI PXDebugEventContinue(const PXI32U processID, const PXI32U threadID);
PXPublic PXResult PXAPI PXDebugEventWait(void* pxDebugEventInfo, const PXI32U time);
PXPublic PXResult PXAPI PXDebugProcessActive(void);
PXPublic PXResult PXAPI PXDebugBreak(void);
PXPublic PXResult PXAPI PXDebugOutputDebugStringA(const char* text);
PXPublic PXResult PXAPI PXDebugOutputDebugStringW(const wchar_t* text);
PXPublic PXResult PXAPI PXDebugProcessBreak(void);
PXPublic PXResult PXAPI PXDebugIsPresent(PXBool PXREF isPresent);
PXPublic PXResult PXAPI PXDebugIsPresentRemote(const PXProcessHandle processHandle, PXBool PXREF isPresent);
PXPublic PXResult PXAPI PXDebugActiveProcessStop(const PXI32U processID);
//---------------------------------------------------------


//---------------------------------------------------------
// Debug - Symbol
//---------------------------------------------------------
PXPublic PXResult PXAPI PXSymbolServerInitialize(void); // SymInitialize
PXPublic PXResult PXAPI PXSymbolServerCleanup(void);
PXPublic PXResult PXAPI PXSymbolServerOptionsSet(void);
PXPublic PXResult PXAPI PXSymbolListLoad(const PXProcessHandle processHandle, const void* baseAddress);
PXPublic PXResult PXAPI PXSymbolModuleLoad(const PXProcessHandle processHandle, const char* moduleName, void** baseAddress); // SymLoadModuleEx
PXPublic PXResult PXAPI PXSymbolStackWalk(PXSymbolStackWalkInfo PXREF pxSymbolStackWalkInfo);
PXPublic PXResult PXAPI PXSymbolStackTrace(PXSymbol PXREF pxSymbolList, const PXSize pxSymbolListAmount);
PXPublic PXResult PXAPI PXSymbolCaller(PXSymbol PXREF pxSymbol);
PXPublic PXResult PXAPI PXSymbolUnDecorateName(const char* inputName, char* name, const PXSize nameLengthMax, PXSize* nameLengthWritten); // UnDecorateSymbolName
PXPublic PXResult PXAPI PXSymbolFromAddress(PXSymbol PXREF pxSymbol, const void PXREF address);
//---------------------------------------------------------

PXPublic PXResult PXAPI PXModuleCurrentPath(PXText PXREF pxText);
PXPublic PXResult PXAPI PXModuleCurrentName(PXText PXREF pxText);

PXPublic PXBool PXAPI PXModuleInSameDirectory(PXText PXREF pxTextA, PXText PXREF pxTextB);


//---------------------------------------------------------
// File - I/O
//---------------------------------------------------------

//---------------------------------------------------------


typedef struct PXPageFaultInfo_
{
    void* ProgramCounter; // Instruction that caused the page fault.
    void* VirtualAddress; // Page that was added to the working set.
}
PXPageFaultInfo;


// Perfromance
typedef struct PXPerformanceInfo_
{
    PXTimeStamp TimeStamp;
    PXF64 TimeDelta;

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
PXPublic PXResult PXAPI PXPerformanceInfoGet(PXPerformanceInfo PXREF pxPerformanceInfo);

PXPublic void PXAPI PXConvertAnyToFloat(void* a, const PXI32U aType, float* b);

#endif