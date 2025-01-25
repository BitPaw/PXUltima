#include "PXMemory.h"

#include <Math/PXMath.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Console/PXConsole.h>
#include <Media/PXText.h>
#include <OS/Debug/PXDebug.h>

#include <stdlib.h>
#include <malloc.h>

#if OSUnix

#include <sys/mman.h>
#include <alloca.h>

#define ProtectionIDRead PROT_READ
#define ProtectionIDWrite PROT_WRITE
#define ProtectionIDReadWrite PROT_READ | PROT_WRITE

#elif OSWindows

#include <Windows.h>
#include <crtdbg.h>

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
#include <Container/PXDictionary/PXDictionary.h>
#include <Time/Time.h>

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
    PXDebugModuleHandleToName(symbolMemory->ModuleAdress, pxSymbol->NameModule);

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

void* PXAPI PXMemoryCalloc(const PXSize amount, const PXSize objectSize)
{
    void* adress = PXNull;

    const PXSize totalAmount = amount * objectSize;

    if(!totalAmount)
    {
        return PXNull;
    }


#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    adress = calloc(amount, objectSize);
#elif OSWindows
    const HANDLE heapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h
    adress = HeapAlloc(heapHandle, HEAP_ZERO_MEMORY, totalAmount); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#else
#error Memory allocate seems not to be supported on this OS
#endif


    // Special logging behaviour
    {
        PXSymbolMemory pxSymbolMemory;
        pxSymbolMemory.Adress = adress;
        pxSymbolMemory.Amount = amount;
        pxSymbolMemory.ObjectSize = objectSize;

        PXSymbol pxSymbol;

        PXDebug* pxDebug = PXDebugInstanceGet();

        PXDebugStackTrace(pxDebug, &pxSymbol, 1, 2, 1);

        pxSymbolMemory.ModuleAdress = pxSymbol.ModuleAdress;
        PXTextCopyA(pxSymbol.NameFile, 64, pxSymbolMemory.FileAdress, 64);
        PXTextCopyA(pxSymbol.NameSymbol, 64, pxSymbolMemory.FunctionAdress, 64);
        pxSymbolMemory.LineNumber = pxSymbol.LineNumber;

        PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingAllocation,
            "PX",
            "Memory-Calloc",
            "%s::%s::%s::%i, %ix %i B @ <%p>",
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber,
            pxSymbolMemory.Amount,
            pxSymbolMemory.ObjectSize,
            adress
        );
#endif
    }

    return adress;
}

void* PXAPI PXMemoryMalloc(const PXSize memorySize)
{
    void* adress = PXNull;

    if(!memorySize)
    {
        return PXNull;
    }

#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    adress = malloc(memorySize);
#elif OSWindows
    const HANDLE heapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h
    adress = HeapAlloc(heapHandle, 0, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
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

        PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeAdd);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingAllocation,
            "PX",
            "Memory-Alloc",
            "%s::%s::%s::%i, %7ix %i B @ <%p>",
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber,
            pxSymbolMemory.Amount,
            pxSymbolMemory.ObjectSize,
            adress
        );
#endif
    }

    return adress;
}

PXBool PXAPI PXMemoryFree(const void* const adress)
{
    if(!adress)
    {
        return PXFalse;
    }

#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    free(adress);
#elif OSWindows

    const HANDLE heapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h
    const PXBool freeResult = HeapFree(heapHandle, 0, adress); // Windows 2000 SP4, Kernel32.dll, heapapi.h


    // Special logging behaviour
    {
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

        PXMemorySymbolAdd(&pxSymbolMemory, PXMemorySymbolInfoModeRemove);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingAllocation,
            "PX",
            "Memory-Free",
            "%s::%s::%s::%i, %i B",
            pxSymbol.NameModule,
            pxSymbol.NameFile,
            pxSymbol.NameSymbol,
            pxSymbolMemory.LineNumber,
            pxSymbolMemory.Amount
        );
#endif
    }

    return freeResult;

#else
#error Memory release seems not to be supported on this OS
#endif
}

void* PXAPI PXMemoryRealloc(const void* const adress, const PXSize memorySize)
{
    void* newAdress = PXNull;
    PXBool updatedLocation = PXFalse;

#if OSUnix || MemoryUseSystemFunction || OSForcePOSIXForWindows
    // Function allows NULL as an adress
    newAdress = realloc(adress, memorySize);
#elif OSWindows

    if(!adress)
    {
        void* memory = PXMemoryMalloc(memorySize);

        PXMemorySet(memory, '°', memorySize);

        return memory;
    }

    const HANDLE heapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h

    // const PXSize oldSize = HeapSize(heapHandle, 0, adress);

    newAdress = HeapReAlloc(heapHandle, HEAP_ZERO_MEMORY, adress, memorySize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
    updatedLocation = newAdress != adress;

    // PXMemorySet(newAdress, '°', memorySize - oldSize);

#if 0
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

#if PXLogEnable && 0
        if(updatedLocation)
        {
            PXLogPrint
            (
                PXLoggingReallocation,
                "PX",
                "Memory-Realloc",
                "%s::%s::%s::%i, %i B",
                pxSymbol.NameModule,
                pxSymbol.NameFile,
                pxSymbol.NameSymbol,
                pxSymbolMemory.LineNumber,
                pxSymbolMemory.Amount
            );

        }
        else
        {
            PXLogPrint
            (
                PXLoggingReallocation,
                "PX",
                "Memory-Realloc",
                "%s::%s::%s::%i, %i B (No Move)",
                pxSymbol.NameModule,
                pxSymbol.NameFile,
                pxSymbol.NameSymbol,
                pxSymbolMemory.LineNumber,
                pxSymbolMemory.Amount
            );
        }
#endif
    }
#endif

    return newAdress;

#else
#error Memory reallocate seems not to be supported on this OS
#endif
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
    void* adress = PXNull;

    PXNewStackList(char, size, &adress, PXNull);

    PXMemoryCopy(bufferA, size, adress, size);
    PXMemoryCopy(bufferB, size, bufferA, size);
    PXMemoryCopy(adress, size, bufferB, size);

    PXDeleteStackList(char, size, adress, PXNull);

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

    if(bufferSize == 0)
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

#if MemoryUseSystemFunction
    memcpy(outputBuffer, inputBuffer, bufferSize);
#else
    for (PXSize index = bufferSize ; index ; --index)
    {
        ((char volatile*)outputBuffer)[index-1] = ((char volatile*)inputBuffer)[index-1];
    }
#endif

    return bufferSize;
}

PXSize PXAPI PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
    const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

#if MemoryUseSystemFunction
    memmove(outputBuffer, inputBuffer, bufferSize);
#else
    // Need a solution for a copy without a variable
    //memmove(outputBuffer, inputBuffer, bufferSize);

    for(size_t i = 0; i < bufferSize; i++)
    {
        char volatile* a = &((char volatile*)outputBuffer)[i];
        char volatile* b = &((char volatile*)inputBuffer)[i];

        *b = *a;
    }
#endif

    return bufferSize;
}

PXActionResult PXAPI PXMemoryHeapAllocate(PXMemoryInfo* const pxMemoryInfo)
{
    if(!pxMemoryInfo)
    {
        return PXActionRefusedArgumentNull;
    }

    if(!pxMemoryInfo->Data)
    {
        return PXActionRefusedArgumentNull;
    }

    const PXSize allocationSize = pxMemoryInfo->TypeSize * pxMemoryInfo->Amount;
    void* allocatedMemory = PXNull;

    if(!allocationSize)
    {
        return PXActionRefusedArgumentInvalid;
    }

    // Try allocate
    {
#if OSUnix

        if(pxMemoryInfo->MemoryClear)
        {
            allocatedMemory = calloc(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize);
        }
        else
        {
            allocatedMemory = malloc(allocationSize);
        }

#elif OSWindows

#if MemoryUseSystemFunction

#if 1
        if(pxMemoryInfo->MemoryClear)
        {
            allocatedMemory = calloc(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize);
        }
        else
        {
            allocatedMemory = malloc(allocationSize);
        }
#else

        if(!pxMemoryInfo->MemoryClear)
        {
            allocatedMemory = _malloc_dbg(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize, _NORMAL_BLOCK, pxMemoryInfo->File, pxMemoryInfo->Line); // crtdbg.h
        }
        else
        {
            allocatedMemory = _calloc_dbg(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize, _NORMAL_BLOCK, pxMemoryInfo->File, pxMemoryInfo->Line); // crtdbg.h
        }
#endif


#else
        const HANDLE healHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h
        const DWORD flags = pxMemoryInfo->MemoryClear * HEAP_ZERO_MEMORY;
        allocatedMemory = HeapAlloc(healHandle, flags, allocationSize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#endif

#endif
    }


    if(!allocatedMemory)
    {
        // Allocation failed!
        return PXActionFailedMemoryAllocation;
    }

    *pxMemoryInfo->Data = allocatedMemory;

    if(pxMemoryInfo->SizeTotal > 0)
    {
        *pxMemoryInfo->SizeTotal = allocationSize;
    }

#if PXLogEnable && PXMemoryDebug && 0
    PXLoggingEventData pxLoggingEventData;
    pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
    pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
    pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
    pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
    pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
    pxLoggingEventData.ModuleSource = "Memory";
    pxLoggingEventData.ModuleAction = "Heap-Alloc";
    pxLoggingEventData.PrintFormat = "";
    pxLoggingEventData.Type = PXLoggingAllocation;
    pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

    PXLogPrintInvoke(&pxLoggingEventData);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMemoryHeapDeallocate(PXMemoryInfo* const pxMemoryInfo)
{
    if(!pxMemoryInfo)
    {
        return PXFalse;
    }

#if PXLogEnable && PXMemoryDebug && 0
    PXLoggingEventData pxLoggingEventData;
    PXClear(PXLoggingEventData, &pxLoggingEventData);
    pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
    pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
    pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
    pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
    pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
    pxLoggingEventData.ModuleSource = "Memory";
    pxLoggingEventData.ModuleAction = "Heap-Free";
    pxLoggingEventData.PrintFormat = "";
    pxLoggingEventData.Type = PXLoggingDeallocation;
    pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

    PXLogPrintInvoke(&pxLoggingEventData);
#endif

#if MemorySanitise
    MemorySet(adress, '#', size);
#endif

#if MemoryDebugLeakDetection
    PXDictionaryRemove(&_memoryAdressLookup, adress);
#endif

    PXMemoryFree(*pxMemoryInfo->Data);

    *pxMemoryInfo->Data = PXNull;

    if(pxMemoryInfo->SizeTotal)
    {
        *pxMemoryInfo->SizeTotal = 0;
    }


    return PXActionSuccessful;
}

PXBool PXAPI PXMemoryHeapReallocate(PXMemoryHeapReallocateEventData* const pxMemoryHeapReallocateInfo)
{
    const PXSize sizeToAllocate = pxMemoryHeapReallocateInfo->AmountDemand * pxMemoryHeapReallocateInfo->TypeSize;




    PXSize oldSize = 0;
    void* adressOld = PXNull;

    if(pxMemoryHeapReallocateInfo->DataSize)
    {
        oldSize = *pxMemoryHeapReallocateInfo->DataSize;
    }

    if(pxMemoryHeapReallocateInfo->DataAdress)
    {
        adressOld = *pxMemoryHeapReallocateInfo->DataAdress;
    }



    // Need realloc?
    const PXSize isEnoughSpace = oldSize >= sizeToAllocate;

    // If there is enough space and we dont want to minimize size, we dont need to realloc
    if(isEnoughSpace && !pxMemoryHeapReallocateInfo->ReduceSizeIfPossible)
    {
        return PXTrue;
    }




    void* adressNew = PXNull;
    const PXSize beforeSize = oldSize;


#if OSUnix || MemoryUseSystemFunction
    adressNew = realloc(adressOld, sizeToAllocate);
#else
    const HANDLE heapHandle = GetProcessHeap();
    const PXBool hasNotBeenAllocated = adressOld == PXNull;

    if(hasNotBeenAllocated)
    {
        adressNew =    HeapAlloc(heapHandle, 0, sizeToAllocate);
    }
    else
    {
        adressNew = HeapReAlloc(heapHandle, 0, adressOld, sizeToAllocate);
    }

#endif

    if (!adressNew)
    {
        pxMemoryHeapReallocateInfo->WasSuccessful = PXFalse;

        return PXFalse;
    }

    pxMemoryHeapReallocateInfo->FreshAllocationPerformed = PXNull == *pxMemoryHeapReallocateInfo->DataAdress;

    pxMemoryHeapReallocateInfo->WasDataMoved = adressNew != pxMemoryHeapReallocateInfo->DataAdress;
    *pxMemoryHeapReallocateInfo->DataAdress = adressNew;
    *pxMemoryHeapReallocateInfo->DataSize = sizeToAllocate;

    if (beforeSize == 0)
    {
        pxMemoryHeapReallocateInfo->WasSizeIncreased = PXTrue;
    }
    else
    {
        pxMemoryHeapReallocateInfo->WasSizeIncreased = pxMemoryHeapReallocateInfo->AmountDemand > *pxMemoryHeapReallocateInfo->AmountCurrent;
    }


    pxMemoryHeapReallocateInfo->WasSuccessful = PXTrue;

    if (pxMemoryHeapReallocateInfo->AmountCurrent)
    {
        *pxMemoryHeapReallocateInfo->AmountCurrent = pxMemoryHeapReallocateInfo->AmountDemand;
    }

    // Is we have new space and we want to fill it, do now.
    if (pxMemoryHeapReallocateInfo->FreshAllocationPerformed || pxMemoryHeapReallocateInfo->WasSizeIncreased)
    {
        pxMemoryHeapReallocateInfo->PointOfNewData = &((char*)adressNew)[beforeSize];
        pxMemoryHeapReallocateInfo->PointOfNewDataSize = sizeToAllocate - beforeSize;

        if (pxMemoryHeapReallocateInfo->DoFillNewSpace)
        {
            PXMemorySet
            (
                pxMemoryHeapReallocateInfo->PointOfNewData,
                pxMemoryHeapReallocateInfo->FillSymbol,
                pxMemoryHeapReallocateInfo->PointOfNewDataSize
            );
        }
    }
    else
    {
        pxMemoryHeapReallocateInfo->PointOfNewData = 0;
        pxMemoryHeapReallocateInfo->PointOfNewDataSize = 0;
    }

#if PXLogEnable && PXMemoryDebug
    PXLoggingEventData pxLoggingEventData;
    PXClear(PXLoggingEventData, &pxLoggingEventData);
    pxLoggingEventData.MemoryData.TypeSize = pxMemoryHeapReallocateInfo->TypeSize;
    pxLoggingEventData.MemoryData.Amount = pxMemoryHeapReallocateInfo->AmountDemand;
    pxLoggingEventData.MemoryData.NameFile = pxMemoryHeapReallocateInfo->CodeFileName;
    pxLoggingEventData.MemoryData.NameFunction = pxMemoryHeapReallocateInfo->CodeFunctionName;
    pxLoggingEventData.MemoryData.NumberLine = pxMemoryHeapReallocateInfo->CodeFileLine;
    pxLoggingEventData.ModuleSource = "Memory";
    pxLoggingEventData.ModuleAction = "Heap-Realloc";
    pxLoggingEventData.PrintFormat = "";
    pxLoggingEventData.Type = PXLoggingReallocation;
    pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

    PXLogPrintInvoke(&pxLoggingEventData);
#endif

    return adressNew != 0;
}

void* PXAPI PXMemoryVirtualAllocate(const PXSize size, const PXAccessMode pxAccessMode)
{
    void* addressAllocated = PXNull;
    void* addressPrefered = 0;
    // const PXAccessMode protectionModeID = PXAccessModeFromID(PXAccessMode);

#if OSUnix && 0
    const int flags = MAP_PRIVATE;// | MAP_ANONYMOUS; | MAP_POPULATE; // missing on linux?
    const int fileDescriptor = -1;
    const off_t offset = 0;

    addressAllocated = mmap
    (
        (void*)addressPrefered,
        size,
        protectionModeID,
        flags,
        fileDescriptor,
        offset
    );
    const PXActionResult mapResult = PXErrorCurrent(MAP_FAILED != addressAllocated);

    if(PXActionSuccessful != mapResult)
    {
        return mapResult;
    }

#elif OSWindows
    DWORD allocationType = MEM_COMMIT | MEM_RESERVE;


#if 0 //MemoryPageLargeEnable
    // Check if large pages can be used
    {
        const PXSize largePageMinimumSize = GetLargePageMinimum(); // [Kernel32.dll] OS minimum: Windows Vista
        const PXBool hasLargePageSupport = largePageMinimumSize != 0u;

        const PXBool useLargeMemoryPages = hasLargePageSupport && (size > (largePageMinimumSize * 0.5f)); // if the allocation is atleah half of a big page, use that.

        if (useLargeMemoryPages)
        {
            const PXSize newSize = largePageMinimumSize * ((size / largePageMinimumSize) + 1);

            //PXActionResult actionResult = WindowsProcessPrivilege(L"SeLockMemoryPrivilege", TRUE);

            allocationType |= SEC_LARGE_PAGES;

            PXActionResult actionResult = WindowsProcessPrivilege(L"SeLockMemoryPrivilege", TRUE);

            printf("[i][Memory] Using large page.. size increased %i -> %i\n", (unsigned int)size, (unsigned int)newSize);

            size = newSize;
        }
    }
#endif

    addressAllocated = VirtualAlloc(addressPrefered, size, allocationType, MEM_COMMIT); // Windows XP (+UWP), Kernel32.dll, memoryapi.h
    const PXActionResult pxActionResult = PXErrorCurrent(!addressAllocated);

    if(PXActionSuccessful != pxActionResult)
    {
        return PXNull; // pxActionResult; // TODO: no error code return?
    }

#endif

#if PXMemoryDebug
    const char* readMode;

    switch(pxAccessMode)
    {
    case PXAccessModeWriteOnly:
        readMode = "Write only";
        break;

    case PXAccessModeReadOnly:
        readMode = "Read only";
        break;

    case PXAccessModeReadAndWrite:
        readMode = "Read & Write";
        break;

    default:
        readMode = "???";
        break;
    }

#if PXLogEnable
    PXLoggingEventData pxLoggingEventData;
    PXClear(PXLoggingEventData, &pxLoggingEventData);
    pxLoggingEventData.MemoryData.TypeSize = size;
    pxLoggingEventData.MemoryData.Amount = 1;
    pxLoggingEventData.ModuleSource = "Memory";
    pxLoggingEventData.ModuleAction = "Virtual-Alloc";
    pxLoggingEventData.PrintFormat = "  0x%p [%s]";
    pxLoggingEventData.Type = PXLoggingAllocation;
    pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

    PXLogPrintInvoke(&pxLoggingEventData, addressAllocated, readMode);
#endif
#endif

    return (void*)addressAllocated;
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
        "Memory",
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

    if (newAllocation)
    {
        return PXMemoryVirtualAllocate(size, PXAccessModeReadAndWrite);
    }

    return 0;
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

    return PXMemoryHeapAllocate(pxMemoryInfo);

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

    return PXMemoryHeapDeallocate(pxMemoryInfo);

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
