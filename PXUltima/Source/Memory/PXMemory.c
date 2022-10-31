#include "PXMemory.h"

#include <Math/PXMath.h>

#include <stdlib.h>
#include <malloc.h>

#if OSUnix

#include <sys/mman.h>

#define ProtectionIDRead PROT_READ
#define ProtectionIDWrite PROT_WRITE
#define ProtectionIDReadWrite PROT_READ | PROT_WRITE

#elif OSWindows

#include <Windows.h>

#define ProtectionIDRead PAGE_READONLY
#define ProtectionIDWrite PAGE_READWRITE
#define ProtectionIDReadWrite PAGE_READWRITE

#endif

//---<Settings>------------------------
#define MemoryAssertEnable 0
#define MemoryDebugOutput 0
#define MemoryDebugLeakDetection 0
#define MemoryUseSystemFunction 0
#define MemorySanitise 1

#if MemoryAssertEnable
#include <assert.h>
#endif

#if MemoryDebugOutput
#include <stdio.h>
#endif

#if MemoryDebugLeakDetection
#include <Container/PXDictionary/PXDictionary.h>
#include <Time/Time.h>

PXDictionary _memoryAdressLookup;

struct MemoryAllocationInfo
{
	Time TimeLastChanged; // When
	size_t Size;
	const char* Source;
};

#endif
//-------------------------------------

unsigned char MemoryScan(MemoryUsage* memoryUsage)
{
	MemorySet(memoryUsage, sizeof(MemoryUsage), 0);

#if OSUnix
#elif WindowsAtleastXP
	// MEMORYSTATUS won't work on >4GB Systems

	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

	const unsigned char result = GlobalMemoryStatusEx(&memoryStatus);



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

void MemorySet(void* __restrict bufferA, const size_t bufferASize, const unsigned char value)
{
//#if MemoryAssertEnable
//	assert(bufferA);
//#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Set to %2x\n", bufferA, bufferASize, value);
#endif

	if(!bufferA)
	{
		return;
	}

#if MemoryUseSystemFunction
	memset(bufferA, value, bufferASize);
#else
	for(size_t i = 0; i < bufferASize; ++i)
	{
		((unsigned char*)bufferA)[i] = value;
	}
#endif
}

unsigned char MemoryCompare(const void* __restrict bufferA, const size_t bufferASize, const void* __restrict bufferB, const size_t bufferBSize)
{
	const size_t bufferSize = MathMinimum(bufferASize, bufferBSize);
	size_t counter = bufferSize;
	size_t equalSum = 0;

#if MemoryAssertEnable
	assert(bufferA);
	assert(bufferB);
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Compare to 0x%p\n", bufferA, bufferSize, bufferB);
#endif

#if MemoryUseSystemFunction
	return memcmp(a, b, length) == 0;
#else

	while(counter--)
	{
		equalSum += ((unsigned char*)bufferA)[counter] == ((unsigned char*)bufferB)[counter];
	}

	return equalSum == bufferSize;
#endif
}

size_t MemoryCopy(const void* __restrict inputBuffer, const size_t inputBufferSize, void* outputBuffer, const size_t outputBufferSize)
{
	const size_t bufferSize = MathMinimum(inputBufferSize, outputBufferSize);
	size_t index = bufferSize;

#if MemoryAssertEnable
	assert(inputBuffer);
	assert(outputBuffer);
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Copy to 0x%p\n", inputBuffer, bufferSize, outputBufferSize);
#endif

	while(index--)
	{
		const unsigned char* inputByte = ((unsigned char*)inputBuffer) + index;
		unsigned char* outputByte = ((unsigned char*)outputBuffer) + index;

		*outputByte = *inputByte;
	}

	return bufferSize;
}

void* MemoryAllocate(const size_t requestedSizeInBytes)
{
	if(!requestedSizeInBytes)
	{
		return 0;
	}

	const void* adress = malloc(requestedSizeInBytes);

	if(!adress)
	{
#if MemoryDebugOutput
		printf("[x][Memory] Allocation failed! (%10zi B) Error: Out of memory\n", requestedSizeInBytes);
#endif

		return 0;
	}

#if MemorySanitise
	MemorySet(adress, requestedSizeInBytes, '#');
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Allocate\n", adress, requestedSizeInBytes);
#endif

#if MemoryDebugLeakDetection
	PXDictionaryAdd(&_memoryAdressLookup, adress, requestedSizeInBytes);
#endif

	return adress;
}

void* MemoryAllocateClear(const size_t requestedSizeInBytes)
{
	if (!requestedSizeInBytes)
	{
		return 0;
	}

	const void* adress = calloc(1u, requestedSizeInBytes);

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Allocate Cleared\n", adress, requestedSizeInBytes);
#endif

	return adress;
}

void* MemoryReallocate(void* adress, const size_t size)
{
	const void* adressReallocated = realloc(adress, size);

#if MemorySanitise
	if (!adress)
	{
		MemorySet(adressReallocated, size, '#');
	}
#endif

#if MemoryDebugOutput
	const unsigned char hasChanged = adress != adressReallocated;

	if(hasChanged)
	{
		printf("[#][Memory] 0x%p (%10zi B) Reallocate to 0x%p\n", adress, size, adressReallocated);
	}
	else
	{
		printf("[#][Memory] 0x%p (%10zi B) Reallocate (No Change)\n", adress, size);
	}
#endif

	return adressReallocated;
}

void* MemoryReallocateClear(const void* const adress, const size_t sizeBefore, const size_t sizeAfter)
{
	const void* adressReallocated = realloc(adress, sizeAfter);
	const unsigned char sizeIncredes = sizeAfter > sizeBefore;

	if (sizeIncredes)
	{
		const unsigned char* startAdress = (unsigned char*)adressReallocated + sizeBefore;
		const size_t sizeDelta = sizeAfter - sizeBefore;

		MemorySet(startAdress, sizeDelta, 0);
	}

#if MemoryDebugOutput
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

void MemoryRelease(const void* adress, const size_t size)
{
	if(!adress || !size)
	{
		return;
	}

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Free\n", adress, size);
#endif

#if MemorySanitise
	MemorySet(adress, size, '#');
#endif

#if MemoryDebugLeakDetection
	PXDictionaryRemove(&_memoryAdressLookup, adress);
#endif

	free(adress);
}

void* MemoryVirtualAllocate(const size_t size, const MemoryProtectionMode memoryProtectionMode)
{
	const void* addressPrefered = 0;
	const MemoryProtectionModeType protectionModeID = ConvertFromMemoryProtectionMode(memoryProtectionMode);

#if OSUnix
	const int flags = MAP_PRIVATE;// | MAP_ANONYMOUS; | MAP_POPULATE; // missing on linux?
	const int fileDescriptor = -1;
	const off_t offset = 0;

	const void* addressAllocated = mmap
	(
		(void*)addressPrefered,
		size,
		protectionModeID,
		flags,
		fileDescriptor,
		offset
	);

	// Check if mmap was sucessful
	{
		const unsigned char sucessful = addressAllocated != MAP_FAILED;

		if(!sucessful)
		{
			return 0;
		}
	}

#elif OSWindows
	DWORD allocationType = MEM_COMMIT | MEM_RESERVE;

	const void* addressAllocated = VirtualAlloc((void*)addressPrefered, size, allocationType, protectionModeID);
#endif

#if MemoryDebugOutput
	const char* readMode;

	switch(memoryProtectionMode)
	{
		case MemoryWriteOnly:
			readMode = "Write only";
			break;

		case MemoryReadOnly:
			readMode = "Read only";
			break;

		case MemoryReadAndWrite:
			readMode = "Read & Write";
			break;

		default:
			readMode = "???";
			break;
	}

	printf("[#][Memory] 0x%p (%10zi B) Virtual allocation [%s]\n", addressAllocated, size, readMode);
#endif

	return (void*)addressAllocated;
}

void MemoryVirtualPrefetch(const void* adress, const size_t size)
{
#if OSUnix
#elif OSWindows
#if WindowsAtleast8
	const HANDLE process = GetCurrentProcess();
	const size_t numberOfEntries = 2;
	WIN32_MEMORY_RANGE_ENTRY memoryRangeEntry;
	const size_t flags = 0; // reserved and needs to be 0

	memoryRangeEntry.VirtualAddress = (void*)adress;
	memoryRangeEntry.NumberOfBytes = size;

	//const bool prefetchResult = PrefetchVirtualMemory(process, numberOfEntries, &memoryRangeEntry, flags);

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Pre-Fetched\n", adress, size);
#endif

#endif // defined(WindowsAtleast8)
#else

#if MemoryDebug
	printf("[#][Memory] 0x%p (%10zi B) Pre-Fetched [NOT SUPPORTED] Skipped...\n", adress, size);
#endif
#endif
}

void MemoryVirtualRelease(const void* adress, const size_t size)
{
#if OSUnix
	const unsigned char result = 1u;

#elif OSWindows
	DWORD freeType = MEM_RELEASE;
	const unsigned char result = VirtualFree((void*)adress, 0, freeType);
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Virtual free\n", adress, size);
#endif

	return result;
}

MemoryProtectionModeType ConvertFromMemoryProtectionMode(const MemoryProtectionMode memoryProtectionMode)
{
	switch(memoryProtectionMode)
	{
		case MemoryReadOnly:
			return ProtectionIDRead;

		case MemoryWriteOnly:
			return ProtectionIDWrite;

		case MemoryReadAndWrite:
			return ProtectionIDReadWrite;

		default:
			return MemoryInvalid;
	}
}
