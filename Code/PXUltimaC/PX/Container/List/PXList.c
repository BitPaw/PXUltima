#include "PXList.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>

void PXAPI PXListInitialize(PXList* const pxList, const PXSize dataTypeSize, const PXSize startAmount)
{
    pxList->DataTypeSize = dataTypeSize;
    pxList->EntryAmountUsed = 0;

    if(startAmount)
    {
        // We want to preallocate memory to prepare space for data
        pxList->Data = PXMemoryHeapCalloc(PXNull, startAmount, dataTypeSize);
        pxList->EntryAmountAllocated = startAmount;
    }
    else
    {
        pxList->Data = PXNull;
        pxList->EntryAmountAllocated = 0;
    }
}

void PXAPI PXListRelease(PXList* const pxList)
{
    PXMemoryHeapFree(PXNull, pxList->Data);

    pxList->Data = PXNull;
}

PXBool PXAPI PXListReserve(PXList* const pxList, const PXSize amountOfElements)
{
    const PXBool isEnoughSpace = amountOfElements <= pxList->EntryAmountAllocated;

    if(isEnoughSpace)
    {
        return PXTrue;
    }

    // Try alloc
    const PXSize newAmount = pxList->EntryGrowthOnAllocation + pxList->EntryAmountAllocated;
    const PXSize newSize = pxList->DataTypeSize * newAmount;

    void* newMem = PXMemoryHeapRealloc(PXNull, pxList->Data, newSize);

    if(!newMem)
    {
        return PXFalse;
    }

    pxList->Data = newMem;
    pxList->EntryAmountAllocated = newAmount;

    return PXTrue;
}

void* PXAPI PXListAdd(PXList* const pxList, void* const dataElement)
{
    PXListReserve(pxList, pxList->EntryAmountUsed + pxList->EntryGrowthOnAllocation);

    void* target = PXListItemAtIndexGet(pxList, pxList->EntryAmountUsed);

    PXMemoryCopy(dataElement, pxList->DataTypeSize, target, pxList->DataTypeSize);

    ++pxList->EntryAmountUsed;

    return target;
}

void* PXAPI PXListItemAtIndexGet(PXList* const pxList, const PXSize index)
{
#if 0 // Additional safety
    const PXBool isInBounce = pxList->AmountUsed >= index;

    if(!isInBounce)
    {
        return PXNull;
    }
#endif

    void* const adress = (char*)pxList->Data + pxList->DataTypeSize * index;

    return adress;
}

PXBool PXAPI PXListItemFromStartGet(PXList* const queue, void* const dataElement)
{
    return PXActionSuccessful;
}

PXBool PXAPI PXListAppend(PXList* const pxList, void* const buffer, const PXSize bufferSize)
{
    PXListReserve(pxList, pxList->EntryAmountUsed + pxList->EntryGrowthOnAllocation);

    void* target = PXListItemAtIndexGet(pxList, pxList->EntryAmountUsed);

    PXMemoryCopy(buffer, bufferSize, target, bufferSize);

    pxList->EntryAmountUsed += bufferSize;

    return PXTrue;
}

PXBool PXAPI PXListExtractAndReduce(PXList* const pxList, void* const buffer, const PXSize bufferSize)
{
    char* dataExtractionAdress = (char*)PXListItemAtIndexGet(pxList, pxList->EntryAmountUsed);

    PXMemoryCopy(dataExtractionAdress - bufferSize, bufferSize, buffer, bufferSize);

    pxList->EntryAmountUsed -= bufferSize;

    return PXTrue;
}