#ifndef PXMemoryINCLUDE
#define PXMemoryINCLUDE

#include <Media/PXType.h>
#include <Compiler/PXCompilerSettings.h>

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


	PXPublic PXBool PXAPI PXMemoryScan(PXMemoryUsage* memoryUsage);





	PXPublic const void* PXAPI PXMemoryLocate(const void* PXRestrict inputBuffer, const PXByte byteBlock, const PXSize inputBufferSize);

	


	// PXPublic char MemoryAdvice(const void* adress, const PXSize length, const FileCachingMode fileCachingMode);










	// Allocates size bytes on the program stack.
	// The allocated space is automatically freed when the calling function exits
	// (not when the allocation merely passes out of scope).
#if PXMemoryDebug
	PXPublic void* PXAPI PXMemoryStackAllocate(const PXSize typeSize, const PXSize amount, const char* file, const char* function, const PXSize line);
#else
	PXPublic void* PXAPI PXMemoryStackAllocate(const PXSize typeSize, const PXSize amount);
#endif



	// Deallocates stack allocated memory if it was commited to the heap.
	// Additional size parameter can be ignored	
#if PXMemoryDebug
	PXPublic void PXAPI PXMemoryStackRelease(const PXSize typeSize, const PXSize amount, void* const dataAdress, const char* file, const char* function, const PXSize line);
#else
	PXPublic void PXAPI PXMemoryStackRelease(const PXSize typeSize, const PXSize amount, void* const dataAdress);
#endif



	typedef struct PXMemoryHeapReallocateEventData_
	{
		PXSize TypeSize;
		PXSize AmountDemand;
		PXSize* AmountCurrent;
		PXSize* DataSize;
		void** DataAdress;
#if PXMemoryDebug
		const char* CodeFileName;
		const char* CodeFunctionName;
		PXSize CodeFileLine;
#endif

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
	}
	PXMemoryHeapReallocateEventData;

	PXPublic PXBool PXAPI PXMemoryHeapReallocate(PXMemoryHeapReallocateEventData* const pxMemoryHeapReallocateEventData);


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






//---------------------------------------------------------
// Allocation
//---------------------------------------------------------
#if PXMemoryDebug
	PXPublic void* PXMemoryHeapAllocateDetailed(const PXSize typeSize, const PXSize amount, const char* file, const char* function, const PXSize line);
#define PXNew(type) (type*)PXMemoryHeapAllocateDetailed(sizeof(type), 1, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#define PXNewList(type, amount) (type*)PXMemoryHeapAllocateDetailed(sizeof(type), amount, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)

#define PXStackNew(type, amount) (type*)PXMemoryStackAllocate(sizeof(type), amount, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)

#else
	PXPublic void* PXAPI PXMemoryHeapAllocate(const PXSize amount, const PXSize typeSize);

#define PXNew(type) (type*)PXMemoryHeapAllocate(1u, sizeof(type))
#define PXNewList(type, amount) (type*)PXMemoryHeapAllocate(amount, sizeof(type))
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Deallocation
//---------------------------------------------------------
#if PXMemoryDebug
	PXPublic void PXAPI PXMemoryReleaseDetailed(void* adress, const PXSize size, const char* file, const char* function, const PXSize line);

#define PXDelete(type, adress) PXMemoryReleaseDetailed(adress, sizeof(type), _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)
#define PXDeleteList(type, adress, amount) PXMemoryReleaseDetailed(adress, sizeof(type) * amount, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)

#define PXStackDelete(type, amount, adress) PXMemoryStackRelease(sizeof(type), amount, adress, _PX_FILENAME_, _PX_FUNCTION_, _PX_LINE_)

#else
	PXPublic void PXAPI PXMemoryRelease(void* adress, const PXSize size);

#define PXDelete(type, adress) PXMemoryRelease(adress, sizeof(type))
#define PXDeleteList(type, adress, amount) PXMemoryRelease(adress, sizeof(type) * (PXInt32U)amount)
#endif
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