#include "PXList.h"

#include <OS/Memory/PXMemory.h>

void PXAPI PXListInitialize(PXList* const pxList, const PXSize dataTypeSize, const PXSize sizeToAllocate)
{
    pxList->DataTypeSize = dataTypeSize;
    pxList->AmountUsed = 0;

    if(sizeToAllocate)
    {
        // We want to preallocate memory to prepare space for data
        pxList->Data = PXMemoryHeapCalloc(PXNull, sizeToAllocate, dataTypeSize);
        pxList->AmountAllocated = sizeToAllocate;
    }
    else
    {
        pxList->Data = PXNull;
        pxList->AmountAllocated = 0;
    }
}

void PXAPI PXListRelease(PXList* const pxList)
{
    PXMemoryHeapFree(PXNull, pxList->Data);

    pxList->Data = PXNull;
}

PXBool PXAPI PXListReserve(PXList* const pxList, const PXSize sizeToReserve)
{
    const PXBool isEnoughSpace = sizeToReserve <= pxList->AmountUsed;

    if(isEnoughSpace)
    {
        return PXTrue;
    }

    // Try alloc
    const PXSize newAmount = pxList->GrouthOnAlloc + sizeToReserve;
    const PXSize newSize = pxList->DataTypeSize * newAmount;

    void* newMem = PXMemoryHeapRealloc(PXNull, pxList->Data, newSize);

    if(!newMem)
    {
        return PXFalse;
    }

    pxList->AmountAllocated = newAmount;

    return PXTrue;
}

void* PXAPI PXListAdd(PXList* const pxList, void* const dataElement)
{
    PXListReserve(pxList, pxList->AmountUsed + pxList->GrouthOnAlloc);

    void* target = PXListItemAtIndexGet(pxList, pxList->AmountUsed);

    PXMemoryCopy(dataElement, pxList->DataTypeSize, target, pxList->DataTypeSize);

    ++pxList->AmountUsed;

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
    PXListReserve(pxList, pxList->AmountUsed + pxList->GrouthOnAlloc);

    void* target = PXListItemAtIndexGet(pxList, pxList->AmountUsed);

    PXMemoryCopy(buffer, bufferSize, target, bufferSize);

    pxList->AmountUsed += bufferSize;

    return PXTrue;
}

PXBool PXAPI PXListExtractAndReduce(PXList* const pxList, void* const buffer, const PXSize bufferSize)
{
    char* dataExtractionAdress = (char*)PXListItemAtIndexGet(pxList, pxList->AmountUsed);

    PXMemoryCopy(dataExtractionAdress - bufferSize, bufferSize, buffer, bufferSize);

    pxList->AmountUsed -= bufferSize;

    return PXTrue;
}