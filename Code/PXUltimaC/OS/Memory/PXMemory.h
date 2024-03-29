#ifndef PXMemoryINCLUDE
#define PXMemoryINCLUDE

#include <Media/PXType.h>
#include <Compiler/PXCompilerSettings.h>
#include <Media/PXResource.h>
#include <OS/Error/PXActionResult.h>

#if OSUnix
typedef int PXMemoryAccessModeType;
#elif OSWindows
typedef unsigned long PXMemoryAccessModeType;// DWORD
#endif

//---<Settings>---
#define MemorySizeUnkown -1
#define MemoryAssertEnable 0
#define PXMemoryDebug 1
#define MemoryDebugLeakDetection 0
#define MemoryUseSystemFunction 0
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

	typedef struct PXMemoryInfo_
	{
		// OUT
		void** Data;
		PXSize* SizeTotal;

		// Input
		PXSize Amount;
		PXSize TypeSize;
	
		// Debug Info
#if PXMemoryDebug
		char* File;
		char* Function;
		PXSize Line;
#endif

		PXBool MemoryClear;
	}
	PXMemoryInfo;




	typedef struct PXMemoryHeapReallocateEventData_
	{
		PXSize TypeSize;
		PXSize AmountDemand;
		PXSize* AmountCurrent;
		PXSize* DataSize;
		void** DataAdress;

		PXBool DoFillNewSpace;
		PXByte FillSymbol;
		PXBool ReduceSizeIfPossible;

		// Additinaional Flags
		PXBool FreshAllocationPerformed;
		PXBool WasSuccessful;
		PXBool WasDataMoved;

		// Data of added memory space
		void* PointOfNewData;
		PXSize PointOfNewDataSize;
		PXBool WasSizeIncreased;

#if PXMemoryDebug
		const char* CodeFileName;
		const char* CodeFunctionName;
		PXSize CodeFileLine;
#endif
	}
	PXMemoryHeapReallocateEventData;




	// POSIX




	PXPublic PXBool PXAPI PXMemoryScan(PXMemoryUsage* memoryUsage);

	PXPublic inline const void* PXAPI PXMemoryLocateFirst(const void* PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);
	PXPublic inline const void* PXAPI PXMemoryLocateLast(const void* PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);
	


	// PXPublic char MemoryAdvice(const void* adress, const PXSize length, const FileCachingMode fileCachingMode);







#if OSUnix
#define _PX_FILENAME_ ((const char*)(PXMemoryLocateLast(__FILE__, '/', -1) ? (char*)PXMemoryLocateLast(__FILE__, '/', -1) + 1 : __FILE__))
#elif OSWindows
#define _PX_FILENAME_ ((const char*)(PXMemoryLocateLast(__FILE__, '\\', -1) ? (char*)PXMemoryLocateLast(__FILE__, '\\', -1) + 1 : __FILE__))
#endif






	PXPublic void* PXAPI PXMemoryHeapRealloc(void* buffer, PXSize size);

	PXPublic PXBool PXAPI PXMemoryHeapReallocate(PXMemoryHeapReallocateEventData* const pxMemoryHeapReallocateInfo);


	//PXPublic PXBool PXMemoryHeapResizeArray(PXSize typeSize, void** dataAddress, PXSize* const dataAddressSizeCurrent, const PXSize dataAddressSizeRequired);

	//PXPublic void* PXMemoryHeapReallocateClear(void* const adress, const PXSize sizeBefore, const PXSize sizeAfter);
	//PXPublic void* PXMemoryHeapReallocateTypeClear(void* const adress, const PXSize objectSize, const PXSize numberOfElementsBefore, const PXSize numberOfElementsAfter);



	// Allocate memory in virtual memory space.
	// The minimal size will be a pagefile (4KB) as the size will be rounded up to the next page boundary.
	// Only use for bigger datablocks as thic has very hi overhead.
	PXPublic void* PXAPI PXMemoryVirtualAllocate(PXSize size, const PXMemoryAccessMode PXMemoryAccessMode);
	PXPublic void PXAPI PXMemoryVirtualPrefetch(const void* adress, const PXSize size);
	PXPublic void PXAPI PXMemoryVirtualRelease(const void* adress, const PXSize size);
	PXPublic void* PXAPI PXMemoryVirtualReallocate(const void* adress, const PXSize size);


	PXPublic PXMemoryAccessModeType PXAPI PXMemoryAccessModeFromID(const PXMemoryAccessMode PXMemoryAccessMode);




	// Allocates size bytes on the program stack.
	// The allocated space is automatically freed when the calling function exits
	// (not when the allocation merely passes out of scope).
	PXPublic PXActionResult PXAPI PXMemoryStackAllocate(PXMemoryInfo* const pxMemoryAllocateInfo);
	
	// Deallocates stack allocated memory if it was commited to the heap.
	// Additional size parameter can be ignored	
	PXPublic PXActionResult PXAPI PXMemoryStackDeallocate(PXMemoryInfo* const pxMemoryAllocateInfo);


	PXPublic PXActionResult PXAPI PXMemoryHeapAllocate(PXMemoryInfo* const pxMemoryAllocateInfo);
	PXPublic PXActionResult PXAPI PXMemoryHeapDeallocate(PXMemoryInfo* const pxMemoryAllocateInfo);

#define PXMemoryInfoFill(pxMemoryInfo, type, amount, dataAdress, dataSizeAdress, memoryClear) \
	pxMemoryHeapAllocateInfo.Data = dataAdress; \
	pxMemoryHeapAllocateInfo.SizeTotal = dataSizeAdress; \
	pxMemoryHeapAllocateInfo.Amount = amount; \
	pxMemoryHeapAllocateInfo.TypeSize = sizeof(type); \
	pxMemoryHeapAllocateInfo.MemoryClear = memoryClear; \
	pxMemoryHeapAllocateInfo.File = _PX_FILENAME_; \
	pxMemoryHeapAllocateInfo.Function = _PX_FUNCTION_; \
	pxMemoryHeapAllocateInfo.Line = _PX_LINE_; \


#define PXNewListSettings(type, amount, dataAdress, dataSizeAdress, memoryClear) { \
	PXMemoryInfo pxMemoryHeapAllocateInfo; \
	PXMemoryInfoFill(pxMemoryInfo,type, amount, dataAdress, dataSizeAdress, memoryClear); \
	PXMemoryHeapAllocate(&pxMemoryHeapAllocateInfo); }

#define PXNewList(type, amount, dataAdress, dataSizeAdress) PXNewListSettings(type, amount, dataAdress, dataSizeAdress, PXFalse);
#define PXNewListZerod(type, amount, dataAdress, dataSizeAdress) PXNewListSettings(type, amount, dataAdress, dataSizeAdress, PXTrue)
	
#define PXNew(type, dataAdress) PXNewList(type, 1u, dataAdress, PXNull)
#define PXNewZerod(type, dataAdress) PXNewListZerod(type, 1u, dataAdress, PXNull)

#define PXDeleteList(type, amount, dataAdress, dataSizeAdress) { \
	PXMemoryInfo pxMemoryHeapAllocateInfo; \
	PXMemoryInfoFill(pxMemoryInfo,type, amount, dataAdress, dataSizeAdress, PXFalse); \
	PXMemoryHeapDeallocate(&pxMemoryHeapAllocateInfo); }

#define PXDelete(type, dataAdress) PXDeleteList(type, 1u, dataAdress, PXNull)


#define PXNewStackList(type, amount, dataAdress, dataSizeAdress) { \
	PXMemoryInfo pxMemoryHeapAllocateInfo; \
	PXMemoryInfoFill(pxMemoryInfo,type, amount, dataAdress, dataSizeAdress, PXFalse); \
	PXMemoryStackAllocate(&pxMemoryHeapAllocateInfo); }

#define PXNewStack(type, dataAdress) PXNewStackList(type, 1u, dataAdress, PXNull)


#define PXDeleteStackList(type, amount, dataAdress, dataSizeAdress) { \
	PXMemoryInfo pxMemoryHeapAllocateInfo; \
	PXMemoryInfoFill(pxMemoryInfo,type, amount, dataAdress, dataSizeAdress, PXFalse); \
	PXMemoryStackDeallocate(&pxMemoryHeapAllocateInfo); }

#define PXDeleteStack(type, dataAdress) PXDeleteStackList(type, 1u, dataAdress, PXNull)

//---------------------------------------------------------



//---------------------------------------------------------
// Reallocation
//---------------------------------------------------------
#if PXMemoryDebug

// PXMemoryHeapReallocateEventData pxMemoryHeapReallocateEventData;
// PXHeapListResizeSize

#define	PXMemoryHeapReallocateEventDataFill(eventDataAdress, typeSize, target, current, sizeCurrent, dataAdress) \
	PXMemoryClear(eventDataAdress, sizeof(PXMemoryHeapReallocateEventData));  \
	(*eventDataAdress).TypeSize = typeSize; \
	(*eventDataAdress).AmountDemand = target; \
	(*eventDataAdress).AmountCurrent = current; \
	(*eventDataAdress).DataSize = sizeCurrent; \
	(*eventDataAdress).DataAdress = dataAdress; \
	(*eventDataAdress).ReduceSizeIfPossible = PXTrue; \
	(*eventDataAdress).CodeFileName = _PX_FILENAME_; \
	(*eventDataAdress).CodeFunctionName = _PX_FUNCTION_; \
	(*eventDataAdress).CodeFileLine = _PX_LINE_;

#define	PXMemoryHeapReallocateEventDataFillType(eventDataAdress, type, target, current, sizeCurrent, dataAdress) \
	PXMemoryHeapReallocateEventDataFill(eventDataAdress, sizeof(type), target, current, sizeCurrent, dataAdress)

#define PXMemoryHeapReallocateEventDataFillNew(eventDataAdress, fillSymbol) \
	(*eventDataAdress).DoFillNewSpace = PXTrue; \
	(*eventDataAdress).FillSymbol = fillSymbol; \


//#define PXHeapListResize(type, amountDemand, amountCurrent, adress) PXMemoryHeapReallocate(sizeof(type), amountDemand, amountCurrent, adress, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
//#define PXHeapListResizeT(typeSize, amountDemand, amountCurrent, adress) PXMemoryHeapReallocate(typeSize, amountDemand, amountCurrent, adress, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#else 
#define PXHeapListResize(type, amountDemand, amountCurrent, adress) PXMemoryHeapReallocate(sizeof(type), amountDemand, amountCurrent, adress)
#endif	
//---------------------------------------------------------



//---------------------------------------------------------
// Copy
//---------------------------------------------------------
	PXPublic PXSize PXAPI PXMemoryCopy(const void* PXRestrict inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

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
	PXPublic PXSize PXAPI PXMemoryMove(const void* inputBuffer, const PXSize inputBufferSize, void* outputBuffer, const PXSize outputBufferSize);

#if PXMemoryDebug
		

//#define PXMove(type, source, target) PXMemoryMove(adress, sizeof(type));
#else

#endif
//---------------------------------------------------------




//---------------------------------------------------------
// Clear
//---------------------------------------------------------
	PXPublic void PXAPI PXMemoryClear(void* const PXRestrict bufferA, const PXSize bufferASize);

#define PXClear(type, adress) PXMemoryClear(adress, sizeof(type));
#define PXClearList(type, adress, amount) PXMemoryClear(adress, sizeof(type) * amount);

#if PXMemoryDebug


#else

#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Set
//---------------------------------------------------------
	PXPublic void PXAPI PXMemorySet(void* const PXRestrict bufferA, const PXByte value, const PXSize bufferSize);
#define PXSet(type, adress, value) PXMemorySet(adress, value, sizeof(type));
#define PXSetList(type, adress, amount, value) PXMemorySet(adress, value, sizeof(type) * amount);

#if PXMemoryDebug

#else

#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Compare
//---------------------------------------------------------

	PXPublic int PXAPI PXMemoryCompareThreeWay(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);


	PXPublic PXBool PXAPI PXMemoryCompareToByte(const void* PXRestrict bufferA, const PXSize bufferASize, const PXByte dataByte);

	// Returns 1 if correct, 0 if not.
	// This function is not like memcmp that returns -1, 0, and 1!
	PXPublic PXBool PXAPI PXMemoryCompare(const void* PXRestrict bufferA, const PXSize bufferASize, const void* PXRestrict bufferB, const PXSize bufferBSize);




//---------------------------------------------------------



	PXPublic PXBool PXAPI PXMemoryIsEmpty(const void* PXRestrict buffer, const PXSize bufferSize);
	PXPublic PXBool PXAPI PXMemorySwap(void* PXRestrict bufferA, void* PXRestrict bufferB, const PXSize size);



#ifdef __cplusplus
}
#endif

#endif