#ifndef MemoryINCLUDE
#define MemoryINCLUDE

#include <Format/Type.h>

#include <OS/OSVersion.h>

#if OSUnix
#define MemoryProtectionModeType int
#elif OSWindows
#define MemoryProtectionModeType unsigned long // DWORD
#endif

//---<Settings>---
#define MemoryAssertEnable 0
#define MemoryDebugOutput 0
#define MemoryDebugLeakDetection 0
#define MemoryUseSystemFunction 0
#define MemorySanitise 0
//----------------

#define _PX_FILEPATH_ __FILE__
#define _PX_FILENAME_ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define _PX_FUNCTION_ __FUNCTION__
#define _PX_LINE_ __LINE__

#if 1//MemoryDebugLeakDetection
#define MemoryAllocate(size) MemoryHeapAllocateDetailed(size, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_) // maybe you need to turn on "Keep comments"
#else
#define MemoryAllocate(size) MemoryAllocate(size)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum MemoryProtectionMode_
	{
		MemoryInvalid,
		MemoryNoReadWrite,
		MemoryReadOnly,
		MemoryWriteOnly,
		MemoryReadAndWrite
	}
	MemoryProtectionMode;

	typedef enum MemoryCachingMode_
	{
		MemoryCachingDefault,

		MemoryCachingRandom,  // Access data in a random order.
		MemoryCachingSequential, // Data sequentially from lower offsets to higher offsets.
		MemoryCachingTemporary, // File will not be saves to drive.
		MemoryCachingUseOnce, // Access the specified data once and then not reuse it again.

		// Windows only
		MemoryCachingWriteThrough,
		MemoryCachingNoBuffering, // No OS Caching, direct to Harddrive if supprted

		// UNIX only
		MemoryCachingNeedLater, // Data is not needed right now. "For later"[near future].
		MemoryCachingDontNeedNow // Data will not be cached. "I dont need it yet"
	}
	MemoryCachingMode;

	typedef struct MemoryUsage_
	{
		size_t PhysicalRAMSize;

		size_t PercentInUse;
		size_t PhysicalTotal;
		size_t PhysicalAvailable;
		size_t VirtualTotal;
		size_t VirtualAvailable;
		size_t PageFileTotal;
		size_t PageFileAvailable;
		size_t ExtendedVirtualAvailable;
	}
	MemoryUsage;


	PXPublic unsigned char MemoryScan(MemoryUsage* memoryUsage);


	PXPublic void MemoryClear(void* const __restrict bufferA, const size_t bufferASize);
	PXPublic void MemorySet(void* const __restrict bufferA, const size_t bufferASize, const unsigned char value);

	// Returns 1 if correct, 0 if not.
	// This function is not like memcmp that returns -1, 0, and 1!
	PXPublic unsigned char MemoryCompare(const void* __restrict bufferA, const size_t bufferASize, const void* __restrict bufferB, const size_t bufferBSize);

	PXPublic size_t MemoryCopy(const void* __restrict inputBuffer, const size_t inputBufferSize, void* outputBuffer, const size_t outputBufferSize);

	//CPublic char MemoryAdvice(const void* adress, const size_t length, const FileCachingMode fileCachingMode);

	// Allocates size bytes on the program stack.
	// The allocated space is automatically freed when the calling function exits
	// (not when the allocation merely passes out of scope).
	PXPublic void* MemoryStackAllocate(const size_t size);
	//PXPublic void* MemoryStackRelease(void* const adress);

	PXPublic void* MemoryHeapAllocateDetailed(const size_t size, const char* file, const char* function, const size_t line);
	PXPublic void* MemoryHeapAllocate(const size_t size);

	// Allocate memory and clear is after. Its just a combination of malloc and memset
	PXPublic void* MemoryAllocateClear(const size_t size);
	PXPublic void* MemoryReallocate(void* adress, const size_t size);
	PXPublic void* MemoryReallocateClear(const void* const adress, const size_t sizeBefore, const size_t sizeAfter);
	PXPublic void MemoryRelease(const void* adress, const size_t size);

	// Allocate memory in virtual memory space.
	// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
	// Only use for bigger datablocks as thic has very hi overhead.
	PXPublic void* MemoryVirtualAllocate(const size_t size, const MemoryProtectionMode memoryProtectionMode);
	PXPublic void MemoryVirtualPrefetch(const void* adress, const size_t size);
	PXPublic void MemoryVirtualRelease(const void* adress, const size_t size);


	PXPublic MemoryProtectionModeType ConvertFromMemoryProtectionMode(const MemoryProtectionMode memoryProtectionMode);

#ifdef __cplusplus
}
#endif

#endif
