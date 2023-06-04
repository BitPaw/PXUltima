#include "PXMemory.h"

#include <Math/PXMath.h>
#include <OS/Error/PXActionResult.h>

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
		return PXErrorCurrent(); // OpenProcessToken

	// get the luid
	if (!LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid))
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

PXBool PXMemoryScan(PXMemoryUsage* memoryUsage)
{
	PXMemoryClear(memoryUsage, sizeof(PXMemoryUsage));

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

void PXMemoryClear(void* const PXRestrict bufferA, const PXSize bufferASize)
{
	return PXMemorySet(bufferA, 0u, bufferASize);
}

void PXMemorySet(void* PXRestrict buffer, const PXByte value, const PXSize bufferSize)
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
	memset(buffer, value, bufferSize);
#else
	for(PXSize i = 0; i < bufferSize; ++i)
	{
		((PXAdress)buffer)[i] = value;
	}
#endif
}

int PXMemoryCompareThreeWay(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize)
{
	const PXSize bufferSize = PXMathMinimumIU(bufferASize, bufferBSize);


#if MemoryAssertEnable
	assert(bufferA);
	assert(bufferB);
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Compare to 0x%p\n", bufferA, bufferSize, bufferB);
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

PXBool PXMemoryCompareToByte(const void* PXRestrict bufferA, const PXSize bufferASize, const PXByte dataByte)
{
	PXSize equalSum = 0;

	for (PXSize counter = bufferASize; counter; --counter)
	{
		equalSum += dataByte == ((PXAdress)bufferA)[counter];
	}

	return equalSum == bufferASize;
}

PXBool PXMemoryCompare(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize)
{
	const PXSize bufferSize = PXMathMinimumIU(bufferASize, bufferBSize);

#if MemoryAssertEnable
	assert(bufferA);
	assert(bufferB);
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Compare to 0x%p\n", bufferA, bufferSize, bufferB);
#endif

#if MemoryUseSystemFunction
	return memcmp(bufferA, bufferB, bufferSize) == 0;
#else
	PXSize equalSum = 0;

	for (PXSize counter = bufferSize; counter; --counter)
	{
		equalSum += ((PXAdress)bufferA)[counter] == ((PXAdress)bufferB)[counter];
	}

	return equalSum == bufferSize;
#endif
}

void* PXMemoryLocate(const void* PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize)
{
#if MemoryUseSystemFunction
	void* memoryPosition = memchr(inputBuffer, byteBlock, inputBufferSize);

	return memoryPosition;
#else
	PXBool found = PXFalse;
	PXSize index = 0;

	for (; index && !found; ++index)
	{
		found = byteBlock == ((PXAdress)inputBuffer)[index];
	}

	return found * (PXSize)((PXAdress)inputBuffer + index);
#endif
}

PXSize PXMemoryCopy(const void* PXRestrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
	const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

#if MemoryAssertEnable
	assert(inputBuffer);
	assert(outputBuffer);
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Copy to 0x%p\n", inputBuffer, bufferSize, outputBufferSize);
#endif

#if MemoryUseSystemFunction
	memcpy(outputBuffer, inputBuffer, bufferSize);
#else
	for (PXSize index = bufferSize ; index ; --index)
	{
		((PXAdress)outputBuffer)[index] = ((PXAdress)inputBuffer)[index];
	}
#endif

	return bufferSize;
}

PXSize PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
	const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

#if MemoryUseSystemFunction
	memmove(outputBuffer, inputBuffer, bufferSize);
#else
	// Need a solution for a copy without a variable
	memmove(outputBuffer, inputBuffer, bufferSize);
#endif

	return bufferSize;
}

void* PXMemoryStackAllocate(const PXSize size)
{ 
	void* const stackAllocated  =

#if OSUnix
		alloca(size);
#elif OSWindows
		_alloca(size); // _alloca() is deprecated (security reasons) but _malloca() is not an alternative
#endif

	return stackAllocated;
}

#if MemoryDebugOutput
void* PXMemoryHeapAllocateDetailed(const PXSize size, const char* file, const char* function, const PXSize line)
{
	void* adress = PXMemoryHeapAllocate(size);
	char buffer[64];

	sprintf_s(buffer, 64, "%s:%s:%i", file, function, (unsigned int)line);
	printf("[+][Memory][%-37.37s|%7iB|\n", buffer, (unsigned int)size);

	return adress;
}
#endif

void* PXMemoryHeapAllocate(const PXSize requestedSizeInBytes)
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

void* PXMemoryAllocateClear(const PXSize requestedSizeInBytes)
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

void* PXMemoryHeapReallocate(void* sourceAddress, const PXSize size)
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

#if MemoryDebugOutput
void* PXMemoryHeapReallocateDetailed(void* sourceAddress, const PXSize size, const char* file, const char* function, const PXSize line)
{
	void* adress = PXMemoryHeapReallocate(sourceAddress, size);
	char buffer[64];

	sprintf_s(buffer, 64, "%s:%s:%i", file, function, (unsigned int)line);
	printf("[+][Memory][%-37.37s|%7iB|\n", buffer, (unsigned int)size);

	return adress;
}
#endif
void* PXMemoryHeapReallocateClear(const void* const sourceAddress, const PXSize sizeBefore, const PXSize sizeAfter)
{
	const void* adressReallocated = realloc(sourceAddress, sizeAfter);
	const unsigned char sizeIncredes = sizeAfter > sizeBefore;

	if (sizeIncredes)
	{
		const PXAdress startAdress = (PXAdress)adressReallocated + sizeBefore;
		const PXSize sizeDelta = sizeAfter - sizeBefore;

		PXMemoryClear(startAdress, sizeDelta);
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

void PXMemoryRelease(const void* adress, const PXSize size)
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

void* PXMemoryVirtualAllocate(PXSize size, const PXMemoryAccessMode PXMemoryAccessMode)
{
	const void* addressPrefered = 0;
	const PXMemoryAccessModeType protectionModeID = PXMemoryAccessModeFromID(PXMemoryAccessMode);

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

	const void* addressAllocated = VirtualAlloc((void*)addressPrefered, size, allocationType, protectionModeID); // Windows XP (+UWP), Kernel32.dll, memoryapi.h 

	PXActionResult x = PXErrorCurrent();

#endif

#if MemoryDebugOutput
	const char* readMode;

	switch(PXMemoryAccessMode)
	{
		case PXMemoryAccessModeWriteOnly:
			readMode = "Write only";
			break;

		case PXMemoryAccessModeReadOnly:
			readMode = "Read only";
			break;

		case PXMemoryAccessModeReadAndWrite:
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

void PXMemoryVirtualPrefetch(const void* adress, const PXSize size)
{
#if OSUnix
#elif OSWindows
#if WindowsAtleast8 && PXOSWindowsDestop
	const HANDLE process = GetCurrentProcess();
	const PXSize numberOfEntries = 2;
	WIN32_MEMORY_RANGE_ENTRY memoryRangeEntry;
	const PXSize flags = 0; // reserved and needs to be 0

	memoryRangeEntry.VirtualAddress = (void*)adress;
	memoryRangeEntry.NumberOfBytes = size;

	//const bool prefetchResult = PrefetchVirtualMemory(process, numberOfEntries, &memoryRangeEntry, flags); // Windows 8, Kernel32.dll, memoryapi.h

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Pre-Fetched\n", adress, size);
#endif
#else
	// Not supported function
#endif
#else

#if MemoryDebug
	printf("[#][Memory] 0x%p (%10zi B) Pre-Fetched [NOT SUPPORTED] Skipped...\n", adress, size);
#endif
#endif
}

void PXMemoryVirtualRelease(const void* adress, const PXSize size)
{
#if OSUnix
	const unsigned char result = 1u;

#elif OSWindows
	DWORD freeType = MEM_RELEASE;
	const unsigned char result = VirtualFree((void*)adress, 0, freeType); // Windows XP (+UWP), Kernel32.dll, memoryapi.h 
#endif

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) Virtual free\n", adress, size);
#endif

	return result;
}

void* PXMemoryVirtualReallocate(const void* adress, const PXSize size)
{
	const PXBool newAllocation = adress == PXNull;

	if (newAllocation)
	{
		return PXMemoryVirtualAllocate(size, PXMemoryAccessModeReadAndWrite);
	}

	return 0;
}

PXMemoryAccessModeType PXMemoryAccessModeFromID(const PXMemoryAccessMode PXMemoryAccessMode)
{
	switch(PXMemoryAccessMode)
	{
		case PXMemoryAccessModeReadOnly:
			return ProtectionIDRead;

		case PXMemoryAccessModeWriteOnly:
			return ProtectionIDWrite;

		case PXMemoryAccessModeReadAndWrite:
			return ProtectionIDReadWrite;

		default:
			return PXMemoryAccessModeInvalid;
	}
}