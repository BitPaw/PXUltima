#ifndef PXListDEFINED
#define PXListDEFINED

#include <PX/Media/PXType.h>

#define PXListBehaviourMemoryVirtual 1

typedef enum PXListMode_
{
    PXListModeInvalid,
    PXListModeDefault,
    PXListModeLinkedSingle,
    PXListModeLinkedDouble,
    PXListModeQueue
}
PXListMode;

typedef struct PXList_
{
    void* Data; // Contains the whole data.

    PXSize EntryAmountUsed;  // Size currently used by data.
    PXSize EntryAmountAllocated; // Size currently allocated and usable
    PXSize EntryGrowthOnAllocation;

    PXSize DataTypeSize; // Size of a single object.
}
PXList;

PXPublic void PXAPI PXListInitialize(PXList* const pxList, const PXSize dataTypeSize, const PXSize startAmount);
PXPublic void PXAPI PXListRelease(PXList* const pxList);

// Returns index of element
PXPublic PXBool PXAPI PXListReserve(PXList* const pxList, const PXSize sizeToReserve);


//---------------------------------------------------------
// List
//---------------------------------------------------------
PXPublic void* PXAPI PXListAdd(PXList* const pxList, void* const dataElement);
#define PXListAddT(Type, pxList, dataElement) (Type*)PXListAdd(pxList, dataElement)

PXPublic void* PXAPI PXListItemAtIndexGet(PXList* const pxList, const PXSize index);
#define PXListItemAtIndexGetT(Type, pxList, index) (Type*)PXListItemAtIndexGet(pxList, index)
//---------------------------------------------------------

//---------------------------------------------------------
// Queue
//---------------------------------------------------------
// Get position of first item
PXPublic PXBool PXAPI PXListItemFromStartGet(PXList* const queue, void* const dataElement);
//---------------------------------------------------------


//---------------------------------------------------------
// Stack
//---------------------------------------------------------
 

PXPublic PXBool PXAPI PXListAppend(PXList* const pxList, void* const buffer, const PXSize bufferSize);
// IF the list is a stack, we can reduce the stack by a 
// given amount and extract the data on it if a buffer
// is defined.
PXPublic PXBool PXAPI PXListExtractAndReduce(PXList* const pxList, void* const buffer, const PXSize bufferSize);

#endif
