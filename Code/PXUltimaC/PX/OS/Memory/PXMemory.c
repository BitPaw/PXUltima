#include "PXMemory.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Debug/PXDebug.h>

#include <stdlib.h>
#include <malloc.h>

const char PXMemoryLogPrintTitle[] = "OS-Kernel";
const char PXMemoryLogPrintMemoryVirtual[] = "Virtual-Memory";
const char PXMemoryLogPrintMemory[] = "Memory";

const char PXMemoryLogPrintMemoryCalloc[] = "Memory-Calloc";
const char PXMemoryLogPrintMemoryMalloc[] = "Memory-Malloc";
const char PXMemoryLogPrintMemoryRealloc[] = "Memory-Realloc";
const char PXMemoryLogPrintMemoryFree[] = "Memory-Free";

const char PXMemoryLogPrintMemoryVirtualAlloc[] = "Virtual-Alloc";
const char PXMemoryLogPrintMemoryVirtualFree[] = "Virtual-Free";
const char PXMemoryLogPrintMemoryVirtualRealloc[] = "Virtual-Realloc";

#if OSUnix

#include <sys/mman.h>
#include <alloca.h>

#define ProtectionIDRead PROT_READ
#define ProtectionIDWrite PROT_WRITE
#define ProtectionIDReadWrite PROT_READ | PROT_WRITE

#elif OSWindows

#include <Windows.h>
#include <crtdbg.h>
#include <Psapi.h>

#define ProtectionIDRead PAGE_READONLY
#define ProtectionIDWrite PAGE_READWRITE
#define ProtectionIDReadWrite PAGE_READWRITE



/*
PXActionResult PXAPI WindowsProcessPrivilege(const char* pszPrivilege, BOOL bEnable)
{
    HANDLE           hToken;
    TOKEN_PRIVILEGES tp;
    BOOL             status;
    DWORD            error;

    // open process token
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return PXErrorCurrent(); // OpenProcessToken

    // get the luid
    if (!LookupPrivilegeValueA(NULL, pszPrivilege, &tp.Privileges[0].Luid))
        return PXErrorCurrent();  // LookupPrivilegeValue

    tp.PrivilegeCount = 1;

    // enable or disable privilege
    if (bEnable)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // enable or disable privilege
    status = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    // It is possible for AdjustTokenPrivileges to return TRUE and still not succeed.
    // So always check for the last error value.
    error = GetLastError();
    if (!status || (error != ERROR_SUCCESS))
        return PXErrorCurrent();  // AdjustTokenPrivileges

    // close the handle
    if (!CloseHandle(hToken))
        return PXErrorCurrent(); // CloseHandle

    return PXActionSuccessful;
}*/


#endif

//---<Settings>------------------------

#if MemoryAssertEnable
#include <assert.h>
#endif

#if PXMemoryDebug
#include <stdio.h>
#endif

#if MemoryDebugLeakDetection
#include <PX/Container/PXDictionary/PXDictionary.h>
#include <Time/Time.h>
#include <Psapi.h>

PXDictionary _memoryAdressLookup;

struct MemoryAllocationInfo
{
    Time TimeLastChanged; // When
    PXSize Size;
    const char* Source;
};

#endif
//-------------------------------------


static PXMemorySymbolLookup _PXGLOBLALMemorySymbolLookup;
static PXBool _PXGLOBLALMemorySymbolLookupENABLED = PXFalse;


void PXAPI PXAccessModeToStringA(char* text, const const PXAccessMode pxAccessMode)
{
    text[0] = (PXAccessREAD     & pxAccessMode) > 0 ? 'R' : '-';
    text[1] = (PXAccessWRITE    & pxAccessMode) > 0 ? 'W' : '-';
    text[2] = (PXAccessEXECUTE  & pxAccessMode) > 0 ? 'X' : '-';
    text[3] = 0;
}

PXMemorySymbolLookup* PXAPI PXMemorySymbolLookupInstanceGet(void)
{
    if(!_PXGLOBLALMemorySymbolLookupENABLED)
    {
        PXClear(PXMemorySymbolLookup, &_PXGLOBLALMemorySymbolLookup);

        PXDictionaryConstruct(&_PXGLOBLALMemorySymbolLookup, sizeof(void*), sizeof(PXSymbolMemory), PXDictionaryValueLocalityInternalEmbedded);

        _PXGLOBLALMemorySymbolLookupENABLED = PXTrue;
    }

    return &_PXGLOBLALMemorySymbolLookup;
}

void PXAPI PXMemorySymbolAdd(PXSymbolMemory* const pxSymbolMemory,  const PXMemorySymbolInfoMode pxMemorySymbolInfoMode)
{
    PXMemorySymbolLookup* const pxMemorySymbolLookup = PXMemorySymbolLookupInstanceGet();

    switch(pxMemorySymbolInfoMode)
    {
        case PXMemorySymbolInfoModeAdd:
        {
            PXDictionaryAdd(&pxMemorySymbolLookup->SymbolLookup, &pxSymbolMemory->Adress, pxSymbolMemory);
            break;
        }
        case PXMemorySymbolInfoModeUpdate:
        {

            break;
        }
        case PXMemorySymbolInfoModeRemove:
        {

            break;
        }
        default:
            break;
    }
}

PXActionResult PXAPI PXMemorySymbolFetch(const void* const adress, PXSymbol* const pxSymbol)
{
    PXMemorySymbolLookup* const pxMemorySymbolLookup = PXMemorySymbolLookupInstanceGet();
    PXSymbolMemory* symbolMemory = PXNull;

    PXBool success = PXDictionaryFindEntry(&pxMemorySymbolLookup->SymbolLookup, &adress, &symbolMemory);

    PXClear(PXSymbol, pxSymbol);

    if(!success)
    {
        return PXActionRefusedNotInitialized;
    }


    pxSymbol->Amount = symbolMemory->Amount;
    pxSymbol->ObjectSize = symbolMemory->ObjectSize;

    // Get module name from adress

    pxSymbol->ModuleAdress = symbolMemory->ModuleAdress;
    PXDebugModuleNameGet(symbolMemory->ModuleAdress, pxSymbol->NameModule, 64, PXNull, PXDebugModuleNameShort);

    PXTextCopyA(symbolMemory->FunctionAdress, 64, pxSymbol->NameSymbol, 64);
    PXTextCopyA(symbolMemory->FileAdress, 64, pxSymbol->NameFile, 64);
    // PXTextCopyA(symbolMemory->N, 64, pxSymbol->Name, 64);

    return PXActionSuccessful;
}

PXBool PXAPI PXMemoryDoAdressesOverlap(void* const adressA, const PXSize adressALengh, void* const adressB, const PXSize adressBLengh)
{
    // What adress is the lower one?
    // We could force the user to only use a as a lower adress but not right now

    PXSize adressLowerSize = PXNull;
    void* adressLower = PXNull;
    void* adressHigher = PXNull;

    if(adressA <= adressB)
    {
        adressHigher = adressB;
        adressLower = adressA;
        adressLowerSize = adressALengh;
    }
    else
    {
        adressHigher = adressA;
        adressLower = adressB;
        adressLowerSize = adressBLengh;

        // ToDo: Print message to user: Order of adresses swapped
    }

    PXBool overlap = ((PXSize)adressLower + adressLowerSize) >= adressHigher;

    return overlap;
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

        PXDebug* pxDebug = PXDebugInstanceGet();

        PXSymbol pxSymbol;
        PXDebugStackTrace(pxDebug, &pxSymbol, 1, 2, 1);

      //  pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
       // PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
       // PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
       // pxSymbolMemory.LineNumber = pxSymbol.LineNumber;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);


#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            PXMemoryLogPrintMemoryCalloc,
            "<%p> %4ix %4i B %s::%s::%s::%i",
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

        PXDebug* pxDebug = PXDebugInstanceGet();

        PXDebugStackTrace(pxDebug, &pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = pxSymbolMemory.LineNumber;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);

#if PXLogEnable 
        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            "Memory-Malloc",
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

        PXDebug* pxDebug = PXDebugInstanceGet();

        PXDebugStackTrace(pxDebug, &pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = -1;

        //PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeRemove);

        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            "Memory-Free",
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

        PXDebug* pxDebug = PXDebugInstanceGet();

        PXDebugStackTrace(pxDebug, &pxSymbol, 1, 2, 1);

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
                PXMemoryLogPrintTitle,
                PXMemoryLogPrintMemoryRealloc,
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
                PXMemoryLogPrintTitle,
                PXMemoryLogPrintMemoryRealloc,
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

int PXAPI PXMemoryProtectionIDTranslate(const PXInt8U protectionMode)
{
    int protectionID = 0;
#if OSUnix

    if(PXAccessModeNoAccess == PXAccessNone)
    {
        protectionID = PROT_NONE;
    }

    protectionID |=
        PROT_READ  * ((PXAccessREAD    & protectionMode) > 0) |
        PROT_WRITE * ((PXAccessWRITE   & protectionMode) > 0) |
        PROT_EXEC  * ((PXAccessEXECUTE & protectionMode) > 0);

#elif OSWindows

    switch(protectionMode)
    {
    case PXAccessModeNoAccess:
        protectionID = PAGE_NOACCESS;
        break;

    case PXAccessModeReadOnly:
        protectionID = PAGE_READONLY;
        break;

    case PXAccessModeWriteOnly:
        protectionID = PAGE_WRITECOPY;
        break;

    case PXAccessModeReadAndWrite:
        protectionID = PAGE_READWRITE;
        break;

    case PXAccessModeReadExecute:
        protectionID = PAGE_EXECUTE_READ;
        break;

    case PXAccessModeFull:
        protectionID = PAGE_EXECUTE_READWRITE;
        break;

    case PXAccessModeWriteExecute:
        protectionID = PAGE_EXECUTE_WRITECOPY;
        break;

    default:
        break;
    }

#endif

    return protectionID;
}

PXActionResult PXAPI PXMemoryProtect(void* dataAdress, const PXSize dataSize, const PXInt8U protectionMode)
{
    const PXInt32U protectionID = PXMemoryProtectionIDTranslate(protectionMode);

#if OSUnix
    const int protectResultID = mprotect(dataAdress, dataSize, protectionID); // libc.so, sys/mman.h
    const PXActionResult protectResult = PXErrorCurrent(0 == protectResultID);

    return protectResult;
#elif OSWindows

    DWORD oldProtectModeID = 0;

    const PXBool result = VirtualProtect(dataAdress, dataSize, protectionID, &oldProtectModeID); // Windows XP (+UWP), Kernel32.dll, memoryapi.h
    const PXActionResult actiobResult = PXErrorCurrent(result);

    return actiobResult;
#else
    return PXActionRefusedTypeNotSupported;
#endif
}


PXBool PXAPI PXMemoryScan(PXMemoryUsage* memoryUsage)
{
    PXClear(PXMemoryUsage, memoryUsage);

#if OSUnix
#elif WindowsAtleastXP
    // MEMORYSTATUS won't work on >4GB Systems

    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    const PXBool result = GlobalMemoryStatusEx(&memoryStatus);



    if(result)
    {
        memoryUsage->PercentInUse = memoryStatus.dwMemoryLoad;
        memoryUsage->PhysicalTotal = memoryStatus.ullTotalPhys;
        memoryUsage->PhysicalAvailable = memoryStatus.ullAvailPhys;
        memoryUsage->PageFileTotal = memoryStatus.ullTotalPageFile;
        memoryUsage->PageFileAvailable = memoryStatus.ullAvailPageFile;
        memoryUsage->VirtualTotal = memoryStatus.ullTotalVirtual;
        memoryUsage->VirtualAvailable = memoryStatus.ullAvailVirtual;
        memoryUsage->ExtendedVirtualAvailable = memoryStatus.ullAvailExtendedVirtual;
    }


    {
        ULONGLONG ramSizte = 0;

        BOOL x = 0;// GetPhysicallyInstalledSystemMemory(&ramSizte);

        if (x)
        {
            memoryUsage->PhysicalRAMSize = ramSizte;
        }

    }

    return result;
#endif
}

void PXAPI PXMemoryClear(void* const PXRestrict bufferA, const PXSize bufferASize)
{
    PXMemorySet(bufferA, 0u, bufferASize);

    //ZeroMemory(bufferA, bufferASize);
}

void PXAPI PXMemorySetI32U(int* const PXRestrict bufferA, const int value, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        ((int volatile*)bufferA)[i] = value;
    }
}

void PXAPI PXMemorySet(void* const PXRestrict buffer, const PXByte value, const PXSize bufferSize)
{
//#if MemoryAssertEnable
//    assert(bufferA);
//#endif

#if PXMemoryDebug
    //printf("[#][Memory] 0x%p (%10zi B) Set to %2x\n", bufferA, bufferASize, value);
#endif

    if(!buffer)
    {
        return;
    }

#if MemoryUseSystemFunction
    memset(buffer, value, bufferSize);
#elif OSWindows && PXDefaultLibraryEnable
    FillMemory(buffer, bufferSize, value);
#else
    for(PXSize i = 0; i < bufferSize; ++i)
    {
        ((PXAdress)buffer)[i] = value;
    }
#endif
}

#include <immintrin.h>

PXInt8U PXAPI PXMemoryCompareI8V(const PXInt8U* const textList, const PXInt8U listAmount, const PXInt8U value)
{
    __m512i zero = _mm512_setzero_si512();
    //__m512i zero = _mm512_set1_epi8('~');

    __m512i value_vector = _mm512_set1_epi8(value); // Load target byte gets copy'ed 64x 

    for(PXInt8U i = 0; i < listAmount; i += 64)
    { 
        __mmask64 mask = ((listAmount -i) > 64) ? 0xFFFFFFFFFFFFFFFF : (1LL << (listAmount - (i))) -1; // How many things can we load?
        __m512i data_vector = _mm512_mask_loadu_epi8(zero, mask, &textList[i]); // Load compare array

        const PXInt64U result = _mm512_cmp_epi8_mask(value_vector, data_vector, _MM_CMPINT_EQ); // Compare both 64x byte vs byte

        char bufferA[64 * 2+1];
        char bufferB[64 * 2+1];

        for(size_t i = 0; i < 64; ++i)
        {
            bufferA[2 * i + 0] = value_vector.m512i_u8[i];
            bufferA[2 * i + 1] = ' ';
            bufferA[2 * i + 2] = 0;

            bufferB[2 * i + 0] = PXCharMakePrintable(data_vector.m512i_u8[i]);
            bufferB[2 * i + 1] = ' ';
            bufferB[2 * i + 2] = 0;
        }

#if 0
        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            "SIMD-Compare",
            "64x (%i) Target: %c\n"
            "%s\n"
            "%s",
            listAmount,
            value,
            bufferA,
            bufferB
        );
#endif


        if(!result) // Not a single match, get to next
        {
            continue;
        }

        // We found a match!

#if OS64B
        const PXInt8U match_index = 63 - _lzcnt_u64(result); // Count leading zeros. We want the first one.
#else
        const PXInt8U match_index = 63 - _lzcnt_u32(result); // Count leading zeros. We want the first one.
#endif
 

        return i + match_index; // Index of first hit
    }

    return (PXInt8U)-1; // No match!
}

PXInt8U PXAPI PXMemoryCompareC32V(const char* value, char* const textList[4], const PXInt8U listAmount)
{
    __m512i value_vector = _mm512_set1_epi32(*(int*)value); // Load target char[4], gets copy'ed 16x 

    for(PXInt8U i = 0; i < listAmount; i += 16)
    {
        __m512i data_vector = _mm512_loadu_epi32(&textList[i]); // Load compare array

        const __mmask16 result = _mm512_cmp_epi32_mask(value_vector, data_vector, _MM_CMPINT_EQ); // Compare both 16x char[4]

        if(!result) // Not a single match, get to next
        {
            continue;
        }

        // We found a match!

        const unsigned int match_index = 31 - _lzcnt_u32(result); // Count leading zeros. We want the first one.

        return i + match_index; // Index of first hit
    }

    return (PXInt8U)-1; // No match!
}

PXInt8U PXAPI PXMemoryCompareSVI8(const char* const stringTarget, char** const stringList, const PXInt8U* const stringSizeList, const PXInt8U amount)
{
    // Load the target string into a 512-bit AVX512 register
    __m512i target = _mm512_set1_epi64(*(PXInt64U*)stringTarget); // Mask for max 8 bytes
    __m512i currentString = _mm512_setzero_si512();

    PXSize offset = 0;

    for(size_t roundIndex = 0; roundIndex < amount; ++roundIndex)
    {
        // Upload strings
        for(PXInt8U batchIndex = 0; batchIndex < 8; ++batchIndex)
        {
            char* stringSource = stringList[(roundIndex * 8) +batchIndex];
            PXInt8U stringLength = stringSizeList[(roundIndex*8) + batchIndex];


            // Load the current string in the list into another AVX512 register
           // __m512i currentString = _mm512_maskz_loadu_epi8(0xFF, stringList[i]);
            PXInt64U mask = (1LLu << stringLength) -1;

            // Add already written bytes
            mask <<= batchIndex*8;

          //  offset += 8-stringLength;


            // Push one string
            currentString = _mm512_mask_loadu_epi8(currentString, mask, stringSource - batchIndex * 8);
        }


        // Compare


        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            "SIMD-Compare",
            "8x char[] vs char[]\n"
            "%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n"
            "%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s",
            &target.m512i_i8[8 * 0],
            &target.m512i_i8[8 * 1],
            &target.m512i_i8[8 * 2],
            &target.m512i_i8[8 * 3],
            &target.m512i_i8[8 * 4],
            &target.m512i_i8[8 * 5],
            &target.m512i_i8[8 * 6],
            &target.m512i_i8[8 * 7],

            &currentString.m512i_i8[8 * 0],
            &currentString.m512i_i8[8 * 1],
            &currentString.m512i_i8[8 * 2],
            &currentString.m512i_i8[8 * 3],
            &currentString.m512i_i8[8 * 4],
            &currentString.m512i_i8[8 * 5],
            &currentString.m512i_i8[8 * 6],
            &currentString.m512i_i8[8 * 7]
        );


        // Compare the target string with the current string (element-wise comparison)
        __mmask8 resultMask = _mm512_cmp_epi64_mask(target, currentString, _MM_CMPINT_EQ);

        if(!resultMask)
        {
            PXLogPrint
            (
                PXLoggingAllocation,
                PXMemoryLogPrintTitle,
                "SIMD-Compare",
                "No match in this batch (%i/%i)",
                roundIndex+1,
                amount/8
            );

            continue;
        }

        const PXInt64U match_index = 31 - _lzcnt_u32(resultMask); // Count leading zeros. We want the first one.
        const PXInt8U index = (roundIndex *8)+ match_index;

        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            "SIMD-Compare",
            "Match found! index:<%i>",
            index
        );

        return index;
    }    


    PXLogPrint
    (
        PXLoggingAllocation,
        PXMemoryLogPrintTitle,
        "SIMD-Compare",
        "Absolutly no match found"
    );

    // Return a sentinel value if no match is found
    return (PXInt8U)-1;

}

PXInt8U PXAPI PXMemoryReadBitI32U(const PXInt32U* const pxInt32U, const PXInt8U index)
{
    return _bittest(pxInt32U, index); // [Intrinsic] intrin.h, winnt.h
}

PXInt8U PXAPI PXMemoryReadBitI64U(const PXInt64U* const pxInt64U, const PXInt8U index)
{
    return 0;// _bittest64(pxInt64U, index);
}

PXInt8U PXAPI PXMemoryReadBitAndClearI32U(const PXInt32U* const pxInt32U, const PXInt8U index)
{
    return 0;
}

int PXAPI PXMemoryCompareThreeWay(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize)
{
    const PXSize bufferSize = PXMathMinimumIU(bufferASize, bufferBSize);


#if MemoryAssertEnable
    assert(bufferA);
    assert(bufferB);
#endif

#if PXMemoryDebug  && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Memory",
        "Compare",
        "%6zi B  0x%p == 0x%p",
        bufferSize,
        bufferA,
        bufferB
    );
#endif

#if MemoryUseSystemFunction
    return memcmp(bufferA, bufferB, bufferSize);
#else

    PXSize acumulatorA = 0;
    PXSize acumulatorB = 0;

    for (PXSize counter = bufferSize ; counter ; --counter)
    {
        const PXBool isEqual = ((PXAdress)bufferA)[counter] == ((PXAdress)bufferB)[counter];

        acumulatorA += !isEqual * ((PXAdress)bufferA)[counter];
        acumulatorB += !isEqual * ((PXAdress)bufferB)[counter];
    }

    const PXSize minimum = PXMathMaximum(acumulatorA, acumulatorB);
    const PXSize maximum = PXMathMaximum(acumulatorA, acumulatorB);
    const int result = maximum - minimum;

    return result;
#endif
}

PXBool PXAPI PXMemoryCompareToByte(const void* PXRestrict bufferA, const PXSize bufferASize, const PXByte dataByte)
{
    if(!(bufferA && bufferASize))
    {
        return 0;
    }

    PXSize equalSum = 0;

    for (PXSize counter = bufferASize; counter; --counter)
    {
        equalSum += dataByte == ((PXAdress)bufferA)[counter];
    }

    return equalSum == bufferASize;
}

PXBool PXAPI PXMemoryCompare(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize)
{
    const PXSize bufferSize = PXMathMinimumIU(bufferASize, bufferBSize);

    if(bufferSize == 0)
    {
        return PXFalse;
    }

#if MemoryAssertEnable
    assert(bufferA);
    assert(bufferB);
#endif

#if PXMemoryDebug && 0

    if(bufferSize == 1)
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "Memory",
            "Compare",
            "%6zi B  0x%2.2x == 0x%2.2x",
            bufferSize,
            *(char*)bufferA,
            *(char*)bufferB
        );
    }
    else
    {
        const PXSize bufferCap = 8;
        const PXSize bufferACap = PXMathMinimumIU(bufferASize, bufferCap);
        const PXSize bufferBCap = PXMathMinimumIU(bufferBSize, bufferCap);

        PXText pxTextA;
        PXText pxTextB;
        PXTextConstructNamedBufferA(&pxTextA, pxTextABuffer, 16);
        PXTextConstructNamedBufferA(&pxTextB, pxTextBBuffer, 16);
        PXTextFormatData(&pxTextA, bufferA, bufferSize);
        PXTextFormatData(&pxTextB, bufferB, bufferSize);

        PXLogPrint
        (
            PXLoggingInfo,
            "Memory",
            "Compare",
            "%6zi B  <%s> == <%s>",
            bufferSize,
            pxTextA.TextA,
            pxTextB.TextA
        );
    }


#endif

#if MemoryUseSystemFunction
    return memcmp(bufferB, bufferA, bufferSize) == 0;
#else
    PXSize equalSum = 0;

    for (PXSize counter = bufferSize; counter; --counter)
    {
        equalSum += ((PXAdress)bufferA)[counter-1] == ((PXAdress)bufferB)[counter-1];
    }

    return equalSum == bufferSize;
#endif
}

PXBool PXAPI PXMemoryIsEmpty(const void* PXRestrict buffer, const PXSize bufferSize)
{
    PXBool isNull = PXTrue;

    for (PXSize i = 0; isNull && i < bufferSize; ++i)
    {
        isNull = ((PXAdress)buffer)[i] == 0;
    }

    return isNull;
}

PXBool PXAPI PXMemorySwap(void* PXRestrict bufferA, void* PXRestrict bufferB, const PXSize size)
{
    void* adress = PXMemoryHeapCallocT(char, size);

    PXMemoryCopy(bufferA, size, adress, size);
    PXMemoryCopy(bufferB, size, bufferA, size);
    PXMemoryCopy(adress, size, bufferB, size);

    PXMemoryHeapFree(PXNull, adress);

    return PXTrue;
}

const void* PXAPI PXMemoryLocateFirst(const void* const PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize)
{
#if MemoryUseSystemFunction
    const void* memoryPosition = memchr(inputBuffer, byteBlock, inputBufferSize);

    return memoryPosition;
#else
    PXBool found = PXFalse;
    PXSize index = 0;

    for (; index < inputBufferSize && !found; ++index)
    {
        found = byteBlock == ((char volatile*)inputBuffer)[index];
    }

    return found * (PXSize)((char volatile*)inputBuffer + index);
#endif
}

const void* PXAPI PXMemoryLocateLast(const void* const PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize)
{
#if MemoryUseSystemFunction

    void* position = strrchr(inputBuffer, byteBlock);

    return position;
#else

    // Own implementation
    PXSize maxRange = inputBufferSize;

    if(-1 == inputBufferSize)
    {
        void* endOfString = PXMemoryLocateFirst(inputBuffer, 0, (PXSize)-1);

        maxRange = (PXSize)endOfString - (PXSize)inputBuffer;
    }


    for(PXSize i = maxRange; i > 0; --i)
    {
        const char data = ((char volatile*)inputBuffer)[i];
        const PXBool isFound = byteBlock == data;

        if(isFound)
        {
            return &((char volatile*)inputBuffer)[i];
        }
    }

#endif

    return PXNull;
}

PXSize PXAPI PXMemoryCopy(const void* PXRestrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
    const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

    if(!(inputBuffer && outputBuffer && bufferSize > 0))
    {
        return 0;
    }

#if MemoryAssertEnable
    //assert(bufferSize > 0);
    assert(inputBuffer);
    assert(outputBuffer);
#endif

#if PXMemoryDebug && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Memory",
        "Copy",
        "%6zi B  0x%p -> 0x%p",
        bufferSize,
        inputBuffer,
        outputBufferSize
    );
#endif

#if PXMemoryUseFunctionSTD
    memcpy(outputBuffer, inputBuffer, bufferSize);
#elif PXMemoryUseFunctionOS
    CopyMemory(outputBuffer, inputBuffer, bufferSize);
#else
    for (PXSize index = bufferSize ; index ; --index)
    {
        ((char volatile*)outputBuffer)[index-1] = ((char volatile*)inputBuffer)[index-1];
    }
#endif

    return bufferSize;
}

void PXAPI PXMemoryCopyF16V(PXF16* const destination, const PXF16* const source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF32V(PXF32* const destination, const PXF32* const source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF64V(PXF64* const destination, const PXF64* const source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF32ToF16V(PXF32* const destination, const PXF16* const source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF16ToF32V(PXF16* const destination, const PXF32* const source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

PXSize PXAPI PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
    const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

#if PXMemoryUseFunctionSTD
    memmove(outputBuffer, inputBuffer, bufferSize);
#elif PXMemoryUseFunctionOS
    MoveMemory(outputBuffer, inputBuffer, bufferSize);
#else
    // Need a solution for a copy without a variable
    //memmove(outputBuffer, inputBuffer, bufferSize);

    for(size_t i = 0; i < bufferSize; i++)
    {
        char volatile* a = &((char volatile*)inputBuffer)[i];
        char volatile* b = &((char volatile*)outputBuffer)[i];       

        *b = *a;
    }
#endif 

    return bufferSize;
}

void PXAPI PXMemoryPageInfoFetch(PXMemoryPageInfo* const pxFilePageFileInfo, const PXSize objectSize)
{
    PXClear(PXMemoryPageInfo, pxFilePageFileInfo);

#if OSUnix

    // Might need : "sudo apt-get install libhugetlbfs-dev"
    // Seems to not do anything.
    // Does linux have large table support?
    // getconf PAGESIZE
    // getconf LARGE_PAGESIZE
    // getconf HUGE_PAGESIZE

    pxFilePageFileInfo->PageSizeNormal = getpagesize(); //  also works, sysconf(PAGESIZE );
    pxFilePageFileInfo->PageSizeLarge = 1 << 21;//sysconf(_SC_LARGE_PAGESIZE); MAP_HUGE_2MB
    pxFilePageFileInfo->PageSizeHuge = 1 << 30;//sysconf(_SC_HUGE_PAGESIZE); MAP_HUGE_1GB

#elif OSWindows
    PERFORMANCE_INFORMATION perfInfo;
    DWORD size = sizeof(PERFORMANCE_INFORMATION);
    PXClear(PERFORMANCE_INFORMATION, &perfInfo);
    perfInfo.cb = size;

    const BOOL result = GetPerformanceInfo(&perfInfo, size);

    if(!result)
    {
        return;
    }

    pxFilePageFileInfo->PageSizeNormal = perfInfo.PageSize;
    pxFilePageFileInfo->PageSizeLarge = GetLargePageMinimum(); // Windows Vista, Kernel32.dll, memoryapi.h
    // pxFilePageFileInfo->PageSizeHuge = 0; // Does this even exist?

#else

#endif


    // Calc the size
    pxFilePageFileInfo->PageAmountNormal = (objectSize / pxFilePageFileInfo->PageSizeNormal + 1);

    pxFilePageFileInfo->PageUtilizationNormal = (objectSize * 100) / (pxFilePageFileInfo->PageSizeNormal * pxFilePageFileInfo->PageAmountNormal);

    if(pxFilePageFileInfo->PageSizeLarge > 0)
    {
        pxFilePageFileInfo->PageAmountLarge = objectSize / pxFilePageFileInfo->PageSizeLarge + 1;
        pxFilePageFileInfo->PageUtilizationLarge = (objectSize * 100) / (pxFilePageFileInfo->PageSizeLarge * pxFilePageFileInfo->PageAmountLarge);
    }

    if(pxFilePageFileInfo->PageSizeHuge > 0)
    {
        pxFilePageFileInfo->PageAmountHuge = objectSize / pxFilePageFileInfo->PageSizeHuge + 1;
        pxFilePageFileInfo->PageUtilizationHuge = (objectSize * 100) / (pxFilePageFileInfo->PageSizeHuge * pxFilePageFileInfo->PageAmountHuge);
    }
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
        PXMemoryLogPrintTitle,
        "Virtual-Alloc",
        "Allocating space for %i...\n"
        "%20s : %6s -> %3i%% %3ix\n"
        "%20s : %6s -> %3i%% %3ix\n"
        "%20s : %6s -> %3i%% %3ix\n"
        "%20s : %s",
        size,
        "Normal-PageSize", pxTextPageSizeNormal.TextA,(int)pxFilePageFileInfo.PageUtilizationNormal, pxFilePageFileInfo.PageAmountNormal,
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
        mode |= MEM_LARGE_PAGES;

#if OSWindows
        // A call to VirtualAlloc() with MEM_LARGE_PAGES
        // WILL normally fail, because we dont have permissions..?
        // We have permissions, it is only disabled by default.



       // AddPrivileges();




        TOKEN_PRIVILEGES privileges;
        HANDLE hToken;
        LUID luid;



        // Open the process token
        const BOOL openTokenID = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
        const PXActionResult openTokenResult = PXErrorCurrent(openTokenID);


        const BOOL lookupSuccess = LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME, &luid); // SeLockMemoryPrivilege
        const PXActionResult lookupSuccessResult = PXErrorCurrent(lookupSuccess);

        privileges.PrivilegeCount = 1;
        privileges.Privileges[0].Luid = luid;
        privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        const BOOL privilgeResultID = AdjustTokenPrivileges
        (
            hToken,
            FALSE,
            &privileges,
            sizeof(TOKEN_PRIVILEGES),
            PXNull,
            PXNull
        );
        const PXActionResult privilgeResult = PXErrorCurrent(privilgeResultID);

        CloseHandle(hToken);
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
            PXMemoryLogPrintTitle,
            PXMemoryLogPrintMemory,
            "Virtual-Alloc failed! -> VirtualAlloc()"
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
        PXMemoryLogPrintTitle,
        PXMemoryLogPrintMemoryVirtualAlloc,
        "VirtualAlloc <%p> Requested:<%i>, Got:<%i>",
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
        PXMemoryLogPrintTitle,
        PXMemoryLogPrintMemoryVirtualFree,
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

    if (newAllocation)
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

#define PXMemoryUseStackAllocation 0

PXActionResult PXAPI PXMemoryStackAllocate(PXMemoryInfo* const pxMemoryInfo)
{
#if PXMemoryUseStackAllocation
    const PXSize totalSize = pxMemoryInfo->TypeSize * pxMemoryInfo->Amount;
    void* const stackAllocated =

#if OSUnix
        alloca(totalSize);
#elif OSWindows
        //_alloca(size); // _alloca() is deprecated (security reasons) but _malloca() is not an alternative

        _malloca(totalSize);
#endif

    * pxMemoryInfo->Data = stackAllocated;

    if(pxMemoryInfo->SizeTotal)
    {
        *pxMemoryInfo->SizeTotal = totalSize;
    }

#if PXLogEnable && PXMemoryDebug
    PXLoggingEventData pxLoggingEventData;
    pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
    pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
    pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
    pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
    pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
    pxLoggingEventData.ModuleSource = "Memory";
    pxLoggingEventData.ModuleAction = "Stack-Alloc";
    pxLoggingEventData.PrintFormat = "";
    pxLoggingEventData.Type = PXLoggingAllocation;
    pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

    PXLogPrintInvoke(&pxLoggingEventData);
#endif

#else

    return PXActionSuccessful; // PXMemoryHeapAllocate(pxMemoryInfo);

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMemoryStackDeallocate(PXMemoryInfo* const pxMemoryInfo)
{
    if(!pxMemoryInfo)
    {
        return;
    }

#if PXMemoryUseStackAllocation

#if OSUnix

#elif OSWindows
    _freea(*pxMemoryInfo->Data);
#endif

    *pxMemoryInfo->Data = PXNull;

    if(pxMemoryInfo->SizeTotal)
    {
        *pxMemoryInfo->SizeTotal = 0;
    }


#if PXLogEnable && 0
    PXLoggingEventData pxLoggingEventData;
    pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
    pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
    pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
    pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
    pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
    pxLoggingEventData.ModuleSource = "Memory";
    pxLoggingEventData.ModuleAction = "Stack Release";
    pxLoggingEventData.PrintFormat = "";
    pxLoggingEventData.Type = PXLoggingDeallocation;
    pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

    PXLogPrintInvoke(&pxLoggingEventData);
#endif

#else

    return PXActionSuccessful; // PXMemoryHeapDeallocate(pxMemoryInfo);

#endif


}

/*

// Bool is we have ECC memory?
GetMemoryErrorHandlingCapabilities(); // Windows 8, Kernel32.dll, memoryapi.h

// out-cached pages?
GetSystemFileCacheSize(); // Windows XP 64Bit, Kernel32.dll, memoryapi.h
SetSystemFileCacheSize(); // Windows XP 64Bit, Kernel32.dll, memoryapi.h

*/


/*
void* PXMemoryHeapAllocateCleared(const PXSize objectSize, const PXSize amount)
{
    if (!objectSize || !amount)
    {
        return PXNull;
    }

    void* const adress = calloc(amount, objectSize);

#if PXMemoryDebug
    printf("[#][Memory] 0x%p (%10zi B) Allocate on heap cleared\n", adress, requestedSizeInBytes);
#endif

    return adress;
}

PXBool PXMemoryHeapResizeArray(PXSize typeSize, void** dataAddress, PXSize* const dataAddressSize, const PXSize dataAddressSizeRequired)
{
    {
        const PXBool needsResize = *dataAddressSize < dataAddressSizeRequired;

        if (!needsResize)
        {
            return PXTrue; // Array is big enough, resize not needed
        }
    }

    void* newAdress = PXMemoryHeapReallocateTypeClear
    (
        *dataAddress,
        typeSize,
        *dataAddressSize,
        dataAddressSizeRequired
    );

    {
        const PXBool reallocSuccess = newAdress != 0;

        if (!reallocSuccess)
        {
            return PXFalse;
        }
    }

    *dataAddressSize = dataAddressSizeRequired;
    *dataAddress = newAdress;

    return PXTrue;

}
void* PXMemoryHeapReallocateClear(void* const sourceAddress, const PXSize sizeBefore, const PXSize sizeAfter)
{
    void* const adressReallocated = realloc(sourceAddress, sizeAfter);
    const PXBool sizeIncredes = sizeAfter > sizeBefore;

    if (sizeIncredes)
    {
        const PXAdress startAdress = (PXAdress)adressReallocated + sizeBefore;
        const PXSize sizeDelta = sizeAfter - sizeBefore;

        PXMemoryClear(startAdress, sizeDelta);
    }

#if PXMemoryDebug
    const unsigned char hasChanged = adress != adressReallocated;

    if (hasChanged)
    {
        printf("[#][Memory] 0x%p (%10zi B) Reallocate to 0x%p\n", adress, sizeAfter, adressReallocated);
    }
    else
    {
        printf("[#][Memory] 0x%p (%10zi B) Reallocate (No Change)\n", adress, sizeAfter);
    }
#endif

    return adressReallocated;
}

void* PXMemoryHeapReallocateTypeClear(void* const adress, const PXSize objectSize, const PXSize numberOfElementsBefore, const PXSize numberOfElementsAfter)
{
    return PXMemoryHeapReallocateClear(adress, objectSize * numberOfElementsBefore, objectSize * numberOfElementsAfter);
}

*/
