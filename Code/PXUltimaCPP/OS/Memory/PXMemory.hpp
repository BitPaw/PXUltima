#pragma once

#include <OS/Memory/PXMemory.h>

namespace PX
{
	enum class MemoryAccessMode
	{
		 Invalid,
		 NoReadWrite,
		 ReadOnly,
		 WriteOnly,
		 ReadAndWrite
	};

	enum class MemoryCachingMode
	{
		 Invalid,

		 Default,

		 Random,  // Access data in a random order.
		 Sequential, // Data sequentially from lower offsets to higher offsets.
		 Temporary, // File will not be saves to drive.
		 UseOnce, // Access the specified data once and then not reuse it again.

		// Windows only
		 WriteThrough,
		 NoBuffering, // No OS Caching, direct to Harddrive if supprted

		// UNIX only
		 NeedLater, // Data is not needed right now. "For later"[near future].
		 DontNeedNow // Data will not be cached. "I dont need it yet"
	};

	class Memory
	{
		public:
		// Allocates memory on the HEAP.
		// Returns NULL if systems is "out of memory"
		template<typename T>
		PXDLLExport static T* Allocate(const PXSize size);

		template<typename T>
		PXDLLExport static T* AllocateCleared(const PXSize amount);

		// Changes the size of a given byteblock.knging the poniter.
		// The function will return NULL if the system is "out of memory".
		template<typename T>
		PXDLLExport static T* Reallocate(const T* const adress, const PXSize size);

		PXDLLExport static bool Scan(PXMemoryUsage& memoryUsage);

		PXDLLExport static void Clear(void* const buffer, const PXSize bufferSize);
		PXDLLExport static void Set(void* const buffer, const PXByte value, const PXSize bufferSize);

		PXDLLExport static int CompareThreeWay(const void* bufferA, const PXSize bufferASize, const void* bufferB, const PXSize bufferBSize);



		PXDLLExport static bool CompareToByte(const void* bufferA, const PXSize bufferASize, const PXByte dataByte);

		// Returns 1 if correct, 0 if not.
		// This function is not like memcmp that returns -1, 0, and 1!
		PXDLLExport static bool Compare(const void* bufferA, const PXSize bufferASize, const void* bufferB, const PXSize bufferBSize);

		PXDLLExport static void* Locate(const void* inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);

		template<typename T>
		PXDLLExport static void Clear(T* const type);

		PXDLLExport static PXSize Copy(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);
		PXDLLExport static PXSize Move(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

		//CPublic char MemoryAdvice(const void* adress, const PXSize length, const FileCachingMode fileCachingMode);

		// Allocates size bytes on the program stack.
		// The allocated space is automatically freed when the calling function exits
		// (not when the allocation merely passes out of scope).
		PXDLLExport static void* StackAllocate(const PXSize size);
		//static void* MemoryStackRelease(void* const adress);

		PXDLLExport static void* HeapAllocate(const PXSize size);

#if PXMemoryDebug
		PXDLLExport static void* HeapAllocateDetailed(const PXSize size, const char* file, const char* function, const PXSize line);
#endif

		PXDLLExport static void* HeapReallocate(void* sourceAddress, const PXSize size);

#if PXMemoryDebug
		PXDLLExport static void* PXMemoryHeapReallocateDetailed(void* sourceAddress, const PXSize size, const char* file, const char* function, const PXSize line);
#endif
		PXDLLExport static void* PXMemoryHeapReallocateClear(const void* const adress, const PXSize sizeBefore, const PXSize sizeAfter);
		PXDLLExport static void PXMemoryRelease(const void* adress, const PXSize size);

		// Allocate memory in virtual memory space.
		// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
		// Only use for bigger datablocks as thic has very hi overhead.
		PXDLLExport static void* PXMemoryVirtualAllocate(PXSize size, const PXMemoryAccessMode PXMemoryAccessMode);
		PXDLLExport static void PXMemoryVirtualPrefetch(const void* adress, const PXSize size);
		PXDLLExport static void PXMemoryVirtualRelease(const void* adress, const PXSize size);
		PXDLLExport static void* PXMemoryVirtualReallocate(const void* adress, const PXSize size);


		PXDLLExport static PXMemoryAccessModeType PXMemoryAccessModeFromID(const PXMemoryAccessMode PXMemoryAccessMode);
	};
}