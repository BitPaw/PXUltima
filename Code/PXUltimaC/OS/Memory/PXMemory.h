#ifndef PXMemoryINCLUDE
#define PXMemoryINCLUDE

#include <Media/PXType.h>
#include <Compiler/PXCompilerSettings.h>

#if OSUnix
typedef int PXMemoryAccessModeType;
#elif OSWindows
typedef unsigned long PXMemoryAccessModeType;// DWORD
#endif

//---<Settings>---
#define MemorySizeUnkown -1
#define MemoryAssertEnable 0
#define PXMemoryDebug 0
#define MemoryDebugLeakDetection 0
#define MemoryUseSystemFunction 1
#define MemorySanitise 0
//----------------

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXMemoryAccessMode_
	{
		PXMemoryAccessModeInvalid,
		PXMemoryAccessModeNoReadWrite,
		PXMemoryAccessModeReadOnly,
		PXMemoryAccessModeWriteOnly,
		PXMemoryAccessModeReadAndWrite
	}
	PXMemoryAccessMode;

	typedef enum PXMemoryCachingMode_
	{
		PXMemoryCachingModeInvalid,

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


	PXPublic PXBool PXMemoryScan(PXMemoryUsage* memoryUsage);





	PXPublic const void* PXMemoryLocate(const void* PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);

	


	//CPublic char MemoryAdvice(const void* adress, const PXSize length, const FileCachingMode fileCachingMode);

	// Allocates size bytes on the program stack.
	// The allocated space is automatically freed when the calling function exits
	// (not when the allocation merely passes out of scope).
	PXPublic void* PXMemoryStackAllocate(const PXSize size);
	PXPublic void* PXMemoryStackRelease(void* const adress);




	//PXPublic PXBool PXMemoryHeapResizeArray(PXSize typeSize, void** dataAddress, PXSize* const dataAddressSizeCurrent, const PXSize dataAddressSizeRequired);

	//PXPublic void* PXMemoryHeapReallocateClear(void* const adress, const PXSize sizeBefore, const PXSize sizeAfter);
	//PXPublic void* PXMemoryHeapReallocateTypeClear(void* const adress, const PXSize objectSize, const PXSize numberOfElementsBefore, const PXSize numberOfElementsAfter);



	// Allocate memory in virtual memory space.
	// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
	// Only use for bigger datablocks as thic has very hi overhead.
	PXPublic void* PXMemoryVirtualAllocate(PXSize size, const PXMemoryAccessMode PXMemoryAccessMode);
	PXPublic void PXMemoryVirtualPrefetch(const void* adress, const PXSize size);
	PXPublic void PXMemoryVirtualRelease(const void* adress, const PXSize size);
	PXPublic void* PXMemoryVirtualReallocate(const void* adress, const PXSize size);


	PXPublic PXMemoryAccessModeType PXMemoryAccessModeFromID(const PXMemoryAccessMode PXMemoryAccessMode);






//---------------------------------------------------------
// Allocation
//---------------------------------------------------------
#if PXMemoryDebug
	PXPublic void* PXMemoryHeapAllocateDetailed(const PXSize typeSize, const PXSize amount, const char* file, const char* function, const PXSize line);
#define PXNew(type) (type*)PXMemoryHeapAllocateDetailed(sizeof(type), 1, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#define PXNewList(type, amount) (type*)PXMemoryHeapAllocateDetailed(sizeof(type), amount, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#else
	PXPublic void* PXMemoryHeapAllocate(const PXSize amount, const PXSize typeSize);

#define PXNew(type) (type*)PXMemoryHeapAllocate(1u, sizeof(type))
#define PXNewList(type, amount) (type*)PXMemoryHeapAllocate(amount, sizeof(type))
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Deallocation
//---------------------------------------------------------
#if PXMemoryDebug
	PXPublic void PXMemoryReleaseDetailed(void* adress, const PXSize size, const char* file, const char* function, const PXSize line);

#define PXDelete(type, adress) PXMemoryReleaseDetailed(adress, sizeof(type), _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#define PXDeleteList(type, adress, amount) PXMemoryReleaseDetailed(adress, sizeof(type) * amount, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#else
	PXPublic void PXMemoryRelease(void* adress, const PXSize size);

#define PXDelete(type, adress) PXMemoryRelease(adress, sizeof(type))
#define PXDeleteList(type, adress, amount) PXMemoryRelease(adress, sizeof(type) * (PXInt32U)amount)
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Reallocation
//---------------------------------------------------------
#if PXMemoryDebug
	PXPublic PXBool PXMemoryHeapReallocateDetailed(void** const sourceAddress, PXSize* const currentSize, const PXSize requestedSize, const char* const file, const char* const function, const PXSize line);
#define PXResizeList(type, address, currentSize, requestedSize) PXMemoryHeapReallocateDetailed(address, currentSize, sizeof(type) * requestedSize, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#else
	PXPublic PXBool PXMemoryHeapReallocate(const PXSize typeSize, void** const sourceAddress, PXSize* const currentSize, const PXSize requestedSize);
#define PXResizeList(type, address, currentSize, requestedSize) PXMemoryHeapReallocate(sizeof(type), address, currentSize, requestedSize)
#endif	
//---------------------------------------------------------



//---------------------------------------------------------
// Guaranteesize
//---------------------------------------------------------
	PXPublic PXBool PXMemoryGuaranteeSize(const PXSize typeSize, void** const sourceAddress, PXSize* const currentSize, const PXSize requestedSize);

#define PXGuaranteeSize(type, address, currentSize, requestedSize) PXMemoryGuaranteeSize(sizeof(type), address, currentSize, requestedSize);
//---------------------------------------------------------



//---------------------------------------------------------
// Copy
//---------------------------------------------------------
	PXPublic PXSize PXMemoryCopy(const void* PXRestrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

#if PXMemoryDebug
#define PXCopy(type, source, target) PXMemoryCopy(source, sizeof(type), target, sizeof(type));
#define PXCopyList(type, amount, source, target) PXMemoryCopy(source, sizeof(type)*amount, target, sizeof(type)*amount);
#else
#define PXCopy(type, source, target) PXMemoryCopy(source, sizeof(type), target, sizeof(type));
#define PXCopyList(type, amount, source, target) PXMemoryCopy(source, sizeof(type)*amount, target, sizeof(type)*amount);
#endif	
//---------------------------------------------------------



//---------------------------------------------------------
// Clear
//---------------------------------------------------------
	PXPublic PXSize PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

#if PXMemoryDebug
		

//#define PXMove(type, source, target) PXMemoryMove(adress, sizeof(type));
#else

#endif
//---------------------------------------------------------




//---------------------------------------------------------
// Clear
//---------------------------------------------------------
	PXPublic void PXMemoryClear(void* const PXRestrict bufferA, const PXSize bufferASize);

#define PXClear(type, adress) PXMemoryClear(adress, sizeof(type));
#define PXClearList(type, adress, amount) PXMemoryClear(adress, sizeof(type) * amount);

#if PXMemoryDebug


#else

#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Set
//---------------------------------------------------------
	PXPublic void PXMemorySet(void* const PXRestrict bufferA, const PXByte value, const PXSize bufferSize);
#define PXSet(type, adress, value) PXMemorySet(adress, value, sizeof(type));
#define PXSetList(type, adress, amount, value) PXMemorySet(adress, value, sizeof(type) * amount);

#if PXMemoryDebug

#else

#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Compare
//---------------------------------------------------------

	PXPublic int PXMemoryCompareThreeWay(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);


	PXPublic PXBool PXMemoryCompareToByte(const void* PXRestrict bufferA, const PXSize bufferASize, const PXByte dataByte);

	// Returns 1 if correct, 0 if not.
	// This function is not like memcmp that returns -1, 0, and 1!
	PXPublic PXBool PXMemoryCompare(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);




//---------------------------------------------------------



	PXPublic PXBool PXAPI PXMemoryIsEmpty(const void* PXRestrict buffer, const PXSize bufferSize);
	PXPublic PXBool PXAPI PXMemorySwap(void* PXRestrict bufferA, void* PXRestrict bufferB, const PXSize size);



#ifdef __cplusplus
}
#endif

#endif