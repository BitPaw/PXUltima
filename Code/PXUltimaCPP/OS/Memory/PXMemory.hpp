#ifndef PXCPPMemoryINCLUDE
#define PXCPPMemoryINCLUDE

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
		static T* Allocate(const PXSize size);

		// Changes the size of a given byteblock.knging the poniter.
		// The function will return NULL if the system is "out of memory".
		template<typename T>
		static T* Reallocate(const T* const adress, const PXSize size);

		static bool Scan(PXMemoryUsage& memoryUsage);

		static void Clear(void* const buffer, const PXSize bufferSize);
		static void Set(void* const buffer, const PXByte value, const PXSize bufferSize);

		static int CompareThreeWay(const void* bufferA, const PXSize bufferASize, const void* bufferB, const PXSize bufferBSize);



		static bool CompareToByte(const void* bufferA, const PXSize bufferASize, const PXByte dataByte);

		// Returns 1 if correct, 0 if not.
		// This function is not like memcmp that returns -1, 0, and 1!
		static bool Compare(const void* bufferA, const PXSize bufferASize, const void* bufferB, const PXSize bufferBSize);

		static void* Locate(const void* inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);

		static PXSize Copy(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);
		static PXSize Move(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

		//CPublic char MemoryAdvice(const void* adress, const PXSize length, const FileCachingMode fileCachingMode);

		// Allocates size bytes on the program stack.
		// The allocated space is automatically freed when the calling function exits
		// (not when the allocation merely passes out of scope).
		static void* StackAllocate(const PXSize size);
		//static void* MemoryStackRelease(void* const adress);

		static void* HeapAllocate(const PXSize size);

#if MemoryDebugOutput
		static void* HeapAllocateDetailed(const PXSize size, const char* file, const char* function, const PXSize line);
#endif
		// Allocate memory and clear is after. Its just a combination of malloc and memset
		static void* AllocateClear(const PXSize size);

		static void* HeapReallocate(void* sourceAddress, const PXSize size);

#if MemoryDebugOutput
		static void* PXMemoryHeapReallocateDetailed(void* sourceAddress, const PXSize size, const char* file, const char* function, const PXSize line);
#endif
		static void* PXMemoryHeapReallocateClear(const void* const adress, const PXSize sizeBefore, const PXSize sizeAfter);
		static void PXMemoryRelease(const void* adress, const PXSize size);

		// Allocate memory in virtual memory space.
		// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
		// Only use for bigger datablocks as thic has very hi overhead.
		static void* PXMemoryVirtualAllocate(PXSize size, const PXMemoryAccessMode PXMemoryAccessMode);
		static void PXMemoryVirtualPrefetch(const void* adress, const PXSize size);
		static void PXMemoryVirtualRelease(const void* adress, const PXSize size);
		static void* PXMemoryVirtualReallocate(const void* adress, const PXSize size);


		static PXMemoryAccessModeType PXMemoryAccessModeFromID(const PXMemoryAccessMode PXMemoryAccessMode);
	};

#if 1 // Use default allocator

#if MemoryDebugOutput
#define PXMemoryReallocate(address, dataSize) PXMemoryHeapReallocateDetailed(address, dataSize, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#define PXMemoryAllocate(dataSize) PXMemoryHeapAllocateDetailed(dataSize, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#else
#define PXMemoryReallocate(address, dataSize) PXMemoryHeapReallocate(address, dataSize)
#define PXMemoryAllocate(dataSize) PXMemoryHeapAllocate(dataSize)
#endif

#else // Use virtual alloc

#define PXMemoryReallocate(address, dataSize) PXMemoryVirtualReallocate(address, dataSize)
#define PXMemoryAllocate(dataSize) MemoryVirtualAllocate(dataSize, PXMemoryAccessModeReadAndWrite)
#endif
}

#endif