#include "PXMemory.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXText.h>
#include <PX/OS/PXOS.h>

const char PXMemoryCPUName[] = "CPU-SIMD";
const char PXMemoryCPUCompare[] = "Compare";

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
PXResult PXAPI WindowsProcessPrivilege(const char* pszPrivilege, BOOL bEnable)
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

        PXDictionaryConstruct(&_PXGLOBLALMemorySymbolLookup.SymbolLookup, sizeof(void*), sizeof(PXSymbolMemory), PXDictionaryValueLocalityInternalEmbedded);

        _PXGLOBLALMemorySymbolLookupENABLED = PXTrue;
    }

    return &_PXGLOBLALMemorySymbolLookup;
}

void PXAPI PXMemorySymbolAdd(PXSymbolMemory PXREF pxSymbolMemory,  const PXMemorySymbolInfoMode pxMemorySymbolInfoMode)
{
    PXMemorySymbolLookup PXREF pxMemorySymbolLookup = PXMemorySymbolLookupInstanceGet();

    switch(pxMemorySymbolInfoMode)
    {
        case PXMemorySymbolInfoModeAdd:
        {
            PXDictionaryEntryAdd(&pxMemorySymbolLookup->SymbolLookup, &pxSymbolMemory->Adress, pxSymbolMemory);
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

PXResult PXAPI PXMemorySymbolFetch(const void PXREF adress, PXSymbol PXREF pxSymbol)
{
    PXMemorySymbolLookup PXREF pxMemorySymbolLookup = PXMemorySymbolLookupInstanceGet();
    PXSymbolMemory* symbolMemory = PXNull;

    PXBool success = PXDictionaryEntryFind(&pxMemorySymbolLookup->SymbolLookup, &adress, &symbolMemory);

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

PXBool PXAPI PXMemoryDoAdressesOverlap(void PXREF adressA, const PXSize adressALengh, void PXREF adressB, const PXSize adressBLengh)
{
    // What adress is the lower one?
    // We could force the user to only use a as a lower adress but not right now

    PXSize adressLowerSize = 0;
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

    PXBool overlap = ((PXSize)adressLower + adressLowerSize) >= (PXSize)adressHigher;

    return overlap;
}




int PXAPI PXMemoryProtectionIDTranslate(const PXI8U protectionMode)
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

PXResult PXAPI PXMemoryProtect(void* dataAdress, const PXSize dataSize, const PXI8U protectionMode)
{
    const PXI32U protectionID = PXMemoryProtectionIDTranslate(protectionMode);

#if OSUnix
    const int protectResultID = mprotect(dataAdress, dataSize, protectionID); // libc.so, sys/mman.h
    const PXResult protectResult = PXErrorCurrent(0 == protectResultID);

    return protectResult;
#elif OSWindows

    DWORD oldProtectModeID = 0;

    const PXBool result = VirtualProtect(dataAdress, dataSize, protectionID, &oldProtectModeID); // Windows XP (+UWP), Kernel32.dll, memoryapi.h
    const PXResult actiobResult = PXErrorCurrent(result);

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

void PXAPI PXMemoryClear(void PXREF PXRestrict bufferA, const PXSize bufferASize)
{
    PXMemorySet(bufferA, 0u, bufferASize);

    //ZeroMemory(bufferA, bufferASize);
}

void PXAPI PXMemorySetI32U(int PXREF PXRestrict bufferA, const int value, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        ((int volatile*)bufferA)[i] = value;
    }
}

void PXAPI PXMemorySet(void PXREF PXRestrict buffer, const PXByte value, const PXSize bufferSize)
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

PXI8U PXAPI PXMemoryCompareI8V(const PXI8U PXREF textList, const PXI8U listAmount, const PXI8U value)
{
    __m512i zero = _mm512_setzero_si512();
    //__m512i zero = _mm512_set1_epi8('~');

    __m512i value_vector = _mm512_set1_epi8(value); // Load target byte gets copy'ed 64x 

    for(PXI8U i = 0; i < listAmount; i += 64)
    { 
        __mmask64 mask = ((listAmount -i) > 64) ? 0xFFFFFFFFFFFFFFFF : (1LL << (listAmount - (i))) -1; // How many things can we load?
        __m512i data_vector = _mm512_mask_loadu_epi8(zero, mask, &textList[i]); // Load compare array

        const PXI64U result = _mm512_cmp_epi8_mask(value_vector, data_vector, _MM_CMPINT_EQ); // Compare both 64x byte vs byte

        char bufferA[64 * 2+1];
        char bufferB[64 * 2+1];

        for(size_t i = 0; i < 64; ++i)
        {
            bufferA[2 * i + 0] = value_vector.m512i_u8[i];
            bufferA[2 * i + 1] = ' ';
            bufferA[2 * i + 2] = 0;            

            bufferB[2 * i + 0] = PXTextMakePrintable(data_vector.m512i_u8[i]);
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
        const PXI8U match_index = 63 - _lzcnt_u64(result); // Count leading zeros. We want the first one.
#else
        const PXI8U match_index = 63 - _lzcnt_u32(result); // Count leading zeros. We want the first one.
#endif
 

        return i + match_index; // Index of first hit
    }

    return (PXI8U)-1; // No match!
}

PXI8U PXAPI PXMemoryCompareC32V(const char* value, char PXREF textList[4], const PXI8U listAmount)
{
    __m512i value_vector = _mm512_set1_epi32(*(int*)value); // Load target char[4], gets copy'ed 16x 

    for(PXI8U i = 0; i < listAmount; i += 16)
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

    return (PXI8U)-1; // No match!
}

PXSize PXAPI PXMemoryCompareI32UV(const PXI32U* valueList, const PXSize listAmount, const PXI32U valueTarget)
{
    PXSize i = 0;

    for(; i < listAmount; ++i)
    {
        if(valueList[i] == valueTarget)
        {
            return i;
        }
    }

    return (PXSize)-1;
}

PXI8U PXAPI PXMemoryCompareSVI8(const char PXREF stringTarget, char* PXREF stringList, const PXI8U PXREF stringSizeList, const PXI8U amount)
{
    // Load the target string into a 512-bit AVX512 register
    __m512i target = _mm512_set1_epi64(*(PXI64U*)stringTarget); // Mask for max 8 bytes
    __m512i currentString = _mm512_setzero_si512();

    PXSize offset = 0;

    for(size_t roundIndex = 0; roundIndex < amount; ++roundIndex)
    {
        // Upload strings
        for(PXI8U batchIndex = 0; batchIndex < 8; ++batchIndex)
        {
            char* stringSource = stringList[(roundIndex * 8) +batchIndex];
            PXI8U stringLength = stringSizeList[(roundIndex*8) + batchIndex];


            // Load the current string in the list into another AVX512 register
           // __m512i currentString = _mm512_maskz_loadu_epi8(0xFF, stringList[i]);
            PXI64U mask = (1LLu << stringLength) -1;

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
            PXMemoryCPUName,
            "Compare",
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
                PXMemoryCPUName,
                PXMemoryCPUCompare,
                "No match in this batch (%i/%i)",
                roundIndex+1,
                amount/8
            );

            continue;
        }

        const PXI64U match_index = 31 - _lzcnt_u32(resultMask); // Count leading zeros. We want the first one.
        const PXI8U index = (roundIndex *8)+ match_index;

        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryCPUName,
            PXMemoryCPUCompare,
            "Match found! index:<%i>",
            index
        );

        return index;
    }    


    PXLogPrint
    (
        PXLoggingAllocation,
        PXMemoryCPUName,
        PXMemoryCPUCompare,
        "Absolutly no match found"
    );

    // Return a sentinel value if no match is found
    return (PXI8U)-1;

}

PXI8U PXAPI PXMemoryReadBitI32U(const PXI32U PXREF value, const PXI8U index)
{
    return _bittest((LONG*)value, index); // [Intrinsic] intrin.h, winnt.h
}

PXI8U PXAPI PXMemoryReadBitI64U(const PXI64U PXREF value, const PXI8U index)
{
    return 0;// _bittest64(PXI64U, index);
}

PXI8U PXAPI PXMemoryReadBitAndClearI32U(const PXI32U PXREF PXI32U, const PXI8U index)
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

        PXText pxA;
        PXText pxTextB;
        PXTextConstructNamedBufferA(&pxA, pxABuffer, 16);
        PXTextConstructNamedBufferA(&pxTextB, pxTextBBuffer, 16);
        PXTextFormatData(&pxA, bufferA, bufferSize);
        PXTextFormatData(&pxTextB, bufferB, bufferSize);

        PXLogPrint
        (
            PXLoggingInfo,
            "Memory",
            "Compare",
            "%6zi B  <%s> == <%s>",
            bufferSize,
            pxA.A,
            pxTextB.A
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

    PXMemoryCopy(bufferA, adress, size);
    PXMemoryCopy(bufferB, bufferA, size);
    PXMemoryCopy(adress, bufferB, size);

    PXMemoryHeapFree(PXNull, adress);

    return PXTrue;
}

const void* PXAPI PXMemoryLocateFirst(const void PXREF PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize)
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

const void* PXAPI PXMemoryLocateLast(const void PXREF PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize)
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

void PXAPI PXMemoryCopyF16V(PXF16 PXREF destination, const PXF16 PXREF source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF32V(PXF32 PXREF destination, const PXF32 PXREF source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF64V(PXF64 PXREF destination, const PXF64 PXREF source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF32ToF16V(PXF32 PXREF destination, const PXF16 PXREF source, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        destination[i] = source[i];
    }
}

void PXAPI PXMemoryCopyF16ToF32V(PXF16 PXREF destination, const PXF32 PXREF source, const PXSize amount)
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

void PXAPI PXMemoryPageInfoFetch(PXMemoryPageInfo PXREF pxFilePageFileInfo, const PXSize objectSize)
{
    PXOS PXREF pxOS = PXSystemGet();

    PXClear(PXMemoryPageInfo, pxFilePageFileInfo);

    pxFilePageFileInfo->PageSizeNormal = pxOS->PageSizeNormal;
    pxFilePageFileInfo->PageSizeLarge = pxOS->PageSizeLarge;
    pxFilePageFileInfo->PageSizeHuge = pxOS->PageSizeHuge;
    pxFilePageFileInfo->PageSizePhysical = pxOS->PageSizePhysical;

    // Calc the size
    if(pxFilePageFileInfo->PageSizeNormal > 0)
    {
        pxFilePageFileInfo->PageAmountNormal = (objectSize / pxFilePageFileInfo->PageSizeNormal + 1);
        pxFilePageFileInfo->PageUtilizationNormal = (objectSize * 100) / (pxFilePageFileInfo->PageSizeNormal * pxFilePageFileInfo->PageAmountNormal);
    }

    if(pxFilePageFileInfo->PageSizePhysical > 0)
    {
        pxFilePageFileInfo->PageAmountPhysical = objectSize / pxFilePageFileInfo->PageSizePhysical + 1;
        pxFilePageFileInfo->PageUtilizationPhysical = (objectSize * 100) / (pxFilePageFileInfo->PageSizePhysical * pxFilePageFileInfo->PageAmountPhysical);
    }

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


    // Calc affinity


    const PXBool usePagesPhysical = pxFilePageFileInfo->PageAmountPhysical > 1;
    const PXBool usePagesLarge = pxFilePageFileInfo->PageAmountLarge > 1;
    const PXBool usePagesHuge = pxFilePageFileInfo->PageAmountHuge > 1;

    pxFilePageFileInfo->Affinity = PXMemoryPageNormal;

    if(usePagesPhysical)
    {
        pxFilePageFileInfo->Affinity = PXMemoryPagePhysical;
    }

    if(usePagesLarge)
    {
        pxFilePageFileInfo->Affinity = PXMemoryPageLarge;
    }

    if(usePagesHuge)
    {
        pxFilePageFileInfo->Affinity = PXMemoryPageHuge;
    }

}



#define PXMemoryUseStackAllocation 0

PXResult PXAPI PXMemoryStackAllocate(PXMemoryInfo PXREF pxMemoryInfo)
{
#if PXMemoryUseStackAllocation
    const PXSize totalSize = pxMemoryInfo->TypeSize * pxMemoryInfo->Amount;
    void PXREF stackAllocated =

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

PXResult PXAPI PXMemoryStackDeallocate(PXMemoryInfo PXREF pxMemoryInfo)
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

PXSize PXAPI PXMemoryCopy(const void* PXRestrict const inputBuffer, void* PXRestrict const outputBuffer, const PXSize bufferSize)
{
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
    for(PXSize index = bufferSize; index; --index)
    {
        ((char volatile*)outputBuffer)[index - 1] = ((char volatile*)inputBuffer)[index - 1];
    }
#endif

    return bufferSize;
}

PXSize PXAPI PXMemoryCopyX(const void* PXRestrict const inputBuffer, const PXSize inputBufferSize, void PXREF outputBuffer, const PXSize outputBufferSize)
{
    const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

    if(!(inputBuffer && outputBuffer && bufferSize > 0))
    {
        return 0;
    }

    return PXMemoryCopy(inputBuffer, outputBuffer, bufferSize);
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

    void PXREF adress = calloc(amount, objectSize);

#if PXMemoryDebug
    printf("[#][Memory] 0x%p (%10zi B) Allocate on heap cleared\n", adress, requestedSizeInBytes);
#endif

    return adress;
}

PXBool PXMemoryHeapResizeArray(PXSize typeSize, void** dataAddress, PXSize PXREF dataAddressSize, const PXSize dataAddressSizeRequired)
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
void* PXMemoryHeapReallocateClear(void PXREF sourceAddress, const PXSize sizeBefore, const PXSize sizeAfter)
{
    void PXREF adressReallocated = realloc(sourceAddress, sizeAfter);
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

void* PXMemoryHeapReallocateTypeClear(void PXREF adress, const PXSize objectSize, const PXSize numberOfElementsBefore, const PXSize numberOfElementsAfter)
{
    return PXMemoryHeapReallocateClear(adress, objectSize * numberOfElementsBefore, objectSize * numberOfElementsAfter);
}

*/
