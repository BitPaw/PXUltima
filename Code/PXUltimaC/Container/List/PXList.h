#ifndef PXListDEFINED
#define PXListDEFINED

#include <Media/PXType.h>

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
PXPublic PXBool PXAPI PXListAdd(PXList* const pxList, void* const dataElement);

#endif