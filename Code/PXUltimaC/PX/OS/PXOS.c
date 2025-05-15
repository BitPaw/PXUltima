#include "PXOS.h"

#include <Windows.h>
#include <DbgHelp.h>

#include "File/PXFile.h"

#include <PX/OS/Console/PXConsole.h>





typedef BOOL(WINAPI* PXDebugActiveProcessStopFunction)(_In_ DWORD dwProcessId);
typedef BOOL(WINAPI* PXContinueDebugEventFunction)(_In_ DWORD dwProcessId, _In_ DWORD dwThreadId, _In_ DWORD dwContinueStatus);
typedef BOOL(WINAPI* PXWaitForDebugEventFunction)(_Out_ LPDEBUG_EVENT lpDebugEvent, _In_ DWORD dwMilliseconds);
typedef BOOL(WINAPI* PXDebugActiveProcessFunction)(_In_ DWORD dwProcessId);
typedef VOID(WINAPI* PXDebugBreakFunction)(VOID);
typedef VOID(WINAPI* PXOutputDebugStringAFunction)(_In_opt_ LPCSTR lpOutputString);
typedef    VOID(WINAPI* PXOutputDebugStringWFunction)(_In_opt_ LPCWSTR lpOutputString);
typedef BOOL(WINAPI* PXDebugBreakProcessFunction)(_In_ HANDLE Process);
typedef BOOL(WINAPI* PXIsDebuggerPresentFunction)(VOID);
typedef BOOL(WINAPI* PXCheckRemoteDebuggerPresentFunction)(_In_ HANDLE hProcess, _Out_ PBOOL pbDebuggerPresent);

typedef BOOL(WINAPI* PXSymCleanupFunction)(_In_ HANDLE hProcess);
typedef DWORD(WINAPI* PXSymLoadModuleFunction)(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD BaseOfDll, _In_ DWORD SizeOfDll);
typedef BOOL(WINAPI* PXSymInitializeFunction)(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess);
typedef BOOL(WINAPI* PXSymEnumSymbolsFunction)(_In_ HANDLE hProcess, _In_ ULONG64 BaseOfDll, _In_opt_ PCSTR Mask, _In_ PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback, _In_opt_ PVOID UserContext);
typedef BOOL(WINAPI* PXSymEnumSymbolsExFunction)(_In_ HANDLE hProcess, _In_ ULONG64 BaseOfDll, _In_opt_ PCSTR Mask, _In_ PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback, _In_opt_ PVOID UserContext, _In_ DWORD Options);
typedef PVOID(WINAPI* PXSymFunctionTableAccessFunction)(__in HANDLE hProcess, __in DWORD AddrBase);
typedef DWORD(WINAPI* PXSymGetModuleBaseFunction)(__in HANDLE hProcess, __in DWORD dwAddr);


// Debug
typedef BOOL(WINAPI* PXStackWalk64Function)
(
    _In_ DWORD MachineType,
    _In_ HANDLE hProcess,
    _In_ HANDLE hThread,
    _Inout_ LPSTACKFRAME64 StackFrame,
    _Inout_ PVOID ContextRecord,
    _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
    _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
    _In_opt_ PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
    _In_opt_ PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress
    );

typedef DWORD(WINAPI* PXUnDecorateSymbolNameFunction)(_In_ PCSTR name, _Out_writes_(maxStringLength) PSTR outputString, _In_ DWORD maxStringLength, _In_ DWORD flags);
typedef BOOL(WINAPI* PXSymGetSymFromAddr64Function)(_In_ HANDLE hProcess, _In_ DWORD64 qwAddr, _Out_opt_ PDWORD64 pdwDisplacement, _Inout_ PIMAGEHLP_SYMBOL64  Symbol);







typedef struct PXSymbolStackWalkInfo_
{
#if OSUnix 
    int x;
#elif OSWindows
    DWORD MachineType;
    STACKFRAME                     StackFrame;
    CONTEXT                        ContextRecord;
    PREAD_PROCESS_MEMORY_ROUTINE   ReadMemoryRoutine;
    PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine;
    PGET_MODULE_BASE_ROUTINE       GetModuleBaseRoutine;
    PTRANSLATE_ADDRESS_ROUTINE     TranslateAddress;

    HANDLE HandleProcessCurrent;
    HANDLE HandleThreadCurrent;

    PGET_MODULE_BASE_ROUTINE64 SymbolFunctionTableAccess;
    PTRANSLATE_ADDRESS_ROUTINE64 SymbolGetModuleBase;
#endif
}
PXSymbolStackWalkInfo;






PXOS _PXOS;


const char PXOSName[] = "OS-Kernel";
const char PXOSSemaphore[] = "Semaphore";
const char PXOSCriticalSection[] = "CriticalSec.";


const char PXWindowsLibraryKernel32[] = "KERNEL32.DLL";
const char PXWindowsLibraryDebugHelp[] = "DBGHELP.DLL";







#if OSWindows

PXPrivate BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);

#endif












PXActionResult PXAPI PXOSAPIInit()
{
    if(_PXOS.DebugBreak)
    {
        return PXActionSuccessful;
    }

    PXClear(PXOS, &_PXOS);

    // kernel32.dll
    {
        PXLibraryOpenA(&_PXOS, PXWindowsLibraryKernel32);

        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
        {
            { &_PXOS.DebugEventContinue, "ContinueDebugEvent"},
            { &_PXOS.DebugBreak, "DebugBreak" },
            { &_PXOS.DebugProcessBreak, "DebugBreakProcess" },
            { &_PXOS.DebugEventWait, "WaitForDebugEvent" },
            { &_PXOS.DebugOutputDebugStringA, "OutputDebugStringA" },
            { &_PXOS.DebugOutputDebugStringW, "OutputDebugStringW" },
            { &_PXOS.DebugIsDebuggerPresent, "IsDebuggerPresent" },
            { &_PXOS.DebugRemotePresentCheck, "CheckRemoteDebuggerPresent" },
            { &_PXOS.DebugActiveProcessStop, "DebugActiveProcessStop" }
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryKernel, pxLibraryFuntionEntryList, amount);
    }

    // Dbghelp.dll
    {
        PXLibraryOpenA(&_PXOS.LibraryDebugHelp, PXWindowsLibraryDebugHelp);

        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
        {
            { &_PXOS.SymbolServerInitialize, "SymInitialize"},
            { &_PXOS.SymbolStackWalk, "StackWalk64"},
            { &_PXOS.SymbolUnDecorateName, "UnDecorateSymbolName"},
            { &_PXOS.SymbolFromAddress, "SymGetSymFromAddr64"},
            { &_PXOS.SymbolEnumerate, "SymEnumSymbols" },
            { &_PXOS.SymbolEnumerateEx, "SymEnumSymbolsEx" },
            { &_PXOS.SymbolModuleLoad, "SymLoadModule" },
            { &_PXOS.SymbolFunctionTableAccess, "SymFunctionTableAccess" },
            { &_PXOS.SymbolModuleBaseGet, "SymGetModuleBase" }
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&_PXOS.LibraryDebugHelp, pxLibraryFuntionEntryList, amount);
    }


    /*
        PXLibraryClose(&pxDebug->LibraryKernel);
    PXLibraryClose(&pxDebug->LibraryDebugHelp);
    */


    return PXActionSuccessful;
}

PXOS* PXAPI PXOSGet()
{
    return &_PXOS;
}

void PXAPI PXProcessCurrentGet()
{
    //const HANDLE processHandle = GetCurrentProcess();
}









PXActionResult PXAPI PXMemoryHeapCreate(PXMemoryHeap* const pxMemoryHeap)
{
#if OSUnix
#elif OSWindows
    pxMemoryHeap->HeapHandle = HeapCreate(NULL, 0, 0); // Windows XP, Kernel32.dll, heapapi.h
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMemoryHeapRelease(PXMemoryHeap* const pxMemoryHeap)
{
#if OSUnix
#elif OSWindows
    const BOOL result = HeapDestroy(pxMemoryHeap->HeapHandle); // Windows XP, Kernel32.dll, heapapi.h
#endif

    return PXActionSuccessful;
}

void PXAPI PXMemoryHeapGetGlobal(PXMemoryHeap* const pxMemoryHeap)
{
    pxMemoryHeap->HeapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h

    return PXActionSuccessful;
}

PXSize PXAPI PXMemoryHeapBlockSize(PXMemoryHeap* pxMemoryHeap, const void* const adress)
{
    PXMemoryHeap redirectHeap;

    // HeapSize cant handle NULL pointers. If we get one here, it sure is 0.
    if(!adress)
    {
        return 0;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if OSUnix  || PXMemoryUseFunctionSTD || OSForcePOSIXForWindows

    return _msize(adress);

#elif OSWindows  

    const SIZE_T size = HeapSize(pxMemoryHeap->HeapHandle, 0, adress);
    const PXActionResult sizeError = PXErrorCurrent((SIZE_T)-1 != size);

    return size;
#else
    return 0;
#endif
}

void* PXAPI PXMemoryHeapCalloc(PXMemoryHeap* pxMemoryHeap, const PXSize amount, const PXSize objectSize)
{
    const PXSize totalAmount = amount * objectSize;
    PXMemoryHeap redirectHeap;
    void* adress = PXNull;

    if(!totalAmount)
    {
        return PXNull;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if OSUnix || PXMemoryUseFunctionSTD || OSForcePOSIXForWindows
    adress = calloc(amount, objectSize);
#elif OSWindows
    adress = HeapAlloc(pxMemoryHeap->HeapHandle, HEAP_ZERO_MEMORY, totalAmount); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#else
#error Memory allocate seems not to be supported on this OS
#endif


    // Special logging behaviour
    {
        // PXSymbolMemory pxSymbolMemory;
        // pxSymbolMemory.Adress = adress;
        // pxSymbolMemory.Amount = amount;
         //pxSymbolMemory.ObjectSize = objectSize;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        //  pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
         // PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
         // PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
         // pxSymbolMemory.LineNumber = pxSymbol.LineNumber;

          //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);


#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Alloc",
            "<%p> %6ix %4i B %s::%s::%s::%i",
            adress,
            amount,
            objectSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbol.LineNumber
        );
#endif
    }

    return adress;
}

void* PXAPI PXMemoryHeapMalloc(PXMemoryHeap* pxMemoryHeap, const PXSize memorySize)
{
    PXMemoryHeap redirectHeap;
    void* adress = PXNull;

    if(!memorySize)
    {
        return PXNull;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    adress = malloc(memorySize);
#elif OSWindows
    adress = HeapAlloc(pxMemoryHeap->HeapHandle, 0, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#else
#error Memory allocate seems not to be supported on this OS
#endif

    // Special logging behaviour
    {
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Adress = adress;
        pxSymbolMemory.Amount = 1;
        pxSymbolMemory.ObjectSize = memorySize;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = pxSymbolMemory.LineNumber;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);

#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Alloc",
            "<%p> %4ix %4i B %s::%s::%s::%i",
            adress,
            pxSymbolMemory.Amount,
            pxSymbolMemory.ObjectSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber
        );
#endif
    }

    return adress;
}

PXBool PXAPI PXMemoryHeapFree(PXMemoryHeap* pxMemoryHeap, const void* const adress)
{
    PXMemoryHeap redirectHeap;

    if(!adress)
    {
        return PXFalse;
    }

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if PXLogEnable 
    const PXSize blockSize = PXMemoryHeapBlockSize(pxMemoryHeap, adress);
#endif


#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    free(adress);
#elif OSWindows
    const PXBool freeResult = HeapFree(pxMemoryHeap->HeapHandle, 0, adress); // Windows 2000 SP4, Kernel32.dll, heapapi.h


    // Special logging behaviour
    {
#if PXLogEnable 
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Adress = adress;
        pxSymbolMemory.Amount = -1;
        pxSymbolMemory.ObjectSize = -1;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = -1;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeRemove);

        PXLogPrint
        (
            PXLoggingAllocation,
            PXOSName,
            "Heap-Free",
            "<%p> %4i B %s::%s::%s::%i",
            adress,
            blockSize,
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber
        );
#endif
    }

    return freeResult;

#else
#error Memory release seems not to be supported on this OS
#endif
}

void* PXAPI PXMemoryHeapRealloc(PXMemoryHeap* pxMemoryHeap, const void* const adress, const PXSize memorySize)
{
    PXMemoryHeap redirectHeap;
    void* newAdress = PXNull;
    PXBool updatedLocation = PXFalse;

    if(!pxMemoryHeap)
    {
        PXMemoryHeapGetGlobal(&redirectHeap);
        pxMemoryHeap = &redirectHeap;
    }

#if PXLogEnable 
    const PXSize blockSizeOLD = PXMemoryHeapBlockSize(pxMemoryHeap, adress);
#endif


#if OSUnix || PXMemoryUseFunctionSTD || OSForcePOSIXForWindows
    // Function allows NULL as an adress
    newAdress = realloc(adress, memorySize);

    // clear new allocated space
    PXSize spaceNewSize = memorySize - blockSizeOLD;
    char* spaceNewAdress = (char*)newAdress + blockSizeOLD;

    PXMemoryClear(spaceNewAdress, spaceNewSize);

#elif OSWindows

    if(!adress)
    {
        void* memory = PXMemoryHeapCalloc(PXNull, 1, memorySize);

        //PXMemorySet(memory, 0xFF, memorySize);

        return memory;
    }

    newAdress = HeapReAlloc(pxMemoryHeap->HeapHandle, HEAP_ZERO_MEMORY, adress, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
    updatedLocation = newAdress != adress;

    // PXMemorySet(newAdress, '°', memorySize - oldSize);
#else
#error Memory reallocate seems not to be supported on this OS
#endif



#if PXLogEnable 
    const PXSize blockSizeNEW = PXMemoryHeapBlockSize(pxMemoryHeap, newAdress);
    const PXOffset offset = blockSizeNEW - blockSizeOLD;
#endif




#if PXLogEnable
    // Special logging behaviour
    {
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Adress = adress;
        pxSymbolMemory.Amount = memorySize;
        pxSymbolMemory.ObjectSize = 1;

        PXSymbol pxSymbol;
        PXSymbolStackTrace(&pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = -1;

        PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeUpdate);

        if(updatedLocation)
        {
            PXLogPrint
            (
                PXLoggingAllocation,
                PXOSName,
                "Heap-ReAlloc",
                "<%p> %i B -> <%p> %i B, (%i), %s::%s::%s::%i",
                adress,
                blockSizeOLD,
                newAdress,
                blockSizeNEW,
                offset,
                pxSymbol.NameModule,
                pxSymbol.NameFile,
                pxSymbol.NameSymbol,
                pxSymbolMemory.LineNumber
            );
        }
        else
        {
            PXLogPrint
            (
                PXLoggingAllocation,
                PXOSName,
                "Heap-ReAlloc",
                "<%p> %i B -> <No-Move> %i B, (%i), %s::%s::%s::%i",
                adress,
                blockSizeOLD,
                blockSizeNEW,
                offset,
                pxSymbol.NameModule,
                pxSymbol.NameFile,
                pxSymbol.NameSymbol,
                pxSymbolMemory.LineNumber
            );
        }
    }
#endif

    return newAdress;


}







void* PXAPI PXMemoryVirtualAllocate(PXSize size, PXSize* const createdSize, const PXAccessMode pxAccessMode)
{
    PXMemoryPageInfo pxFilePageFileInfo;
    PXSize recievedSize = 0;

    PXMemoryPageInfoFetch(&pxFilePageFileInfo, size);

    // Calculate if large pages shall be used
    const PXBool usePagesLarge = pxFilePageFileInfo.PageAmountLarge > 1;
    const PXBool usePagesHuge = pxFilePageFileInfo.PageAmountHuge > 1;

#if PXLogEnable
    const char* text = "Normal";

    if(usePagesLarge)
    {
        text = "Large";
    }

    if(usePagesHuge)
    {
        text = "Huge";
    }


    PXText pxTextPageSizeNormal;
    PXText pxTextPageSizeLarge;
    PXText pxTextPageSizeHuge;

    PXTextConstructNamedBufferA(&pxTextPageSizeNormal, pxTextPageSizeNormalBuffer, 32);
    PXTextConstructNamedBufferA(&pxTextPageSizeLarge, pxTextPageSizeLargeBuffer, 32);
    PXTextConstructNamedBufferA(&pxTextPageSizeHuge, pxTextPageSizeHugeBuffer, 32);

    PXTextFormatSize(&pxTextPageSizeNormal, pxFilePageFileInfo.PageSizeNormal);
    PXTextFormatSize(&pxTextPageSizeLarge, pxFilePageFileInfo.PageSizeLarge);
    PXTextFormatSize(&pxTextPageSizeHuge, pxFilePageFileInfo.PageSizeHuge);

    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        "Virtual-Alloc",
        "Allocating space for %i...\n"
        "%20s : %6s -> %3i%% %3ix\n"
        "%20s : %6s -> %3i%% %3ix\n"
        "%20s : %6s -> %3i%% %3ix\n"
        "%20s : %s",
        size,
        "Normal-PageSize", pxTextPageSizeNormal.TextA, (int)pxFilePageFileInfo.PageUtilizationNormal, pxFilePageFileInfo.PageAmountNormal,
        "Large-PageSize", pxTextPageSizeLarge.TextA, (int)pxFilePageFileInfo.PageUtilizationLarge, pxFilePageFileInfo.PageAmountLarge,
        "Huge-PageSize", pxTextPageSizeHuge.TextA, (int)pxFilePageFileInfo.PageUtilizationHuge, pxFilePageFileInfo.PageAmountHuge,
        "Targeted type", text
    );
#endif



    // Special behaviour if the object size is 0.
    // Ofcourse it would make no sense to create an object with 
    // a size of zero but lets assume we mean to just create one 
    // page for general useage
    if(0 == size)
    {
        size = pxFilePageFileInfo.PageSizeNormal;
    }






#if OSUnix
    // PROT_READ, PROT_WRITE = read&write permission
    // MAP_PRIVATE           = only for this process
    // MAP_ANONYMOUS         = no attached file, aka. "in memory"
    // MAP_POPULATE          = Preallocate, if not, we could get a SEGVAULT later if memory is suddenly not avalible
    int permission = PROT_READ | PROT_WRITE;
    int mode =
        MAP_PRIVATE |
        MAP_ANONYMOUS |
        MAP_POPULATE; // Will be ignored if not spesifcally enabled

    PXBool useLargePage = usePagesLarge && !usePagesHuge;

#if 0 // MAP_UNINITIALIZED not public??
    if(0) // Dont clear memory, can improve performance but will be ignored if not directly enabled. Safty reasons.
    {
        mode |= MAP_UNINITIALIZED;
    }
#endif

    if(useLargePage) // Create large page
    {
        mode |= MAP_HUGETLB | MAP_HUGE_2MB;
    }

    if(usePagesHuge) // Create huge page
    {
        mode |= MAP_HUGETLB | MAP_HUGE_1GB;
    }

    for(;;)
    {
        pxFile->Data = mmap(NULL, pxFileIOInfo->FileSizeRequest, permission, mode, -1, 0);
        const PXActionResult allocResult = PXErrorCurrent(MAP_FAILED != pxFile->Data);

        if(PXActionSuccessful == allocResult)
        {
            break;
        }

        // Did we do a normal allocation?
        if(!useLargePage && !usePagesHuge)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "File",
                "Open-Virtual",
                "Allocation failed! -> mmap()"
            );
#endif

            pxFile->Data = PXNull;
            pxFile->DataUsed = 0;
            pxFile->DataAllocated = 0;


            break;
        }


        // Try to recover to a normal page
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            "File",
            "Open-Virtual",
            "Allocation failed! We try again..."
        );
#endif


        if(useLargePage)
        {
            mode &= ~(MAP_HUGETLB | MAP_HUGE_2MB);
            useLargePage = PXFalse;
        }
        if(usePagesHuge)
        {
            mode &= ~(MAP_HUGETLB | MAP_HUGE_1GB);
            useLargePage = PXTrue;
        }
    }



#elif OSWindows

    DWORD permissions = PAGE_READWRITE;
    DWORD mode =
        MEM_RESERVE |
        MEM_COMMIT;

    if(usePagesLarge)
    {
#if OSWindows
        if(PXThreadPoolIsMainThread())
        {
            mode |= MEM_LARGE_PAGES;

            PXWindowsCheckPermission();
        }
        else
        {

        }
#endif


        // Adjust allocation size to be EXACTLY a multible of the page size
        // The documentation states it will be rounded up, this is true.
        // Except if we add the MEM_LARGE_PAGES flag, because microsoft.
        size = pxFilePageFileInfo.PageSizeLarge * pxFilePageFileInfo.PageAmountLarge;
    }

    // TODO: huge pages do not exist? We cant do them then.

    void* allocatedData = VirtualAlloc(PXNull, size, mode, permissions);
    const PXActionResult allocResult = PXErrorCurrent(0 < allocatedData);

    if(PXActionSuccessful != allocResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOSName,
            "Virtual-Alloc",
            "failed! -> VirtualAlloc()"
        );
#endif

        return PXNull;
    }


    MEMORY_BASIC_INFORMATION memoryInfo;
    PXClear(MEMORY_BASIC_INFORMATION, &memoryInfo);

    VirtualQuery(allocatedData, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

    recievedSize = memoryInfo.RegionSize;

#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingAllocation,
        PXOSName,
        "Virtual-Alloc",
        "<%p> Requested:<%i>, Got:<%i>",
        allocatedData,
        size,
        recievedSize
    );
#endif

    if(createdSize)
    {
        *createdSize = recievedSize;
    }

    return allocatedData;
}

void PXAPI PXMemoryVirtualPrefetch(const void* adress, const PXSize size)
{
#if OSUnix
#elif OSWindows
#if WindowsAtleast8 && PXOSWindowsDestop
    const HANDLE process = GetCurrentProcess();
    const PXSize numberOfEntries = 2;
    WIN32_MEMORY_RANGE_ENTRY memoryRangeEntry;
    const PXSize flags = 0; // reserved and needs to be 0

    memoryRangeEntry.VirtualAddress = (void*)adress;
    memoryRangeEntry.NumberOfBytes = size;

    //const bool prefetchResult = PrefetchVirtualMemory(process, numberOfEntries, &memoryRangeEntry, flags); // Windows 8, Kernel32.dll, memoryapi.h

#if PXLogEnable
    PXText pxTextSize;
    PXTextConstructNamedBufferA(&pxTextSize, pxTextBuffer, 32);
    PXTextFormatSize(&pxTextSize, size);

    PXLogPrint
    (
        PXLoggingInfo,
        "Memory",
        "Prefetch",
        "<%p> %s",
        adress,
        pxTextSize.TextA
    );
#endif

#else
    // Not supported function
#endif
#else

#if MemoryDebug
    printf("[#][Memory] 0x%p (%10zi B) Pre-Fetched [NOT SUPPORTED] Skipped...\n", adress, size);
#endif
#endif
}

PXActionResult PXAPI PXMemoryVirtualRelease(const void* adress, const PXSize size)
{
    if(!(adress && size))
    {
        return PXActionRefusedArgumentNull;
    }

#if PXLogEnable && PXMemoryDebug
    PXLogPrint
    (
        PXLoggingDeallocation,
        PXOSName,
        "Virtual-Free",
        "<%p> Size:%i",
        adress,
        size
    );
#endif

#if OSUnix
    const int resultID = munmap(adress, size); // sys/mman.h
    const PXActionResult unmapResult = PXErrorCurrent(0 == resultID);

    return unmapResult;

#elif OSWindows
    DWORD freeType = MEM_RELEASE;
    const PXBool freeResultID = VirtualFree((void*)adress, 0, freeType); // Windows XP (+UWP), Kernel32.dll, memoryapi.h
    const PXActionResult freeResult = PXErrorCurrent(freeResultID);

    return freeResult;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

void* PXAPI PXMemoryVirtualReallocate(const void* adress, const PXSize size)
{
    const PXBool newAllocation = adress == PXNull;

    if(newAllocation)
    {
        return PXMemoryVirtualAllocate(size, PXNull, PXAccessModeReadAndWrite);
    }

#if OSUnix
    return PXNull;
#elif OSWindows

    // Create new pages
    void* newSpaceMemory = PXMemoryVirtualAllocate(size, PXNull, PXAccessModeReadAndWrite);

    // Get size of current adress
    MEMORY_BASIC_INFORMATION memoryInfo;
    VirtualQuery(adress, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

    // Copy data to new pages
    PXMemoryCopy(adress, memoryInfo.RegionSize, newSpaceMemory, memoryInfo.RegionSize);

    // Delete old pages
    PXMemoryVirtualRelease(adress, memoryInfo.RegionSize);

    return newSpaceMemory;
#else
    return PXNull;
#endif

}
















PXActionResult PXAPI PXSymbolServerInitialize()
{
    PXOSAPIInit();

    const PXSymInitializeFunction pxSymbolServerInitialize = (PXSymInitializeFunction)_PXOS.SymbolServerInitialize;
    
   // PXThread pxThread;
   // PXThreadCurrent(&pxThread);
   // pxDebug->Process.ThreadHandle = pxThread.Info.Handle.ThreadHandle;

    HANDLE hProcess = GetCurrentProcess(); // Doc says this is not good?

    // Initializes the symbol handler for a process.
    const PCSTR UserSearchPath = NULL;
    const BOOL fInvadeProcess = TRUE;
    const BOOL initializeSuccessful = pxSymbolServerInitialize(hProcess, UserSearchPath, fInvadeProcess); // Dbghelp.dll, Dbghelp.h, SymInitialize
   // const PXActionResult result = PXErrorCurrent(initializeSuccessful);

    if(!initializeSuccessful)
    {
        return PXActionInvalid;
    }

   // const HANDLE processHandle = GetCurrentProcess();
   // const PXBool initRes = SymInitialize(processHandle, PXNull, PXTrue);
  //  DWORD xxxas = SymSetOptions(SYMOPT_LOAD_ANYTHING); // SYMOPT_LOAD_LINES


   // const PXBool initRes = SymInitialize(processHandle, PXNull, PXTrue);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolServerCleanup()
{
    HANDLE processID = GetCurrentProcess();
    const PXSymCleanupFunction pxSymCleanup = (PXSymCleanupFunction)_PXOS.SymbolServerCleanup;
    const PXBool cleanupSuccess = pxSymCleanup(processID); // DbgHelp.dll 5.1 or later

    if(!cleanupSuccess)
    {
        return PXActionFailedCleanup;
    }

    return PXActionFailedLoad;
}

PXActionResult PXAPI PXSymbolServerOptionsSet()
{
   // DWORD xxxas = SymSetOptions(SYMOPT_LOAD_ANYTHING); // SYMOPT_LOAD_LINES

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolListLoad(const PXProcessHandle processHandle, const void* baseAdress)
{
#if OSUnix


#elif OSWindows

    PXSymbolEnumerator pxSymbolEnumerator;
    pxSymbolEnumerator.SymbolList = 0;
    pxSymbolEnumerator.Amount = 0;
    pxSymbolEnumerator.Size = 128;

    // SymEnumSym, SymEnumerateSymbols64 is outdated?
    const PXSymEnumSymbolsFunction pxSymbolEnumerate = (PXSymEnumSymbolsFunction)_PXOS.SymbolEnumerate; // DbgHelp.dll 5.1 or later
    const PXBool enumerateResult = pxSymbolEnumerate
    (
        processHandle,
        baseAdress,
        0,
        PXLibraryNameSymbolEnumerate,
        &pxSymbolEnumerator
    );

#else

#endif
}

PXActionResult PXAPI PXSymbolModuleLoad(const PXProcessHandle processHandle, const char* moduleName, void** baseAdress)
{
    const PXSymLoadModuleFunction pxSymbolModuleLoad = (PXSymLoadModuleFunction)_PXOS.SymbolModuleLoad;
    const DWORD64 baseOfDll = pxSymbolModuleLoad // DbgHelp.dll 5.1 or later        SymLoadModuleEx, DbgHelp.dll 6.0 or later
    (
        processHandle,
        PXNull,
        moduleName,
        PXNull,
        0,
        0
    );

    const PXBool wasLoadingSuccessful = baseOfDll != 0;

    if(!wasLoadingSuccessful)
    {

    }


    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolStackWalk(PXSymbolStackWalkInfo* const pxSymbolStackWalkInfo)
{
     const PXStackWalk64Function pxStackWalk64 = (PXStackWalk64Function)_PXOS.SymbolStackWalk;
// const PXSymGetSymFromAddr64 pXSymGetSymFromAddr64 = (PXSymGetSymFromAddr64)pxDebug->SymbolFromAddress;

    const BOOL result = pxStackWalk64
    (
        pxSymbolStackWalkInfo->MachineType,
        pxSymbolStackWalkInfo->HandleProcessCurrent,
        pxSymbolStackWalkInfo->HandleThreadCurrent,
        &pxSymbolStackWalkInfo->StackFrame,
        &pxSymbolStackWalkInfo->ContextRecord,
        NULL,
        pxSymbolStackWalkInfo->SymbolFunctionTableAccess,
        pxSymbolStackWalkInfo->SymbolGetModuleBase,
        NULL
    );
    const PXBool sucessfull = result != 0;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSymbolStackTrace(PXSymbol* const pxSymbolList, const PXSize pxSymbolListAmount, const PXSize start, const PXSize depth)
{
    PXMemoryClear(pxSymbolList, sizeof(PXSymbol) * pxSymbolListAmount);

#if OSUnix 
    void* array[10];
    size_t size;
    char** strings;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    //printf("Obtained %zd stack frames.\n", size);

    // Example "../../PXUltima/Code/[Export]/PXUltimaC/64B-Linux-Debug/PXUltima.so(PXMemoryHeapMalloc+0xb8) [0x7fce66aa763a]"

    PXSize symbolOffset = 0;


    for(PXSize i = start; i < start + depth; ++i)
    {
        PXSymbol* const pxSymbol = &pxSymbolList[symbolOffset];
        const char* traceEntry = strings[i];

        // printf("%s\n", traceEntry);

        const PXSize lastSlash = PXTextFindLastCharacterA(traceEntry, PXTextUnkownLength, '/');

        if(lastSlash != -1)
        {
            // Get module name
            char* moduleName = &traceEntry[lastSlash + 1];

            const PXSize moduleNameSize = PXTextFindLastCharacterA(moduleName, PXTextUnkownLength, '(');


            char* moduleFunction = &moduleName[moduleNameSize + 1];
            const PXSize moduleFunctionSize = PXTextFindLastCharacterA(moduleFunction, PXTextUnkownLength, '+');

            PXTextCopyA(moduleName, moduleNameSize, pxSymbol->NameModule, 64);
            PXTextCopyA(moduleFunction, moduleFunctionSize, pxSymbol->NameSymbol, 64);
            PXTextCopyA("???", 3, pxSymbol->NameFile, 64);
            pxSymbol->LineNumber = -1;

            //   printf("    - %s::%s\n", pxSymbol->NameModule, pxSymbol->NameSymbol);
        }
    }


    free(strings);

#elif PXOSWindowsDestop

    // Prevent failure because not inizilized

#if PXLogEnable && PXDebugStackTracePrint
    PXLogPrint
    (
        PXLoggingInfo,
        PXDebugerText,
        PXDebugerStackTraceText,
        "----- BEGIN -----"
    );
#endif



     //const HANDLE processHeapHandle = GetProcessHeap();
    const HANDLE handleThreadCurrent = GetCurrentThread();
    const HANDLE handleProcessCurrent = GetCurrentProcess();
    const DWORD processID = GetProcessId(handleProcessCurrent);

    const PXActionResult initializeResult = PXSymbolServerInitialize();

    if(PXActionSuccessful != initializeResult)
    {
        for(size_t i = 0; i < pxSymbolListAmount; i++)
        {
            PXSymbol* pxSymbol = &pxSymbolList[i];

            PXTextCopyA("???", 3, pxSymbol->NameUndecorated, 64);
            PXTextCopyA("???", 3, pxSymbol->NameSymbol, 64);
            PXTextCopyA("???.c", 3, pxSymbol->NameFile, 64);
            PXTextCopyA("???.dll", 3, pxSymbol->NameModule, 64);
        }

        return PXActionInvalid;
    }

    const DWORD optionsMask = SymSetOptions(SYMOPT_DEBUG | SYMOPT_LOAD_ANYTHING | SYMOPT_LOAD_LINES);


    // SymLoadModuleEx(,);





    PXSymbolStackWalkInfo pxSymbolStackWalkInfo;

    PXClear(PXSymbolStackWalkInfo, &pxSymbolStackWalkInfo);

    pxSymbolStackWalkInfo.MachineType = IMAGE_FILE_MACHINE_AMD64; // IMAGE_FILE_MACHINE_I386

    RtlCaptureContext(&pxSymbolStackWalkInfo.ContextRecord);
    pxSymbolStackWalkInfo.StackFrame.AddrPC.Mode = AddrModeFlat;
    pxSymbolStackWalkInfo.StackFrame.AddrStack.Mode = AddrModeFlat;
    pxSymbolStackWalkInfo.StackFrame.AddrFrame.Mode = AddrModeFlat;

#if OS32B
    pxSymbolStackWalkInfo.StackFrame.AddrPC.Offset = pxSymbolStackWalkInfo.ContextRecord.Eip;
    pxSymbolStackWalkInfo.StackFrame.AddrStack.Offset = pxSymbolStackWalkInfo.ContextRecord.Esp;
    pxSymbolStackWalkInfo.StackFrame.AddrFrame.Offset = pxSymbolStackWalkInfo.ContextRecord.Ebp;
#elif OS64B
    pxSymbolStackWalkInfo.StackFrame.AddrPC.Offset = pxSymbolStackWalkInfo.ContextRecord.Rip;
    pxSymbolStackWalkInfo.StackFrame.AddrStack.Offset = pxSymbolStackWalkInfo.ContextRecord.Rsp;
    pxSymbolStackWalkInfo.StackFrame.AddrFrame.Offset = pxSymbolStackWalkInfo.ContextRecord.Rbp;
#endif


    PXSize frame;
    PXSize symbolIndex = 0;

    PXClear(PXSymbol, pxSymbolList);

    for(frame = 0; frame < (start + depth); ++frame)
    {
        const PXActionResult result = PXSymbolStackWalk(&pxSymbolStackWalkInfo);

        if(PXActionSuccessful != result)
        {
            break;
        }

        if(frame < start)
        {
            continue;
        }

        void* symbolAdress = (ULONG64)pxSymbolStackWalkInfo.StackFrame.AddrPC.Offset; // ULONG64

        PXSymbol* pxSymbol = &pxSymbolList[symbolIndex];
        // PXSymbol pxSymbol;
        // PXClear(PXSymbol, &pxSymbol);

        PXSymbolFromAddress(pxSymbol, symbolAdress);
        PXDebugModuleNameGet(pxSymbol->ModuleAdress, pxSymbol->NameModule, 64, PXNull, PXDebugModuleNameShort);

        ++symbolIndex;


#if PXLogEnable  && PXDebugStackTracePrint
        PXLogPrint
        (
            PXLoggingInfo,
            PXDebugerText,
            PXDebugerStackTraceText,
            "%c 0x%p - %23s - %s",
            (frame == 0 ? '/' : '|'),
            symbolAdress,
            pxSymbol->NameModule,
            pxSymbol->NameUndecorated
        );
#endif







        /*

#define PXMSDebugSymbolNameSize 268

        typedef union PXMSDebugSymbol_
        {
            IMAGEHLP_SYMBOL Symbol;
            PXByte Buffer[sizeof(IMAGEHLP_SYMBOL) + PXMSDebugSymbolNameSize];
        }
        PXMSDebugSymbol;

        PXMSDebugSymbol pxMSDebugSymbol;

        PXClear(PXMSDebugSymbol, &pxMSDebugSymbol);
        pxMSDebugSymbol.Symbol.SizeOfStruct = sizeof(PXMSDebugSymbol);
        pxMSDebugSymbol.Symbol.MaxNameLength = PXMSDebugSymbolNameSize;

        const BOOL getResult = pXSymGetSymFromAddr64(pxDebug->Process.ProcessHandle, (ULONG64)stackFrame.AddrPC.Offset, &displacement, &pxMSDebugSymbol.Symbol);
        const PXBool getFailed = getResult != 0;*/




#if PXLogEnable && PXDebugStackTracePrint && 0
#if 0
        printf
        (
            "| %-55s() |\n"
            "|  PC address:     0x%08p |\n"
            "|  Stack address:  0x%08p |\n"
            "|  Frame address:  0x%08p |\n",
            pxMSDebugSymbol.Symbol.Name,
            (void*)stackFrame.AddrPC.Offset,
            (void*)stackFrame.AddrStack.Offset,
            (void*)stackFrame.AddrFrame.Offset
        );
#else
        char functioName[64];
        char adress[64];

        sprintf_s(functioName, 64, "%s()", pxMSDebugSymbol.Symbol.Name);
        sprintf_s(adress, 64, "0x%p", (void*)pxMSDebugSymbol.Symbol.Address);

        printf("| %-34s | %-18s |\n", functioName, adress);
#endif
#endif
    }


#if PXLogEnable && PXDebugStackTracePrint
    PXLogPrint
    (
        PXLoggingInfo,
        PXDebugerText,
        PXDebugerStackTraceText,
        "%c %s %i Jumps",
        '^',
        "(START)",
        frame
    );
#endif

#if PXLogEnable && PXDebugStackTracePrint
    PXLogPrint
    (
        PXLoggingInfo,
        PXDebugerText,
        PXDebugerStackTraceText,
        "----- END -----"
    );
#endif

#endif
}

PXActionResult PXAPI PXSymbolUnDecorateName(const char* inputName, char* name, const PXSize nameLengthMax, PXSize* nameLengthWritten)
{
    if(!(inputName && name && nameLengthMax))
    {
        return PXActionRefusedArgumentNull;
    }

    const PXUnDecorateSymbolNameFunction pxUnDecorateSymbolName = (PXUnDecorateSymbolNameFunction)_PXOS.SymbolUnDecorateName;

    if(!pxUnDecorateSymbolName)
    {
        return PXActionRefusedNoSuchFunction;
    }

    const DWORD decResultSize = pxUnDecorateSymbolName(inputName, (PSTR)name, nameLengthMax, UNDNAME_COMPLETE); // UnDecorateSymbolName
    const PXActionResult pxActionResult = PXErrorCurrent(0 != decResultSize);

    if(nameLengthWritten)
    {
        nameLengthWritten = 0;
    }

    return pxActionResult;
}

PXActionResult PXAPI PXSymbolFromAddress(PXSymbol* const pxSymbol, const void* const adress)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    PXClear(PXSymbol, pxSymbol);

    const HANDLE processHandle = GetCurrentProcess(); // TODO: what if we want another process?

    // Extract symbol
    {
        DWORD64 displacement = 0;

        union
        {
            char buffer[sizeof(SYMBOL_INFO) + sizeof(TCHAR) * 200]; // MAX_SYM_NAME
            SYMBOL_INFO SymbolInfo;
        }
        symbolInfo;


        symbolInfo.SymbolInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
        symbolInfo.SymbolInfo.MaxNameLen = 200;

        const PXBool symbolFetchSuccess = SymFromAddr(processHandle, adress, &displacement, &symbolInfo.SymbolInfo);
        const PXActionResult symbolFetchResult = PXErrorCurrent(symbolFetchSuccess);

        /*
        BOOL asas = SymGetTypeInfo
        (
          processHandle,
          [in]  DWORD64                   ModBase,
          [in]  ULONG                     TypeId,
          [in]  IMAGEHLP_SYMBOL_TYPE_INFO GetType,
          [out] PVOID                     pInfo
        );
        */

        /*
        if(!symbolFetchSuccess)
        {
            // Try again but with another functiuon

            IMAGEHLP_LINE64 lineInfo;
            lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            lineInfo.FileName = &symbolInfo;

            const PXBool symbolFetchSuccessW = SymGetLineFromAddr64(processHandle, adress, &displacement, &lineInfo);
            const PXActionResult symbolFetchSuccessWRes = PXWindowsErrorCurrent(symbolFetchSuccess);


            IMAGEHLP_SYMBOL64* imageHelpSYMBOL = &symbolInfo; // function avove failed, reuse memory
            imageHelpSYMBOL->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            imageHelpSYMBOL->MaxNameLength = 2000;

            const PXBool symbolFetchSuccessB = SymGetSymFromAddr64(processHandle, adress, &displacement, imageHelpSYMBOL);

            return PXActionInvalid;
        }*/


        //  SYM_TYPE xx = symbolInfo.SymbolInfo.TypeIndex;
        // SymTagEnum xxxx = symbolInfo->Tag;

        PXSize written =
            PXSymbolUnDecorateName(symbolInfo.SymbolInfo.Name, (PSTR)pxSymbol->NameUndecorated, 64, &written);


        pxSymbol->Amount = 1;
        pxSymbol->ObjectSize = symbolInfo.SymbolInfo.Size;
        pxSymbol->ModuleAdress = symbolInfo.SymbolInfo.ModBase;
        pxSymbol->SymbolAdress = symbolInfo.SymbolInfo.Address;

        PXTextCopyA(symbolInfo.SymbolInfo.Name, symbolInfo.SymbolInfo.NameLen, pxSymbol->NameSymbol, 64);


        // Index
        {
            const BOOL succca = SymFromIndex
            (
                processHandle,
                pxSymbol->ModuleAdress,
                symbolInfo.SymbolInfo.Index,
                &symbolInfo.SymbolInfo
            );

            if(succca)
            {
                // override
                PXTextCopyA(symbolInfo.SymbolInfo.Name, symbolInfo.SymbolInfo.NameLen, pxSymbol->NameSymbol, 64);

                char bufferB[64];
                IMAGEHLP_LINE64 imageHelperLine;
                imageHelperLine.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
                imageHelperLine.FileName = bufferB;

                const PXBool lineResult = SymGetLineFromAddr(processHandle, symbolInfo.SymbolInfo.Address, &displacement, &imageHelperLine);
                char* fileNameAdress = imageHelperLine.FileName;
                PXSize fileNameAdressSize = 0;

                if(lineResult)
                {
                    const PXSize index = PXTextFindLastCharacterA(imageHelperLine.FileName, -1, '\\');

                    if(-1 != index)
                    {
                        fileNameAdress += index + 1;
                        fileNameAdressSize = PXTextLengthA(fileNameAdress, 64);
                    }

                    pxSymbol->LineNumber = imageHelperLine.LineNumber;
                }

                PXTextCopyA(fileNameAdress, -1, pxSymbol->NameFile, 64);

            }
        }
    }
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXSymbolEnumerate()
{
   
}

PXActionResult PXAPI PXSymbolFunctionTableAccess()
{
   
}

PXActionResult PXAPI PXSymbolModuleBaseGet()
{
 
}


const PXInt32U _lockWaitSpan = 100;
const PXInt32U _lockWaitTrys = 100;


PXActionResult PXAPI PXSemaphorCreate(PXLock* const pxLock)
{
    PXActionResult pxActionResult;


#if OSUnix
 
    int sharedPointer = 0;
    unsigned int value = 1;

    const int resultID = sem_init(&pxLock->ID, sharedPointer, value);
    pxActionResult = PXErrorCurrent(0 == resultID); // 0=sucessful, -1=Error

#elif OSWindows
    LONG lInitialCount = 1;
    LONG lMaximumCount = 1;

    pxLock->SemaphoreHandle = CreateSemaphoreA(PXNull, lInitialCount, lMaximumCount, PXNull);
    pxActionResult = PXErrorCurrent(pxLock->SemaphoreHandle != PXNull); 

#else
    pxActionResult = PXActionRefusedNotImplemented;
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Created: HANDLE:<%p>",
        pxLock->SemaphoreHandle
    );
#endif

    return pxActionResult; // Success
}

PXActionResult PXAPI PXSemaphorDelete(PXLock* const pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Created: HANDLE:<%p>",
        pxLock->SemaphoreHandle
    );
#endif

#if OSUnix
    const int closingResult = sem_destroy(&lock->ID);
#elif OSWindows
    const BOOL closingResult = CloseHandle(pxLock->SemaphoreHandle);
#endif
}

PXActionResult PXAPI PXSemaphorEnter(PXLock* const pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Enter"
    );
#endif

    PXActionResult waitResult = PXActionInvalid;

#if OSUnix
    const int waitResultID = sem_wait(&lock->SemaphoreHandle);
    waitResult = PXErrorCurrent(0 == waitResultID);
#elif OSWindows

    PXSize tryCounter = 0;

    for(;;)
    {
        const DWORD result = WaitForSingleObject(pxLock->SemaphoreHandle, _lockWaitSpan); // INFINITE


        if(WAIT_OBJECT_0 == result)
        {
            waitResult = PXActionSuccessful;
            // We got runtime
            break;
        }   

        if(WAIT_TIMEOUT == result)
        {
            ++tryCounter;

            if(_lockWaitTrys >= tryCounter)
            {
                continue;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                PXOSName,
                PXOSSemaphore,
                "Failed.. timeout!"
            );
#endif


            waitResult = PXActionRefusedNotReady;
            break;
        }


        switch(result)
        {
            case WAIT_ABANDONED:
                waitResult = PXActionRefusedStateInvalid;
                break;

            case WAIT_FAILED:
                waitResult = 0;// PXErrorCurrent(0);
                break;
        }
    }
    
#endif

    return waitResult;
}

PXActionResult PXAPI PXSemaphorLeave(PXLock* const pxLock)
{ 
    if(!pxLock)
    {
        return PXActionInvalid;
    }

    if(0 == pxLock->SemaphoreHandle)
    {
        return PXActionInvalid;
    }

#if PXLogEnable  && 0
    PXLogPrint 
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Leave"
    );
#endif

#if OSUnix
    const int releaseResultID = sem_post(&pxLock->ID);
    const PXActionResult releaseResult = PXErrorCurrent(0 == releaseResultID);
#elif OSWindows
    const BOOL releaseResultID = ReleaseSemaphore(pxLock->SemaphoreHandle, 1, PXNull);
    const PXActionResult releaseResult = PXErrorCurrent(releaseResultID);
#endif   
}

PXActionResult PXAPI PXCriticalSectionCreate(PXLock* const pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Create"
    );
#endif

#if OSUnix

#elif OSWindows     
    InitializeCriticalSection(&pxLock->SectionHandle);
#endif  
}

PXActionResult PXAPI PXCriticalSectionDelete(PXLock* const pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Delete"
    );
#endif

#if OSUnix

#elif OSWindows
    DeleteCriticalSection(&pxLock->SectionHandle);
#endif
}




PXActionResult PXAPI PXCriticalSectionEnter(PXLock* const pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Enter"
    );
#endif

#if OSUnix

#elif OSWindows
    CRITICAL_SECTION* const criticalSectionCast = (CRITICAL_SECTION*)&pxLock->SectionHandle;

    PXSize failEnterCounter = 0;

    for(;;)
    {
        const BOOL success = TryEnterCriticalSection(criticalSectionCast);

        if(success)
        {
            break;
        }

        ++failEnterCounter;

        if(failEnterCounter < 10)
        {
            Sleep(0);
            continue;
        }

#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingInfo,
            PXOSName,
            PXOSCriticalSection,
            "Enter failed! Waiting..."
        );
#endif

        Sleep(100);


    }

    //EnterCriticalSection(criticalSectionCast);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCriticalSectionLeave(PXLock* const pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Leave"
    );
#endif

#if OSUnix

#elif OSWindows
    CRITICAL_SECTION* const criticalSectionCast = (CRITICAL_SECTION*)&pxLock->SectionHandle;

    LeaveCriticalSection(criticalSectionCast);
#endif

    return PXActionSuccessful;
}










PXActionResult PXAPI PXDebugEventContinue(const PXInt32U processID, const PXInt32U threadID)
{
#if OSUnix

    const long ree = ptrace(PTRACE_CONT, pxDebug->Process.ProcessID, NULL, NULL);

    return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
    const PXContinueDebugEventFunction pxContinueDebugEvent = (PXContinueDebugEventFunction)_PXOS.DebugEventContinue;
    const DWORD dwContinueStatus = 0;
    const PXBool continueResultID = pxContinueDebugEvent(processID, threadID, dwContinueStatus); // Windows XP, Kernel32.dll, debugapi.h
    const PXActionResult continueResult = PXErrorCurrent(continueResultID);

    return continueResult;
#else

    return PXActionRefusedNotImplemented;

#endif
}

PXActionResult PXAPI PXDebugEventWait(void* pxDebugEventInfo, const PXInt32U time)
{
    // WaitForDebugEvent() Windows XP, Kernel32.dll, debugapi.h
// WaitForDebugEventEx() Windows 10, Kernel32.dll, debugapi.h

    // Windows XP, Kernel32.dll, debugapi.h

    const PXWaitForDebugEventFunction pxWaitForDebugEvent = (PXWaitForDebugEventFunction)_PXOS.DebugEventWait;
    DEBUG_EVENT* debugEvent = (DEBUG_EVENT*)pxDebugEventInfo;

    const BOOL success = pxWaitForDebugEvent(debugEvent, time);
    const PXActionResult pxActionResult = PXErrorCurrent(success);

    return pxActionResult;
}

PXActionResult PXAPI PXDebugProcessActive()
{
    
}

PXActionResult PXAPI PXDebugBreak()
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    const PXDebugBreakFunction pxDebugBreak = (PXDebugBreakFunction)_PXOS.DebugBreak;

    PXDebugBreak(); // Windows XP (+UWP), Kernel32.dll, debugapi.h

    return PXActionSuccessful; // We dont have any other result
#else
    return PXActionRefusedNotImplemented;
#endif
}

PXActionResult PXAPI PXDebugOutputDebugStringA(const char* text)
{
#if OSUnix
#elif OSWindows
    const PXOutputDebugStringAFunction pxOutputDebugStringA = (PXOutputDebugStringAFunction)_PXOS.DebugOutputDebugStringA;

    pxOutputDebugStringA(text); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
}

PXActionResult PXAPI PXDebugOutputDebugStringW(const wchar_t* text)
{
#if OSUnix
#elif OSWindows
    const PXOutputDebugStringWFunction pxOutputDebugStringW = (PXOutputDebugStringWFunction)_PXOS.DebugOutputDebugStringW;

    pxOutputDebugStringW(text); // Windows XP (+UWP), Kernel32.dll, debugapi.h
#endif
}

PXActionResult PXAPI PXDebugProcessBreak()
{
 
}

PXActionResult PXAPI PXDebugIsPresent(PXBool* const isPresent)
{
#if OSUnix
#elif OSWindows
    const PXIsDebuggerPresentFunction pxIsDebuggerPresent = (PXIsDebuggerPresentFunction)_PXOS.DebugIsDebuggerPresent;
    const PXBool result = pxIsDebuggerPresent(); // Windows XP (+UWP), Kernel32.dll, debugapi.h

    if(isPresent)
    {
        *isPresent = result;
    }

    return PXActionSuccessful;
#endif
}

PXActionResult PXAPI PXDebugIsPresentRemote(const PXProcessHandle processHandle, PXBool* const isPresent)
{
    if(!isPresent)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif PXOSWindowsDestop
    const PXCheckRemoteDebuggerPresentFunction pxCheckRemoteDebuggerPresent = (PXCheckRemoteDebuggerPresentFunction)_PXOS.DebugRemotePresentCheck;

    BOOL debuggerPresent = 0;
    const BOOL result = pxCheckRemoteDebuggerPresent(processHandle, &debuggerPresent); // Windows XP, Kernel32.dll, debugapi.h
    const PXActionResult pxActionResult = PXErrorCurrent(result);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    *isPresent = debuggerPresent;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDebugActiveProcessStop(const PXInt32U processID)
{
#if OSUnix
    const long result = ptrace(PTRACE_DETACH, pxDebug->Process.ProcessID, 0, 0);

#elif PXOSWindowsDestop

    const PXDebugActiveProcessStopFunction pxDebugActiveProcessStop = (PXDebugActiveProcessStopFunction)_PXOS.DebugActiveProcessStop;

    if(!pxDebugActiveProcessStop)
    {
        return PXActionRefusedNotSupportedByOperatingSystem;
    }

    const BOOL successful = pxDebugActiveProcessStop(processID);
    const PXActionResult pxActionResult = PXErrorCurrent(successful);

    return pxActionResult;
#endif
}

PXActionResult PXAPI PXDebugModuleNameGet
(
    const PXHandleModule pxHandleModule,
    char* moduleName,
    const PXSize moduleNameSize,
    PXSize* const sizeWritten,
    const PXInt32U flags
)
{
    char moduleNameBuffer[PXPathSizeMax];
    PXClearList(char, moduleNameBuffer, PXPathSizeMax);
    PXSize moduleNameLength = 0;

    PXActionResult moduleFetchResult;

    // Stage 1) Module to string
#if OSUnix

    if(pxHandleModule)
    {
        /*
        Dl_info info;

        const int resultID = dladdr(current_module, &info);
        const PXActionResult moduleFetchResult = PXErrorCurrent((0 != resultID));

        if(info.dli_fname)
        {
            printf("Current shared object: %s\n", info.dli_fname);
        }
        else {
            printf("Could not determine the current shared object.\n");
            */
    }
    else
    {
        // The HANDLE is NULL. So we want the path of the current executable
        moduleNameLength = readlink("/proc/self/exe", moduleName, PXPathSizeMax); // unistd.h
        moduleFetchResult = PXErrorCurrent(-1 != count);
    }

#elif OSWindows

    moduleNameLength = GetModuleFileNameA(pxHandleModule, moduleNameBuffer, PXPathSizeMax);
    moduleFetchResult = PXErrorCurrent(0 != moduleNameLength);

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif


    if(PXActionSuccessful != moduleFetchResult)
    {
        moduleName[0] = '?';
        moduleName[1] = '?';
        moduleName[2] = '?';
        moduleName[3] = '\0';

        if(sizeWritten)
        {
            *sizeWritten = 3;
        }

        return moduleFetchResult;
    }


    // Stage 2) Truncate name
    if(!(PXDebugModuleNameShort & flags))
    {
        PXTextReplaceByte(moduleNameBuffer, moduleNameLength, '\\', '/');
        PXTextCopyA(moduleNameBuffer, moduleNameLength, moduleName, moduleNameSize);


        if(sizeWritten)
        {
            *sizeWritten = moduleNameLength;
        }

        return PXActionSuccessful; // We dont do any further processing
    }




#if OSUnix

    moduleName[count] = '\0';
    *sizeWritten = count;

    // Truncate
    const PXSize lastSlashPosition = PXTextFindLastCharacterA(moduleName, count, '/');

    if(lastSlashPosition != -1)
    {
        PXSize sourceSize = count - lastSlashPosition;
        char* source = &moduleName[lastSlashPosition];

        *sizeWritten = PXMemoryMove(source, sourceSize, moduleName, moduleNameSize);
    }


#elif OSWindows


    // net to get moduleinto, but we have those
    // IMAGEHLP_MODULE64 mMAGEHLP_MODULE64;
    // mMAGEHLP_MODULE64.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
    //  const PXBool moduleFetchSuccessaSAS = SymGetModuleInfo64(processHandle, moduleHandle, &mMAGEHLP_MODULE64);


    // Extract names
    {
        char* molduleNamefixed = moduleNameBuffer;
        PXSize actualSize = moduleNameLength;

        const char systemPath[] = "C:\\WINDOWS\\SYSTEM32\\";
        const PXSize systemPathSize = sizeof(systemPath);
        const PXBool isSystem = PXTextCompareIgnoreCaseA(systemPath, systemPathSize, moduleNameBuffer, systemPathSize);

        if(isSystem)
        {
            molduleNamefixed += systemPathSize - 1;
            actualSize -= systemPathSize - 1;

            // Check if its a driver
            const char driverStoreText[] = "DriverStore";
            const PXSize driverStoreTextSize = sizeof(driverStoreText);

            const PXBool isDriver = PXTextCompareA(driverStoreText, driverStoreTextSize, molduleNamefixed, driverStoreTextSize, 0);

            if(isDriver)
            {
                // The path is very long and useless, only get the DLL name.
                const PXSize lastSlash = PXTextFindLastCharacterA(molduleNamefixed, actualSize, '\\');

                if(lastSlash != -1)
                {
                    molduleNamefixed += lastSlash + 1;
                    actualSize -= lastSlash + 1;
                }
            }
        }
        else
        {
            char currentWorkPath[MAX_PATH];
            PXClearList(char, currentWorkPath, MAX_PATH);
            PXSize currentWorkPathSize = GetModuleFileName(NULL, currentWorkPath, MAX_PATH);


            PXSize lastSlashA = PXTextFindLastCharacterA(currentWorkPath, currentWorkPathSize, '\\');
            PXSize lastSlashB = PXTextFindLastCharacterA(moduleNameBuffer, currentWorkPathSize, '\\');
            PXBool isRelativePath = PXTextCompareA(currentWorkPath, lastSlashA, moduleNameBuffer, lastSlashB, 0);

            if(isRelativePath)
            {
                molduleNamefixed += lastSlashA + 1;
                actualSize -= lastSlashA + 1;
            }
        }

        moduleNameLength = PXTextCopyA(molduleNamefixed, actualSize, moduleName, PXPathSizeMax);
    }
#endif

    if(sizeWritten)
    {
        *sizeWritten = moduleNameLength;
    }

    return PXActionSuccessful;
}



#if OSUnix
#elif PXOSWindowsDestop
BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
    PXSymbolEnumerator* const pxSymbolEnumerator = (PXSymbolEnumerator*)UserContext;

    PXSymbol* pxSymbol = &pxSymbolEnumerator->SymbolList[pxSymbolEnumerator->Amount];
    pxSymbol->TypeIndex = pSymInfo->TypeIndex;
    pxSymbol->Index = pSymInfo->Index;
    pxSymbol->ObjectSize = pSymInfo->Size;
    pxSymbol->ModuleAdress = (void*)pSymInfo->ModBase;
    pxSymbol->Flags = pSymInfo->Flags;
    pxSymbol->Address = pSymInfo->Address;
    pxSymbol->Register = pSymInfo->Register;
    pxSymbol->Scope = pSymInfo->Scope;
    pxSymbol->Type = (PXSymbolType)pSymInfo->Tag;

    PXTextCopyA(pSymInfo->Name, pSymInfo->NameLen, pxSymbol->NameSymbol, 64);

    ++(pxSymbolEnumerator->Amount);

    return PXTrue;
}
#endif