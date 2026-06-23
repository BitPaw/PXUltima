#pragma once

#ifndef PXListDEFINED
#define PXListDEFINED

#include <PX/Type/PXType.h>
#include <PX/Container/Buffer/PXBuffer.h>

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

typedef struct PXListEntry_
{
    void* Address;
    PXSize Index;
}
PXListEntry;

typedef struct PXList_
{
    PXBuffer Buffer;

    PXSize EntryAmountUsed;         // Size currently used by data.
    PXSize EntryAmountAllocated;    // Size currently allocated and usable
    PXSize EntryGrowthOnAllocation; // How much to grow if an allocation if space is insufficent

    PXSize ValueTypeSize;           // Size of a single object.

    // Only needed for queue behaviour
    PXSize HeadIndex; // dequeue position
    PXSize TailIndex; // enqueue position
}
PXList;

#define PXListT(x) PXList

PXPublic PXResult PXAPI PXListCreate(PXList PXREF pxList, const PXSize dataTypeSize, const PXSize startAmount);
PXPublic PXResult PXAPI PXListRelease(PXList PXREF pxList);

// Returns index of element
PXPublic PXBool PXAPI PXListReserve(PXList PXREF pxList, const PXSize sizeToReserve);

PXPublic PXSize PXAPI PXListSizeUsed(const PXList PXREF pxList);

// Check if the given adress exist or is owned by this list
PXPublic PXBool PXAPI PXListIsAddresValid(const PXList PXREF pxList, const void PXREF adress);
PXPublic PXBool PXAPI PXListIsIndexValid(const PXList PXREF pxList, const PXSize index);


//---------------------------------------------------------
// List
//---------------------------------------------------------
PXPublic PXListEntry PXAPI PXListAdd(PXList PXREF pxList, void PXREF dataElement);
//#define PXListAddT(Type, pxList, dataElement) (Type*)PXListAdd(pxList, dataElement)

PXPublic PXResult PXAPI PXListRemove(PXList PXREF pxList, void PXREF dataElement);
#define PXListRemoveT(Type, pxList, dataElement) PXListRemove(pxList, dataElement)

PXPublic PXSize PXAPI PXListItemAmount(const PXList PXREF pxList);

PXPublic void* PXAPI PXListItemAtIndexGet(const PXList PXREF pxList, const PXSize index);
#define PXListItemAtIndexGetT(Type, pxList, index) (Type*)PXListItemAtIndexGet(pxList, index)
//---------------------------------------------------------

//---------------------------------------------------------
// Queue
//---------------------------------------------------------
// Get position of first item
PXPublic PXBool PXAPI PXListEnqueue(PXList PXREF pxList, void PXREF dataElement);
PXPublic void* PXAPI PXListDequeue(PXList PXREF pxList);
//---------------------------------------------------------


//---------------------------------------------------------
// Stack
//---------------------------------------------------------
 

PXPublic PXBool PXAPI PXListAppend(PXList PXREF pxList, void PXREF buffer, const PXSize bufferSize);
// IF the list is a stack, we can reduce the stack by a 
// given amount and extract the data on it if a buffer
// is defined.
PXPublic PXBool PXAPI PXListExtractAndReduce(PXList PXREF pxList, void PXREF buffer, const PXSize bufferSize);

#endif
