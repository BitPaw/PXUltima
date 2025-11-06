#include "PXList.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

void PXAPI PXListInitialize(PXList PXREF pxList, const PXSize dataTypeSize, const PXSize startAmount)
{
    PXClear(PXList, pxList);

    pxList->ValueTypeSize = dataTypeSize;

    if(startAmount)
    {
        // We want to preallocate memory to prepare space for data
        PXBufferAllocate(&pxList->Buffer, startAmount);
        pxList->EntryAmountAllocated = startAmount;
    }
}

void PXAPI PXListRelease(PXList PXREF pxList)
{
    PXBufferRelese(&pxList->Buffer);
}

PXBool PXAPI PXListReserve(PXList PXREF pxList, const PXSize amountOfElements)
{
    const PXBool isEnoughSpace = amountOfElements <= pxList->EntryAmountAllocated;
    PXResult pxResult;

    if(isEnoughSpace)
    {
        return PXActionSuccessful;
    }

    // If new list
    if(0 == pxList->EntryAmountAllocated)
    {
        pxResult = PXBufferAllocate(&pxList->Buffer, pxList->ValueTypeSize * amountOfElements);

        pxList->EntryAmountAllocated = amountOfElements;
    }
    else
    {
        // Try alloc
        const PXSize newAmount = pxList->EntryGrowthOnAllocation + pxList->EntryAmountAllocated;
        const PXSize newSize = pxList->ValueTypeSize * newAmount;

        pxResult = PXBufferResize(&pxList->Buffer, newSize);

        pxList->EntryAmountAllocated = newAmount;
    }

    return pxResult;
}

PXSize PXAPI PXListSizeUsed(const PXList PXREF pxList)
{
    if(!pxList)
    {
        return 0;
    }

    return pxList->EntryAmountUsed * pxList->ValueTypeSize;
}

PXBool PXAPI PXListIsAddresValid(const PXList PXREF pxList, const void PXREF adress)
{
    const PXSize totalSize = PXListSizeUsed(pxList);
    const PXBool isInRange = PXMathAdressInRange(pxList->Buffer.Data, totalSize, adress);

    return isInRange;
}

PXBool PXAPI PXListIsIndexValid(const PXList PXREF pxList, const PXSize index)
{
    return pxList->EntryAmountUsed > index;
}

void* PXAPI PXListAdd(PXList PXREF pxList, void PXREF dataElement)
{
    PXListReserve(pxList, pxList->EntryAmountUsed + pxList->EntryGrowthOnAllocation);

    void* target = PXListItemAtIndexGet(pxList, pxList->EntryAmountUsed);

    PXMemoryCopy(dataElement, target, pxList->ValueTypeSize);

    ++pxList->EntryAmountUsed;

    return target;
}

void* PXAPI PXListItemAtIndexGet(PXList PXREF pxList, const PXSize index)
{
#if 0 // Additional safety
    const PXBool isInBounce = pxList->AmountUsed >= index;

    if(!isInBounce)
    {
        return PXNull;
    }
#endif

    void PXREF adress = (char*)pxList->Buffer.Data + pxList->ValueTypeSize * index;

    return adress;
}

PXBool PXAPI PXListItemFromStartGet(PXList PXREF queue, void PXREF dataElement)
{
    return PXActionSuccessful;
}

PXBool PXAPI PXListAppend(PXList PXREF pxList, void PXREF buffer, const PXSize bufferSize)
{
    PXListReserve(pxList, pxList->EntryAmountUsed + pxList->EntryGrowthOnAllocation);

    void* target = PXListItemAtIndexGet(pxList, pxList->EntryAmountUsed);

    PXMemoryCopy(buffer, target, bufferSize);

    pxList->EntryAmountUsed += bufferSize;

    return PXTrue;
}

PXBool PXAPI PXListExtractAndReduce(PXList PXREF pxList, void PXREF buffer, const PXSize bufferSize)
{
    char* dataExtractionAdress = (char*)PXListItemAtIndexGet(pxList, pxList->EntryAmountUsed);

    PXMemoryCopy(dataExtractionAdress - bufferSize, buffer, bufferSize);

    pxList->EntryAmountUsed -= bufferSize;

    return PXTrue;
}