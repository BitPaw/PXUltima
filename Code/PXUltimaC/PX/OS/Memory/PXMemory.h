#ifndef PXMemoryINCLUDE
#define PXMemoryINCLUDE

#include <PX/Media/PXType.h>
#include <PX/Compiler/PXCompilerSettings.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/Container/Dictionary/PXDictionary.h>

typedef struct PXSymbol_ PXSymbol;


//---<Settings>---
#define MemorySizeUnkown -1
#define MemoryAssertEnable 0
#define PXMemoryDebug 1
#define MemoryDebugLeakDetection 0
#define PXMemoryUseFunctionSTD 0
#define PXMemoryUseFunctionOS 1
#define MemorySanitise 0
//----------------


//----------------------------------------------------------
// Access permissiom
//----------------------------------------------------------
#define PXAccessREAD      (1<<0)
#define PXAccessWRITE     (1<<1)
#define PXAccessEXECUTE   (1<<2)

#define PXAccessNone            0
#define PXAccessReadOnly        PXAccessREAD
#define PXAccessWriteOnly       PXAccessWRITE
#define PXAccessReadWrite       PXAccessReadOnly | PXAccessWriteOnly
#define PXAccessReadExecute     PXAccessEXECUTE | PXAccessReadOnly
#define PXAccessWriteExecute    PXAccessEXECUTE | PXAccessWriteOnly
#define PXAccessFull            PXAccessEXECUTE | PXAccessReadWrite

typedef enum PXAccessMode_
{
    PXAccessModeNoAccess     = PXAccessNone,
    PXAccessModeReadOnly     = PXAccessReadOnly,
    PXAccessModeWriteOnly    = PXAccessWriteOnly,
    PXAccessModeReadAndWrite = PXAccessReadWrite,
    PXAccessModeReadExecute  = PXAccessReadExecute,
    PXAccessModeWriteExecute = PXAccessWriteExecute,
    PXAccessModeFull         = PXAccessFull
}
PXAccessMode;
//----------------------------------------------------------

PXPublic void PXAPI PXAccessModeToStringA(char* text, const PXAccessMode pxAccessMode);



typedef enum PXMemoryCachingMode_
{
    PXMemoryCachingModeDefault,

    PXMemoryCachingModeRandom,  // Access data in a random order.
    PXMemoryCachingModeSequential, // Data sequentially from lower offsets to higher offsets.
    PXMemoryCachingModeTemporary, // File will not be saves to drive.
    PXMemoryCachingModeUseOnce, // Access the specified data once and then not reuse it again.

    // Windows only
    PXMemoryCachingModeWriteThrough,
    PXMemoryCachingModeNoBuffering, // No OS Caching, direct to Harddrive if supprted

    // UNIX only
    PXMemoryCachingModeNeedLater, // Data is not needed right now. "For later"[near future].
    PXMemoryCachingModeDontNeedNow // Data will not be cached. "I dont need it yet"
}
PXMemoryCachingMode;

typedef struct PXMemoryUsage_
{
    PXSize PhysicalRAMSize;

    PXSize PercentInUse;
    PXSize PhysicalTotal;
    PXSize PhysicalAvailable;
    PXSize VirtualTotal;
    PXSize VirtualAvailable;
    PXSize PageFileTotal;
    PXSize PageFileAvailable;
    PXSize ExtendedVirtualAvailable;
}
PXMemoryUsage;

typedef struct PXMemoryInfo_
{
    // OUT
    void** Data;
    PXSize* SizeTotal;

    // Input
    PXSize Amount;
    PXSize TypeSize;

    // Debug Info
#if PXMemoryDebug
    const char* File;
    const char* Function;
    PXSize Line;
#endif

    PXBool MemoryClear;
}
PXMemoryInfo;




typedef struct PXMemoryHeapReallocateEventData_
{
    PXSize TypeSize;
    PXSize AmountDemand;
    PXSize* AmountCurrent;
    PXSize* DataSize;
    void** DataAdress;

    PXBool DoFillNewSpace;
    PXByte FillSymbol;
    PXBool ReduceSizeIfPossible;

    // Additinaional Flags
    PXBool FreshAllocationPerformed;
    PXBool WasSuccessful;
    PXBool WasDataMoved;

    // Data of added memory space
    void* PointOfNewData;
    PXSize PointOfNewDataSize;
    PXBool WasSizeIncreased;

#if PXMemoryDebug
    const char* CodeFileName;
    const char* CodeFunctionName;
    PXSize CodeFileLine;
#endif
}
PXMemoryHeapReallocateEventData;





#define PXSymbolMemoryStateAllocated
#define PXSymbolMemoryStateFreed

// A special debug symbol that is used for information
// about allocations. As the system does not track allocations
// we are in the need to create them ourselfs.
typedef struct PXSymbolMemory_
{
    void* Adress;
    void* ModuleAdress; // What EXE or DLL is the creator?
    char FileAdress[64]; // Calling adress
    char FunctionAdress[64]; // Name of the
    PXSize Amount;
    PXSize ObjectSize;
    PXSize LineNumber;
    //PXInt64U CreationTime;   // timestamps?



    // is it reallocated
    // Thread source? is it from another thread.
}
PXSymbolMemory;



typedef struct PXMemorySymbolLookup_
{
    PXDictionary SymbolLookup;
}
PXMemorySymbolLookup;

typedef enum PXMemorySymbolInfoMode_
{
    PXMemorySymbolInfoModeInvalid,
    PXMemorySymbolInfoModeAdd,
    PXMemorySymbolInfoModeUpdate,
    PXMemorySymbolInfoModeRemove
}
PXMemorySymbolInfoMode;


// Page file size, zero if not supported
typedef struct PXMemoryPageInfo_
{
    PXSize PageSizeNormal;
    PXSize PageSizeLarge;
    PXSize PageSizeHuge;

    PXSize PageAmountNormal;
    PXSize PageAmountLarge;
    PXSize PageAmountHuge;

    PXInt8U PageUtilizationNormal;
    PXInt8U PageUtilizationLarge;
    PXInt8U PageUtilizationHuge;
}
PXMemoryPageInfo;



typedef struct PXMemoryBlockInfo_
{
    int c;
}
PXMemoryBlockInfo;


typedef struct PXMemoryManager_
{
    int x;
}
PXMemoryManager;









// Global memory lookup.
// This is a ugly solution but due to the
// scattered calls of malloc and free, we
// need to have this global reference to compare to

// Singleton of a global memory table, all PXMemory allocations are regsieterd here
PXPrivate PXMemorySymbolLookup* PXAPI PXMemorySymbolLookupInstanceGet(void);
PXPublic void PXAPI PXMemorySymbolAdd(PXSymbolMemory* const pxSymbolMemory, const PXMemorySymbolInfoMode pxMemorySymbolInfoMode);
PXPublic PXActionResult PXAPI PXMemorySymbolFetch(const void* const adress, PXSymbol* const pxSymbol);


// Check if two pointers overlap, if they do, we can't use a memory copy function, instaead 
// we need to use memory move, because of optimisattion that results in copy corruption.
PXPublic PXBool PXAPI PXMemoryDoAdressesOverlap(void* const adressA, const PXSize adressALengh, void* const adressB, const PXSize adressBLengh);


//-----------------------------------------------------------------------------
// Heap - Space to allocate dynamic memory
//-----------------------------------------------------------------------------
typedef struct PXMemoryHeap_
{
    void* HeapHandle;
    PXSize Allocated;
}
PXMemoryHeap;

PXPublic PXActionResult PXAPI PXMemoryHeapCreate(PXMemoryHeap* const pxMemoryHeap);
PXPublic PXActionResult PXAPI PXMemoryHeapRelease(PXMemoryHeap* const pxMemoryHeap);
PXPublic void PXAPI PXMemoryHeapGetGlobal(PXMemoryHeap* const pxMemoryHeap);
//PXPublic PXMemoryHeap* PXAPI PXMemoryHeapGetPX(void);

// Get the size of the allocated block
PXPublic PXSize PXAPI PXMemoryHeapBlockSize(PXMemoryHeap* pxMemoryHeap, const void* const adress);

PXPublic void* PXAPI PXMemoryHeapCalloc(PXMemoryHeap* pxMemoryHeap, const PXSize amount, const PXSize objectSize); // [POSIX]
PXPublic void* PXAPI PXMemoryHeapMalloc(PXMemoryHeap* pxMemoryHeap, const PXSize memorySize); // [POSIX]
PXPublic PXBool PXAPI PXMemoryHeapFree(PXMemoryHeap* pxMemoryHeap, const void* const adress); // [POSIX]
PXPublic void* PXAPI PXMemoryHeapRealloc(PXMemoryHeap* pxMemoryHeap, const void* const adress, const PXSize memorySize); // [POSIX]

#define PXMemoryHeapCallocT(type, amount) (type*)PXMemoryHeapCalloc(PXNull, amount, sizeof(type))
#define PXMemoryHeapMallocT(type, amount) (type*)PXMemoryHeapMalloc(PXNull, sizeof(type) * amount)
#define PXMemoryHeapReallocT(type, adress, amount) (type*)PXMemoryHeapRealloc(PXNull, adress, sizeof(type) * amount)
//-----------------------------------------------------------------------------



PXPublic int PXAPI PXMemoryProtectionIDTranslate(const PXInt8U protectionMode);

PXPublic PXActionResult PXAPI PXMemoryProtect(void* dataAdress, const PXSize dataSize, const PXInt8U protectionMode);

PXPublic PXBool PXAPI PXMemoryScan(PXMemoryUsage* memoryUsage);

PXPublic inline const void* PXAPI PXMemoryLocateFirst(const void* const PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);
PXPublic inline const void* PXAPI PXMemoryLocateLast(const void* const PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);



// PXPublic char MemoryAdvice(const void* adress, const PXSize length, const FileCachingMode fileCachingMode);







#if OSUnix
#define _PX_FILENAME_ ((const char*)(PXMemoryLocateLast(__FILE__, '/', -1) ? (char*)PXMemoryLocateLast(__FILE__, '/', -1) + 1 : __FILE__))
#elif OSWindows
#define _PX_FILENAME_ ((const char*)(PXMemoryLocateLast(__FILE__, '\\', -1) ? (char*)PXMemoryLocateLast(__FILE__, '\\', -1) + 1 : __FILE__))
#endif





//PXPublic PXBool PXMemoryHeapResizeArray(PXSize typeSize, void** dataAddress, PXSize* const dataAddressSizeCurrent, const PXSize dataAddressSizeRequired);

//PXPublic void* PXMemoryHeapReallocateClear(void* const adress, const PXSize sizeBefore, const PXSize sizeAfter);
//PXPublic void* PXMemoryHeapReallocateTypeClear(void* const adress, const PXSize objectSize, const PXSize numberOfElementsBefore, const PXSize numberOfElementsAfter);


PXPublic void PXAPI PXMemoryPageInfoFetch(PXMemoryPageInfo* const pxFilePageFileInfo, const PXSize objectSize);


// Allocate memory in virtual memory space.
// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
// Only use for bigger datablocks as thic has very hi overhead.
PXPublic void* PXAPI PXMemoryVirtualAllocate(PXSize size, PXSize* const createdSize, const PXAccessMode pxAccessMode);
PXPublic void PXAPI PXMemoryVirtualPrefetch(const void* adress, const PXSize size);
PXPublic PXActionResult PXAPI PXMemoryVirtualRelease(const void* adress, const PXSize size);
PXPublic void* PXAPI PXMemoryVirtualReallocate(const void* adress, const PXSize size);




// Allocates size bytes on the program stack.
// The allocated space is automatically freed when the calling function exits
// (not when the allocation merely passes out of scope).
PXPublic PXActionResult PXAPI PXMemoryStackAllocate(PXMemoryInfo* const pxMemoryAllocateInfo);

// Deallocates stack allocated memory if it was commited to the heap.
// Additional size parameter can be ignored
PXPublic PXActionResult PXAPI PXMemoryStackDeallocate(PXMemoryInfo* const pxMemoryAllocateInfo);
//---------------------------------------------------------




//---------------------------------------------------------
// Copy
//---------------------------------------------------------
#pragma optimize( "", off )
PXPublic PXSize PXAPI PXMemoryCopy(const void* PXRestrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

#if PXMemoryDebug
#define PXCopy(type, source, target) PXMemoryCopy(source, sizeof(type), target, sizeof(type));
#define PXCopyList(type, amount, source, target) PXMemoryCopy(source, sizeof(type)*amount, target, sizeof(type)*amount);
#else
#define PXCopy(type, source, target) PXMemoryCopy(source, sizeof(type), target, sizeof(type));
#define PXCopyList(type, amount, source, target) PXMemoryCopy(source, sizeof(type)*amount, target, sizeof(type)*amount);
#endif
//---------------------------------------------------------

PXPublic void PXAPI PXMemoryCopyF16V(PXF16* const destination, const PXF16* const source, const PXSize amount);
PXPublic void PXAPI PXMemoryCopyF32V(PXF32* const destination, const PXF32* const source, const PXSize amount);
PXPublic void PXAPI PXMemoryCopyF64V(PXF64* const destination, const PXF64* const source, const PXSize amount);
PXPublic void PXAPI PXMemoryCopyF32ToF16V(PXF32* const destination, const PXF16* const source, const PXSize amount);
PXPublic void PXAPI PXMemoryCopyF16ToF32V(PXF16* const destination, const PXF32* const source, const PXSize amount);

//---------------------------------------------------------
// Clear
//---------------------------------------------------------
#pragma optimize( "", off )
PXPublic PXSize PXAPI PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

#if PXMemoryDebug
//#define PXMove(type, source, target) PXMemoryMove(adress, sizeof(type));
#else

#endif
//---------------------------------------------------------




//---------------------------------------------------------
// Clear
//---------------------------------------------------------
#pragma optimize( "", off )
PXPublic void PXAPI PXMemoryClear(void* const PXRestrict bufferA, const PXSize bufferASize);

#define PXClear(type, adress) PXMemoryClear(adress, sizeof(type));
#define PXClearList(type, adress, amount) PXMemoryClear(adress, sizeof(type) * amount);

#if PXMemoryDebug


#else

#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Set
//---------------------------------------------------------
#pragma optimize( "", off )
PXPublic void PXAPI PXMemorySetI32U(int* const PXRestrict bufferA, const int value, const PXSize amount);

#pragma optimize( "", off )
PXPublic void PXAPI PXMemorySet(void* const PXRestrict bufferA, const PXByte value, const PXSize bufferSize);
#define PXSet(type, adress, value) PXMemorySet(adress, value, sizeof(type));
#define PXSetList(type, adress, amount, value) PXMemorySet(adress, value, sizeof(type) * amount);

#if PXMemoryDebug

#else

#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Compare
//---------------------------------------------------------

// Compare a list of 4-byte text elements.
// Return the index of the first match
PXPublic PXInt8U PXAPI PXMemoryCompareC32V(const char value[4],  char* const textList[4], const PXInt8U listAmount);


PXPublic int PXAPI PXMemoryCompareThreeWay(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);


PXPublic PXBool PXAPI PXMemoryCompareToByte(const void* PXRestrict bufferA, const PXSize bufferASize, const PXByte dataByte);

// Returns 1 if correct, 0 if not.
// This function is not like memcmp that returns -1, 0, and 1!
PXPublic PXBool PXAPI PXMemoryCompare(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);

//---------------------------------------------------------

PXPublic PXBool PXAPI PXMemoryIsEmpty(const void* PXRestrict buffer, const PXSize bufferSize);
PXPublic PXBool PXAPI PXMemorySwap(void* PXRestrict bufferA, void* PXRestrict bufferB, const PXSize size);

#endif
