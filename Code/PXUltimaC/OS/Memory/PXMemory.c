#include "PXMemory.h"

#include <Math/PXMath.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Console/PXConsole.h>
#include <Media/PXText.h>

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
#include <crtdbg.h>

#define ProtectionIDRead PAGE_READONLY
#define ProtectionIDWrite PAGE_READWRITE
#define ProtectionIDReadWrite PAGE_READWRITE


PXActionResult PXAPI WindowsProcessPrivilege(const char* pszPrivilege, BOOL bEnable)
{
	HANDLE           hToken;
	TOKEN_PRIVILEGES tp;
	BOOL             status;
	DWORD            error;

	// open process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return PXErrorCurrent(); // OpenProcessToken

	// get the luid
	if (!LookupPrivilegeValueA(NULL, pszPrivilege, &tp.Privileges[0].Luid))
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

#if PXMemoryDebug
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

PXBool PXAPI PXMemoryScan(PXMemoryUsage* memoryUsage)
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

void PXAPI PXMemoryClear(void* const PXRestrict bufferA, const PXSize bufferASize)
{
	PXMemorySet(bufferA, 0u, bufferASize);
}

void PXAPI PXMemorySetI32U(int* const PXRestrict bufferA, const int value, const PXSize amount)
{
	for(PXSize i = 0; i < amount; ++i)
	{
		((int volatile*)bufferA)[i] = value;
	}
}

void PXAPI PXMemorySet(void* const PXRestrict buffer, const PXByte value, const PXSize bufferSize)
{
//#if MemoryAssertEnable
//	assert(bufferA);
//#endif

#if PXMemoryDebug
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

int PXAPI PXMemoryCompareThreeWay(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize)
{
	const PXSize bufferSize = PXMathMinimumIU(bufferASize, bufferBSize);


#if MemoryAssertEnable
	assert(bufferA);
	assert(bufferB);
#endif

#if PXMemoryDebug  && 0
	PXLogPrint
	(
		PXLoggingInfo,
		"Memory",
		"Compare",
		"%6zi B  0x%p == 0x%p",	
		bufferSize,
		bufferA,
		bufferB
	);
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

PXBool PXAPI PXMemoryCompareToByte(const void* PXRestrict bufferA, const PXSize bufferASize, const PXByte dataByte)
{
	PXSize equalSum = 0;

	for (PXSize counter = bufferASize; counter; --counter)
	{
		equalSum += dataByte == ((PXAdress)bufferA)[counter];
	}

	return equalSum == bufferASize;
}

PXBool PXAPI PXMemoryCompare(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize)
{
	const PXSize bufferSize = PXMathMinimumIU(bufferASize, bufferBSize);

	if(bufferSize == 0)
	{
		return PXFalse;
	}

#if MemoryAssertEnable
	assert(bufferA);
	assert(bufferB);
#endif

#if PXMemoryDebug && 0

	if(bufferSize == 1)
	{
		PXLogPrint
		(
			PXLoggingInfo,
			"Memory",
			"Compare",
			"%6zi B  0x%2.2x == 0x%2.2x",
			bufferSize,
			*(char*)bufferA,
			*(char*)bufferB
		);
	}
	else
	{
		const PXSize bufferCap = 8;
		const PXSize bufferACap = PXMathMinimumIU(bufferASize, bufferCap);
		const PXSize bufferBCap = PXMathMinimumIU(bufferBSize, bufferCap);

		PXText pxTextA;
		PXText pxTextB;
		PXTextConstructNamedBufferA(&pxTextA, pxTextABuffer, 16);
		PXTextConstructNamedBufferA(&pxTextB, pxTextBBuffer, 16);
		PXTextFormatData(&pxTextA, bufferA, bufferSize);
		PXTextFormatData(&pxTextB, bufferB, bufferSize);

		PXLogPrint
		(
			PXLoggingInfo,
			"Memory",
			"Compare",
			"%6zi B  <%s> == <%s>",
			bufferSize,
			pxTextA.TextA,
			pxTextB.TextA
		);
	}

	
#endif

#if MemoryUseSystemFunction
	return memcmp(bufferB, bufferA, bufferSize) == 0;
#else
	PXSize equalSum = 0;

	for (PXSize counter = bufferSize; counter; --counter)
	{
		equalSum += ((PXAdress)bufferA)[counter-1] == ((PXAdress)bufferB)[counter-1];
	}

	return equalSum == bufferSize;
#endif
}

PXBool PXAPI PXMemoryIsEmpty(const void* PXRestrict buffer, const PXSize bufferSize)
{
	PXBool isNull = PXTrue;

	for (PXSize i = 0; isNull && i < bufferSize; ++i)
	{
		isNull = ((PXAdress)buffer)[i] == 0;
	}

	return isNull;
}

PXBool PXAPI PXMemorySwap(void* PXRestrict bufferA, void* PXRestrict bufferB, const PXSize size)
{
	void* adress = PXNull;

	PXNewStackList(char, size, &adress, PXNull);

	PXMemoryCopy(bufferA, size, adress, size);
	PXMemoryCopy(bufferB, size, bufferA, size);
	PXMemoryCopy(adress, size, bufferB, size);

	PXDeleteStackList(char, size, adress, PXNull);

	return PXTrue;
}

const void* PXAPI PXMemoryLocateFirst(const void* const PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize)
{
#if MemoryUseSystemFunction
	const void* memoryPosition = memchr(inputBuffer, byteBlock, inputBufferSize);

	return memoryPosition;
#else
	PXBool found = PXFalse;
	PXSize index = 0;

	for (; index < inputBufferSize && !found; ++index)
	{
		found = byteBlock == ((char volatile*)inputBuffer)[index];
	}

	return found * (PXSize)((char volatile*)inputBuffer + index);
#endif
}

const void* PXAPI PXMemoryLocateLast(const void* const PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize)
{
#if MemoryUseSystemFunction

	void* position = strrchr(inputBuffer, byteBlock);

	return position;
#else

	// Own implementation
	PXSize maxRange = inputBufferSize;

	if(-1 == inputBufferSize)
	{
		void* endOfString = PXMemoryLocateFirst(inputBuffer, 0, (PXSize)-1);

		maxRange = (PXSize)endOfString - (PXSize)inputBuffer;
	}


	for(PXSize i = maxRange; i > 0; --i)
	{		
		const char data = ((char volatile*)inputBuffer)[i];
		const PXBool isFound = byteBlock == data;

		if(isFound)
		{
			return &((char volatile*)inputBuffer)[i];
		}
	}

#endif

	return PXNull;
}

PXSize PXAPI PXMemoryCopy(const void* PXRestrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
	const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

#if MemoryAssertEnable
	assert(inputBuffer);
	assert(outputBuffer);
#endif

#if PXMemoryDebug && 0
	PXLogPrint
	(
		PXLoggingInfo,
		"Memory",
		"Copy",
		"%6zi B  0x%p -> 0x%p",
		bufferSize,
		inputBuffer,	
		outputBufferSize
	);
#endif

#if MemoryUseSystemFunction
	memcpy(outputBuffer, inputBuffer, bufferSize);
#else
	for (PXSize index = bufferSize ; index ; --index)
	{
		((char volatile*)outputBuffer)[index-1] = ((char volatile*)inputBuffer)[index-1];
	}
#endif

	return bufferSize;
}

PXSize PXAPI PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize)
{
	const PXSize bufferSize = PXMathMinimumIU(inputBufferSize, outputBufferSize);

#if MemoryUseSystemFunction
	memmove(outputBuffer, inputBuffer, bufferSize);
#else
	// Need a solution for a copy without a variable
	//memmove(outputBuffer, inputBuffer, bufferSize);

	for(size_t i = 0; i < bufferSize; i++)
	{
		char volatile* a = &((char volatile*)outputBuffer)[i];
		char volatile* b = &((char volatile*)inputBuffer)[i];

		*b = *a;
	}
#endif

	return bufferSize;
}

PXActionResult PXAPI PXMemoryHeapAllocate(PXMemoryInfo* const pxMemoryInfo)
{
	if(!pxMemoryInfo)
	{
		return PXActionRefusedArgumentNull;
	}

	if(!pxMemoryInfo->Data)
	{
		return PXActionRefusedArgumentNull;
	}

	const PXSize allocationSize = pxMemoryInfo->TypeSize * pxMemoryInfo->Amount;
	void* allocatedMemory = PXNull;

	if(!allocationSize)
	{
		return PXActionRefusedArgumentInvalid;
	}

	// Try allocate
	{
#if OSUnix

		if(pxMemoryInfo->MemoryClear)
		{
			allocatedMemory = calloc(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize);
		}
		else
		{
			allocatedMemory = malloc(allocationSize);
		}

#elif OSWindows

#if MemoryUseSystemFunction

#if 1
		if(pxMemoryInfo->MemoryClear)
		{
			allocatedMemory = calloc(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize);
		}
		else
		{
			allocatedMemory = malloc(allocationSize);
		}
#else

		if(!pxMemoryInfo->MemoryClear)
		{
			allocatedMemory = _malloc_dbg(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize, _NORMAL_BLOCK, pxMemoryInfo->File, pxMemoryInfo->Line); // crtdbg.h
		}
		else
		{
			allocatedMemory = _calloc_dbg(pxMemoryInfo->Amount, pxMemoryInfo->TypeSize, _NORMAL_BLOCK, pxMemoryInfo->File, pxMemoryInfo->Line); // crtdbg.h
		}
#endif

		
#else
		const HANDLE healHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h
		const DWORD flags = pxMemoryInfo->MemoryClear * HEAP_ZERO_MEMORY;
		allocatedMemory = HeapAlloc(healHandle, flags, allocationSize); // Windows 2000 SP4, Kernel32.dll, heapapi.h
#endif

#endif
	}


	if(!allocatedMemory)
	{
		// Allocation failed!
		return PXActionFailedMemoryAllocation;
	}

	*pxMemoryInfo->Data = allocatedMemory;

	if(pxMemoryInfo->SizeTotal > 0)
	{
		*pxMemoryInfo->SizeTotal = allocationSize;
	}

#if PXLogEnable && PXMemoryDebug && 0
	PXLoggingEventData pxLoggingEventData;
	pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
	pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
	pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
	pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
	pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Heap-Alloc";
	pxLoggingEventData.PrintFormat = "";
	pxLoggingEventData.Type = PXLoggingAllocation;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData);
#endif

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMemoryHeapDeallocate(PXMemoryInfo* const pxMemoryInfo)
{
	if(!pxMemoryInfo)
	{
		return PXFalse;
	}

#if PXLogEnable && PXMemoryDebug && 0
	PXLoggingEventData pxLoggingEventData;
	PXClear(PXLoggingEventData, &pxLoggingEventData);
	pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
	pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
	pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
	pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
	pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Heap-Free";
	pxLoggingEventData.PrintFormat = "";
	pxLoggingEventData.Type = PXLoggingDeallocation;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData);
#endif

#if MemorySanitise
	MemorySet(adress, '#', size);
#endif

#if MemoryDebugLeakDetection
	PXDictionaryRemove(&_memoryAdressLookup, adress);
#endif

#if MemoryUseSystemFunction
	//_free_dbg(*pxMemoryInfo->Data, _NORMAL_BLOCK);
	free(*pxMemoryInfo->Data);
#else

#if OSUnix

	// TODO: linux free

#elif OSWindows
	const HANDLE heapHandle = GetProcessHeap(); // Windows 2000 SP4, Kernel32.dll, heapapi.h
	const BOOL freeResult = HeapFree(heapHandle, 0, *pxMemoryInfo->Data); // Windows 2000 SP4, Kernel32.dll, heapapi.h

	if(!freeResult)
	{
		return PXActionFailedMemoryRelease;
	}
#endif

#endif

	*pxMemoryInfo->Data = PXNull;

	if(pxMemoryInfo->SizeTotal)
	{
		*pxMemoryInfo->SizeTotal = 0;
	}


	return PXActionSuccessful;
}

void* PXAPI PXMemoryHeapRealloc(void* buffer, PXSize size)
{
#if 0
	PXMemoryHeapReallocateEventData pxMemoryHeapReallocateEventData;
	PXMemoryHeapReallocateEventDataFillType(&pxMemoryHeapReallocateEventData, PXByte, size, 0, 0, buffer);

	PXMemoryHeapReallocate(&pxMemoryHeapReallocateEventData);

	return pxMemoryHeapReallocateEventData.DataAdress;
#else

	return realloc(buffer, size);


#endif
}

PXBool PXAPI PXMemoryHeapReallocate(PXMemoryHeapReallocateEventData* const pxMemoryHeapReallocateInfo)
{
	const PXSize sizeToAllocate = pxMemoryHeapReallocateInfo->AmountDemand * pxMemoryHeapReallocateInfo->TypeSize;
	
	
	
	
	PXSize oldSize = 0;
	void* adressOld = PXNull;

	if(pxMemoryHeapReallocateInfo->DataSize)
	{
		oldSize = *pxMemoryHeapReallocateInfo->DataSize;
	}

	if(pxMemoryHeapReallocateInfo->DataAdress)
	{
		adressOld = *pxMemoryHeapReallocateInfo->DataAdress;
	}



	// Need realloc?
	const PXSize isEnoughSpace = oldSize >= sizeToAllocate;

	// If there is enough space and we dont want to minimize size, we dont need to realloc
	if(isEnoughSpace && !pxMemoryHeapReallocateInfo->ReduceSizeIfPossible)
	{
		return PXTrue;
	}


	

	void* adressNew = PXNull;
	const PXSize beforeSize = oldSize;


#if MemoryUseSystemFunction
	adressNew = realloc(adressOld, sizeToAllocate);
#else
	const HANDLE heapHandle = GetProcessHeap();
	const PXBool hasNotBeenAllocated = adressOld == PXNull;

	if(hasNotBeenAllocated)
	{
		adressNew =	HeapAlloc(heapHandle, 0, sizeToAllocate);
	}
	else
	{
		adressNew = HeapReAlloc(heapHandle, 0, adressOld, sizeToAllocate);
	}

#endif

	if (!adressNew)
	{
		pxMemoryHeapReallocateInfo->WasSuccessful = PXFalse;

		return PXFalse;
	}

	pxMemoryHeapReallocateInfo->FreshAllocationPerformed = PXNull == *pxMemoryHeapReallocateInfo->DataAdress;

	pxMemoryHeapReallocateInfo->WasDataMoved = adressNew != pxMemoryHeapReallocateInfo->DataAdress;
	*pxMemoryHeapReallocateInfo->DataAdress = adressNew;
	*pxMemoryHeapReallocateInfo->DataSize = sizeToAllocate;

	if (beforeSize == 0)
	{
		pxMemoryHeapReallocateInfo->WasSizeIncreased = PXTrue;
	}
	else
	{
		pxMemoryHeapReallocateInfo->WasSizeIncreased = pxMemoryHeapReallocateInfo->AmountDemand > *pxMemoryHeapReallocateInfo->AmountCurrent;
	}

	
	pxMemoryHeapReallocateInfo->WasSuccessful = PXTrue;

	if (pxMemoryHeapReallocateInfo->AmountCurrent)
	{
		*pxMemoryHeapReallocateInfo->AmountCurrent = pxMemoryHeapReallocateInfo->AmountDemand;
	}

	// Is we have new space and we want to fill it, do now.
	if (pxMemoryHeapReallocateInfo->FreshAllocationPerformed || pxMemoryHeapReallocateInfo->WasSizeIncreased)
	{
		pxMemoryHeapReallocateInfo->PointOfNewData = &((char*)adressNew)[beforeSize];
		pxMemoryHeapReallocateInfo->PointOfNewDataSize = sizeToAllocate - beforeSize;

		if (pxMemoryHeapReallocateInfo->DoFillNewSpace)
		{
			PXMemorySet
			(
				pxMemoryHeapReallocateInfo->PointOfNewData,
				pxMemoryHeapReallocateInfo->FillSymbol,
				pxMemoryHeapReallocateInfo->PointOfNewDataSize
			);
		}	
	}
	else
	{
		pxMemoryHeapReallocateInfo->PointOfNewData = 0;
		pxMemoryHeapReallocateInfo->PointOfNewDataSize = 0;
	}

#if PXLogEnable && PXMemoryDebug
	PXLoggingEventData pxLoggingEventData;
	PXClear(PXLoggingEventData, &pxLoggingEventData);
	pxLoggingEventData.MemoryData.TypeSize = pxMemoryHeapReallocateInfo->TypeSize;
	pxLoggingEventData.MemoryData.Amount = pxMemoryHeapReallocateInfo->AmountDemand;
	pxLoggingEventData.MemoryData.NameFile = pxMemoryHeapReallocateInfo->CodeFileName;
	pxLoggingEventData.MemoryData.NameFunction = pxMemoryHeapReallocateInfo->CodeFunctionName;
	pxLoggingEventData.MemoryData.NumberLine = pxMemoryHeapReallocateInfo->CodeFileLine;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Heap-Realloc";
	pxLoggingEventData.PrintFormat = "";
	pxLoggingEventData.Type = PXLoggingReallocation;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData);
#endif

	return adressNew != 0;
}

void* PXAPI PXMemoryVirtualAllocate(PXSize size, const PXMemoryAccessMode PXMemoryAccessMode)
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

#if PXMemoryDebug
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

#if PXLogEnable
	PXLoggingEventData pxLoggingEventData;
	PXClear(PXLoggingEventData, &pxLoggingEventData);
	pxLoggingEventData.MemoryData.TypeSize = size;
	pxLoggingEventData.MemoryData.Amount = 1;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Virtual-Alloc";
	pxLoggingEventData.PrintFormat = "  0x%p [%s]";
	pxLoggingEventData.Type = PXLoggingAllocation;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData, addressAllocated,	readMode);
#endif
#endif

	return (void*)addressAllocated;
}

void PXAPI PXMemoryVirtualPrefetch(const void* adress, const PXSize size)
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

#if PXLogEnable
	PXLoggingEventData pxLoggingEventData;
	PXClear(PXLoggingEventData, &pxLoggingEventData);
	pxLoggingEventData.MemoryData.TypeSize = size;
	pxLoggingEventData.MemoryData.Amount = 1;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Pre-Fetch";
	pxLoggingEventData.PrintFormat = "  0x%p";
	pxLoggingEventData.Type = PXLoggingInfo;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData, adress);
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

void PXAPI PXMemoryVirtualRelease(const void* adress, const PXSize size)
{
#if OSUnix
	const unsigned char result = 1u;

#elif OSWindows
	DWORD freeType = MEM_RELEASE;
	const PXBool result = VirtualFree((void*)adress, 0, freeType); // Windows XP (+UWP), Kernel32.dll, memoryapi.h 
#endif

#if PXLogEnable && PXMemoryDebug
	PXLoggingEventData pxLoggingEventData;
	PXClear(PXLoggingEventData, &pxLoggingEventData);
	pxLoggingEventData.MemoryData.TypeSize = size;
	pxLoggingEventData.MemoryData.Amount = 1;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Virtual-free";
	pxLoggingEventData.PrintFormat = "  0x%p";
	pxLoggingEventData.Type = PXLoggingDeallocation;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData, adress);
#endif

	//return result; // We dont return info
}

void* PXAPI PXMemoryVirtualReallocate(const void* adress, const PXSize size)
{
	const PXBool newAllocation = adress == PXNull;

	if (newAllocation)
	{
		return PXMemoryVirtualAllocate(size, PXMemoryAccessModeReadAndWrite);
	}

	return 0;
}

PXMemoryAccessModeType PXAPI PXMemoryAccessModeFromID(const PXMemoryAccessMode PXMemoryAccessMode)
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

#define PXMemoryUseStackAllocation 0

PXActionResult PXAPI PXMemoryStackAllocate(PXMemoryInfo* const pxMemoryInfo)
{
#if PXMemoryUseStackAllocation
	const PXSize totalSize = pxMemoryInfo->TypeSize * pxMemoryInfo->Amount;
	void* const stackAllocated =

#if OSUnix
		alloca(totalSize);
#elif OSWindows
		//_alloca(size); // _alloca() is deprecated (security reasons) but _malloca() is not an alternative

		_malloca(totalSize);
#endif

	* pxMemoryInfo->Data = stackAllocated;

	if(pxMemoryInfo->SizeTotal)
	{
		*pxMemoryInfo->SizeTotal = totalSize;
	}

#if PXLogEnable && PXMemoryDebug
	PXLoggingEventData pxLoggingEventData;
	pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
	pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
	pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
	pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
	pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Stack-Alloc";
	pxLoggingEventData.PrintFormat = "";
	pxLoggingEventData.Type = PXLoggingAllocation;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData);
#endif

#else

	return PXMemoryHeapAllocate(pxMemoryInfo);

#endif

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMemoryStackDeallocate(PXMemoryInfo* const pxMemoryInfo)
{
	if(!pxMemoryInfo)
	{
		return;
	}

#if PXMemoryUseStackAllocation

#if OSUnix

#elif OSWindows
	_freea(*pxMemoryInfo->Data);
#endif

	*pxMemoryInfo->Data = PXNull;

	if(pxMemoryInfo->SizeTotal)
	{
		*pxMemoryInfo->SizeTotal = 0;
	}


#if PXLogEnable && 0
	PXLoggingEventData pxLoggingEventData;
	pxLoggingEventData.MemoryData.TypeSize = pxMemoryInfo->TypeSize;
	pxLoggingEventData.MemoryData.Amount = pxMemoryInfo->Amount;
	pxLoggingEventData.MemoryData.NameFile = pxMemoryInfo->File;
	pxLoggingEventData.MemoryData.NameFunction = pxMemoryInfo->Function;
	pxLoggingEventData.MemoryData.NumberLine = pxMemoryInfo->Line;
	pxLoggingEventData.ModuleSource = "Memory";
	pxLoggingEventData.ModuleAction = "Stack Release";
	pxLoggingEventData.PrintFormat = "";
	pxLoggingEventData.Type = PXLoggingDeallocation;
	pxLoggingEventData.Target = PXLoggingTypeTargetMemory;

	PXLogPrintInvoke(&pxLoggingEventData);
#endif

#else	

	return PXMemoryHeapDeallocate(pxMemoryInfo);

#endif


}



/*
void* PXMemoryHeapAllocateCleared(const PXSize objectSize, const PXSize amount)
{
	if (!objectSize || !amount)
	{
		return PXNull;
	}

	void* const adress = calloc(amount, objectSize);

#if PXMemoryDebug
	printf("[#][Memory] 0x%p (%10zi B) Allocate on heap cleared\n", adress, requestedSizeInBytes);
#endif

	return adress;
}

PXBool PXMemoryHeapResizeArray(PXSize typeSize, void** dataAddress, PXSize* const dataAddressSize, const PXSize dataAddressSizeRequired)
{
	{
		const PXBool needsResize = *dataAddressSize < dataAddressSizeRequired;

		if (!needsResize)
		{
			return PXTrue; // Array is big enough, resize not needed
		}
	}

	void* newAdress = PXMemoryHeapReallocateTypeClear
	(
		*dataAddress,
		typeSize,
		*dataAddressSize,
		dataAddressSizeRequired
	);

	{
		const PXBool reallocSuccess = newAdress != 0;

		if (!reallocSuccess)
		{
			return PXFalse;
		}
	}

	*dataAddressSize = dataAddressSizeRequired;
	*dataAddress = newAdress;

	return PXTrue;

}
void* PXMemoryHeapReallocateClear(void* const sourceAddress, const PXSize sizeBefore, const PXSize sizeAfter)
{
	void* const adressReallocated = realloc(sourceAddress, sizeAfter);
	const PXBool sizeIncredes = sizeAfter > sizeBefore;

	if (sizeIncredes)
	{
		const PXAdress startAdress = (PXAdress)adressReallocated + sizeBefore;
		const PXSize sizeDelta = sizeAfter - sizeBefore;

		PXMemoryClear(startAdress, sizeDelta);
	}

#if PXMemoryDebug
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

void* PXMemoryHeapReallocateTypeClear(void* const adress, const PXSize objectSize, const PXSize numberOfElementsBefore, const PXSize numberOfElementsAfter)
{
	return PXMemoryHeapReallocateClear(adress, objectSize * numberOfElementsBefore, objectSize * numberOfElementsAfter);
}

*/
