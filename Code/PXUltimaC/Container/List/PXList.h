#ifndef PXListDEFINED
#define PXListDEFINED

#include <Media/PXType.h>

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
    PXSize DataTypeSize; // Size of a single object.
    PXSize AmountUsed;  // Size currently used by data.
    PXSize AmountAllocated; // Size currently allocated and usable
    PXSize GrouthOnAlloc;
}
PXList;

PXPublic void PXAPI PXListInitialize(PXList* const pxList, const PXSize dataTypeSize, const PXSize sizeToAllocate);
PXPublic void PXAPI PXListRelease(PXList* const pxList);
PXPublic PXBool PXAPI PXListReserve(PXList* const pxList, const PXSize sizeToReserve);


//---------------------------------------------------------
// List
//---------------------------------------------------------
PXPublic void* PXAPI PXListAdd(PXList* const pxList, void* const dataElement);
#define PXListAddT(Type, pxList, dataElement) (Type*)PXListAdd(pxList, dataElement)

PXPublic void* PXAPI PXListItemAtIndexGet(PXList* const pxList, const PXSize index);
#define PXListEntyrGetT(Type, pxList, index) (Type*)PXListItemAtIndexGet(pxList, index)
//---------------------------------------------------------

//---------------------------------------------------------
// Queue
//---------------------------------------------------------
// Get position of first item
PXPublic PXBool PXAPI PXListItemFromStartGet(PXList* const queue, void* const dataElement);
//---------------------------------------------------------

#endif
