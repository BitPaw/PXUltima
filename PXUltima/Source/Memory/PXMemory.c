#include "PXMemory.h"

#include <Math/PXMath.h>
#include <Error/PXActionResult.h>

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


PXActionResult WindowsProcessPrivilege(const wchar_t* pszPrivilege, BOOL bEnable)
{
	HANDLE           hToken;
	TOKEN_PRIVILEGES tp;
	BOOL             status;
	DWORD            error;

	// open process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return GetCurrentError(); // OpenProcessToken

	// get the luid
	if (!LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid))
		return GetCurrentError();  // LookupPrivilegeValue

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
		return GetCurrentError();  // AdjustTokenPrivileges

	// close the handle
	if (!CloseHandle(hToken))
		return GetCurrentError(); // CloseHandle

	return PXActionSuccessful;
}


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
	PXSize Size;
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

void MemoryClear(void* const __restrict bufferA, const PXSize bufferASize)
{
	return MemorySet(bufferA, 0u, bufferASize);
}

void MemorySet(void* __restrict buffer, const unsigned char value, const PXSize bufferSize)
{
//#if MemoryAssertEnable
//	assert(bufferA);
//#endif

#if MemoryDebugOutput
	//printf("[#][Memory] 0x%p (%10zi B) Set to %2x\n", bufferA, bufferASize, value);
#endif

	if(!buffer)
	{
		return;
	}

#if MemoryUseSystemFunction
	memset(bufferA, value, bufferASize);
#else
	for(PXSize i = 0; i < bufferSize; ++i)
	{
		((PXAdress)buffer)[i] = value;
	}
#endif
}

PXBool MemoryCompare(const void* __restrict bufferA, const PXSize bufferASize, const void* __restrict bufferB, const PXSize bufferBSize)
{
	const PXSize bufferSize = MathMinimumIU(bufferASize, bufferBSize);
	PXSize counter = bufferSize;
	PXSize equalSum = 0;

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

PXSize MemoryCopy(const void* __restrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
	const PXSize bufferSize = MathMinimumIU(inputBufferSize, outputBufferSize);
	PXSize index = bufferSize;

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

PXSize MemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
	const PXSize bufferSize = MathMinimumIU(inputBufferSize, outputBufferSize);

	memmove(outputBuffer, inputBuffer, bufferSize);

	return bufferSize;
}

void* MemoryStackAllocate(const PXSize size)
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


void* MemoryHeapAllocateDetailed(const PXSize size, const char* file, const char* function, const PXSize line)
{
	void* adress = MemoryHeapAllocate(size);
	char buffer[60];

	sprintf(buffer, "%s:%s:%i", file, function, (unsigned int)line);
	printf("[+][Memory][%-37.37s|%7iB|\n", buffer, (unsigned int)size);

	return adress;
}

void* MemoryHeapAllocate(const PXSize requestedSizeInBytes)
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
	MemorySet(adress, '#', requestedSizeInBytes);
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Allocate\n", adress, requestedSizeInBytes);
#endif

#if MemoryDebugLeakDetection
	PXDictionaryAdd(&_memoryAdressLookup, adress, requestedSizeInBytes);
#endif

	return adress;
}

void* MemoryAllocateClear(const PXSize requestedSizeInBytes)
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

void* MemoryHeapReallocate(void* sourceAddress, const PXSize size)
{
	const void* adressReallocated = realloc(sourceAddress, size);

#if MemorySanitise
	if (!adress)
	{
		MemorySet(adressReallocated, '#', size);
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

void* MemoryHeapReallocateDetailed(void* sourceAddress, const PXSize size, const char* file, const char* function, const PXSize line)
{
	void* adress = MemoryHeapReallocate(sourceAddress, size);
	char buffer[60];

	sprintf(buffer, "%s:%s:%i", file, function, (unsigned int)line);
	printf("[+][Memory][%-37.37s|%7iB|\n", buffer, (unsigned int)size);

	return adress;
}

void* MemoryHeapReallocateClear(const void* const sourceAddress, const PXSize sizeBefore, const PXSize sizeAfter)
{
	const void* adressReallocated = realloc(sourceAddress, sizeAfter);
	const unsigned char sizeIncredes = sizeAfter > sizeBefore;

	if (sizeIncredes)
	{
		const PXAdress startAdress = (PXAdress)adressReallocated + sizeBefore;
		const PXSize sizeDelta = sizeAfter - sizeBefore;

		MemoryClear(startAdress, sizeDelta);
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

void MemoryRelease(const void* adress, const PXSize size)
{
	if(!adress || !size)
	{
		return;
	}

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Free\n", adress, size);
#endif

#if MemorySanitise
	MemorySet(adress, '#', size);
#endif

#if MemoryDebugLeakDetection
	PXDictionaryRemove(&_memoryAdressLookup, adress);
#endif

	free(adress);
}

void* MemoryVirtualAllocate(PXSize size, const MemoryProtectionMode memoryProtectionMode)
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

	const void* addressAllocated = VirtualAlloc((void*)addressPrefered, size, allocationType, protectionModeID);

	PXActionResult x = GetCurrentError();

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

void MemoryVirtualPrefetch(const void* adress, const PXSize size)
{
#if OSUnix
#elif OSWindows
#if WindowsAtleast8
	const HANDLE process = GetCurrentProcess();
	const PXSize numberOfEntries = 2;
	WIN32_MEMORY_RANGE_ENTRY memoryRangeEntry;
	const PXSize flags = 0; // reserved and needs to be 0

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

void MemoryVirtualRelease(const void* adress, const PXSize size)
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

void* MemoryVirtualReallocate(const void* adress, const PXSize size)
{
	const PXBool newAllocation = adress == PXNull;

	if (newAllocation)
	{
		return MemoryVirtualAllocate(size, MemoryReadAndWrite);
	}

	return 0;
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
