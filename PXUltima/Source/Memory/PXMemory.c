#include "PXMemory.h"

#include <Math/PXMath.h>

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

#define ProtectionIDRead PAGE_READONLY
#define ProtectionIDWrite PAGE_READWRITE
#define ProtectionIDReadWrite PAGE_READWRITE

#endif

//---<Settings>------------------------

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

PXBool MemoryScan(MemoryUsage* memoryUsage)
{
	MemoryClear(memoryUsage, sizeof(MemoryUsage));

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

void MemoryClear(void* const __restrict bufferA, const size_t bufferASize)
{
	return MemorySet(bufferA, bufferASize, 0u);
}

void MemorySet(void* __restrict bufferA, const size_t bufferASize, const unsigned char value)
{
//#if MemoryAssertEnable
//	assert(bufferA);
//#endif

#if MemoryDebugOutput
	//printf("[#][Memory] 0x%p (%10zi B) Set to %2x\n", bufferA, bufferASize, value);
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
		((PXAdress)bufferA)[i] = value;
	}
#endif
}

PXBool MemoryCompare(const void* __restrict bufferA, const size_t bufferASize, const void* __restrict bufferB, const size_t bufferBSize)
{
	const size_t bufferSize = MathMinimumIU(bufferASize, bufferBSize);
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
		equalSum += ((PXAdress)bufferA)[counter] == ((PXAdress)bufferB)[counter];
	}

	return equalSum == bufferSize;
#endif
}

size_t MemoryCopy(const void* __restrict inputBuffer, const size_t inputBufferSize, void* outputBuffer, const size_t outputBufferSize)
{
	const size_t bufferSize = MathMinimumIU(inputBufferSize, outputBufferSize);
	size_t index = bufferSize;

#if MemoryAssertEnable
	assert(inputBuffer);
	assert(outputBuffer);
#endif

#if MemoryDebugOutput
	//printf("[#][Memory] 0x%p (%10zi B) Copy to 0x%p\n", inputBuffer, bufferSize, outputBufferSize);
#endif

	while(index--)
	{
		const PXAdress inputByte = ((PXAdress)inputBuffer) + index;
		PXAdress outputByte = ((PXAdress)outputBuffer) + index;

		*outputByte = *inputByte;
	}

	return bufferSize;
}

void* MemoryStackAllocate(const size_t size)
{ 
	void* const stackAllocated  =

#if OSUnix
		alloca(size);
#elif OSWindows
		_alloca(size); // _alloca() is deprecated (security reasons) but _malloca() is not an alternative
#endif

	return stackAllocated;
}

#include <stdio.h>


void* MemoryHeapAllocateDetailed(const size_t size, const char* file, const char* function, const size_t line)
{
	void* adress = MemoryHeapAllocate(size);

	printf("[+][Memory][%-13s|%-19s|%4i|%7iB|\n", file, function, (unsigned int)line, (unsigned int)size);

	return adress;
}

void* MemoryHeapAllocate(const size_t requestedSizeInBytes)
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

void* MemoryHeapReallocate(void* sourceAddress, const size_t size)
{
	const void* adressReallocated = realloc(sourceAddress, size);

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
		printf("[#][Memory] 0x%p (%10zi B) Reallocate to 0x%p\n", sourceAddress, size, adressReallocated);
	}
	else
	{
		printf("[#][Memory] 0x%p (%10zi B) Reallocate (No Change)\n", sourceAddress, size);
	}
#endif

	return adressReallocated;
}

void* MemoryHeapReallocateDetailed(void* sourceAddress, const size_t size, const char* file, const char* function, const size_t line)
{
	void* adress = MemoryHeapReallocate(sourceAddress, size);

	printf("[R][Memory][%-13s|%-19s|%4i|%7iB|\n", file, function, (unsigned int)line, (unsigned int)size);

	return adress;
}

void* MemoryHeapReallocateClear(const void* const sourceAddress, const size_t sizeBefore, const size_t sizeAfter)
{
	const void* adressReallocated = realloc(sourceAddress, sizeAfter);
	const unsigned char sizeIncredes = sizeAfter > sizeBefore;

	if (sizeIncredes)
	{
		const PXAdress startAdress = (PXAdress)adressReallocated + sizeBefore;
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
