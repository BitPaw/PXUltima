#ifndef PXMemoryINCLUDE
#define PXMemoryINCLUDE

#include <Media/PXType.h>

#if OSUnix
typedef int PXMemoryAccessModeType;
#elif OSWindows
typedef unsigned long PXMemoryAccessModeType;// DWORD
#endif

//---<Settings>---
#define MemorySizeUnkown -1
#define MemoryAssertEnable 0
#define MemoryDebugOutput 0
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


	PXPublic void PXMemoryClear(void* const PXRestrict bufferA, const PXSize bufferASize);
	PXPublic void PXMemorySet(void* const PXRestrict bufferA, const PXByte value, const PXSize bufferSize);

	PXPublic int PXMemoryCompareThreeWay(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);



	PXPublic PXBool PXMemoryCompareToByte(const void* PXRestrict bufferA, const PXSize bufferASize, const PXByte dataByte);

	// Returns 1 if correct, 0 if not.
	// This function is not like memcmp that returns -1, 0, and 1!
	PXPublic PXBool PXMemoryCompare(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);

	PXPublic const void* PXMemoryLocate(const void* PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);

	PXPublic PXSize PXMemoryCopy(const void* PXRestrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);
	PXPublic PXSize PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

	//CPublic char MemoryAdvice(const void* adress, const PXSize length, const FileCachingMode fileCachingMode);

	// Allocates size bytes on the program stack.
	// The allocated space is automatically freed when the calling function exits
	// (not when the allocation merely passes out of scope).
	PXPublic void* PXMemoryStackAllocate(const PXSize size);
	//PXPublic void* MemoryStackRelease(void* const adress);

	PXPublic void* PXMemoryHeapAllocate(const PXSize size);

	PXPublic void* PXMemoryHeapAllocateCleared(const PXSize objectSize, const PXSize amount);

#if MemoryDebugOutput
	PXPublic void* PXMemoryHeapAllocateDetailed(const PXSize size, const char* file, const char* function, const PXSize line);
#endif
	
	PXPublic void* PXMemoryHeapReallocate(void* sourceAddress, const PXSize size);

#if MemoryDebugOutput
	PXPublic void* PXMemoryHeapReallocateDetailed(void* sourceAddress, const PXSize size, const char* file, const char* function, const PXSize line);
#endif
	PXPublic void* PXMemoryHeapReallocateClear(void* const adress, const PXSize sizeBefore, const PXSize sizeAfter);
	PXPublic void* PXMemoryHeapReallocateTypeClear(void* const adress, const PXSize objectSize, const PXSize numberOfElementsBefore, const PXSize numberOfElementsAfter);
	PXPublic void PXMemoryRelease(void* adress, const PXSize size);

	// Allocate memory in virtual memory space.
	// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
	// Only use for bigger datablocks as thic has very hi overhead.
	PXPublic void* PXMemoryVirtualAllocate(PXSize size, const PXMemoryAccessMode PXMemoryAccessMode);
	PXPublic void PXMemoryVirtualPrefetch(const void* adress, const PXSize size);
	PXPublic void PXMemoryVirtualRelease(const void* adress, const PXSize size);
	PXPublic void* PXMemoryVirtualReallocate(const void* adress, const PXSize size);


	PXPublic PXMemoryAccessModeType PXMemoryAccessModeFromID(const PXMemoryAccessMode PXMemoryAccessMode);

#ifdef __cplusplus
}
#endif


#define PXObjectClear(objectType, adressofObject) PXMemoryClear(adressofObject, sizeof(objectType))

#if 1 // Use default allocator

#if MemoryDebugOutput
#define PXMemoryReallocate(address, dataSize) PXMemoryHeapReallocateDetailed(address, dataSize, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#define PXMemoryAllocate(dataSize) PXMemoryHeapAllocateDetailed(dataSize, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#else
#define PXMemoryReallocate(address, dataSize) PXMemoryHeapReallocate(address, dataSize)
#define PXMemoryAllocate(dataSize) PXMemoryHeapAllocate(dataSize)
#define PXMemoryAllocateType(type, dataSize) (type*)PXMemoryHeapAllocate(sizeof(type) * dataSize)
#define PXMemoryAllocateTypeCleared(type, dataSize) (type*)PXMemoryHeapAllocateCleared(sizeof(type), dataSize)
#define PXMemoryReallocateTypeCleared(type, adressCurrent, dataSizeBefore, dataSizeAfter) (type*)PXMemoryHeapReallocateTypeClear(adressCurrent, sizeof(type), dataSizeBefore, dataSizeAfter)

#define PXNew(type) (type*)PXMemoryHeapAllocate(sizeof(type))
#define PXDelete(type, adress) PXMemoryRelease(adress, sizeof(type))
#endif

#else // Use virtual alloc

#define PXMemoryReallocate(address, dataSize) PXMemoryVirtualReallocate(address, dataSize)
#define PXMemoryAllocate(dataSize) MemoryVirtualAllocate(dataSize, PXMemoryAccessModeReadAndWrite)
#endif


#endif