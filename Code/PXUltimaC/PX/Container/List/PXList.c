#include "PXList.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

PXResult PXAPI PXListCreate(PXList PXREF pxList, const PXSize dataTypeSize, const PXSize startAmount)
{
    if(!(pxList && dataTypeSize))
    {
        return PXResultRefusedParameterNull;
    }

    PXClear(PXList, pxList);

    if(startAmount)
    {
        // We want to preallocate memory to prepare space for data
        PXResult pxResult = PXBufferAllocate(&pxList->Buffer, dataTypeSize * startAmount);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }

        pxList->EntryAmountAllocated = startAmount;
    }

    pxList->ValueTypeSize = dataTypeSize;

    return PXResultOK;
}

PXResult PXAPI PXListRelease(PXList PXREF pxList)
{
    PXResult pxResult = PXBufferRelese(&pxList->Buffer);

    return pxResult;
}

PXBool PXAPI PXListReserve(PXList PXREF pxList, const PXSize amountOfElements)
{
    const PXBool isEnoughSpace = amountOfElements <= pxList->EntryAmountAllocated;
    PXResult pxResult;

    if(isEnoughSpace)
    {
        return PXResultOK;
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
    const PXBool isInRange = PXMathAdressInRange(pxList->Buffer.Data4, totalSize, adress);

    return isInRange;
}

PXBool PXAPI PXListIsIndexValid(const PXList PXREF pxList, const PXSize index)
{
    return pxList->EntryAmountUsed > index;
}

PXListEntry PXAPI PXListAdd(PXList PXREF pxList, void PXREF dataElement)
{
    PXListEntry pxListEntry;

    PXListReserve(pxList, pxList->EntryAmountUsed + pxList->EntryGrowthOnAllocation);

    void* target = PXListItemAtIndexGet(pxList, pxList->EntryAmountUsed);

    pxListEntry.Address = target;
    pxListEntry.Index = pxList->EntryAmountUsed;

    if(dataElement)
    {
        PXMemoryCopy(dataElement, target, pxList->ValueTypeSize);
    }

    ++pxList->EntryAmountUsed;

    return pxListEntry;
}

PXResult PXAPI PXListRemove(PXList PXREF pxList, void PXREF dataElement)
{
    if(!(pxList && dataElement))
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXSize PXAPI PXListItemAmount(const PXList PXREF pxList)
{
    return pxList->EntryAmountUsed;
}

void* PXAPI PXListItemAtIndexGet(const PXList PXREF pxList, const PXSize index)
{
#if 1 // Additional safety
    const PXBool isInBounce = pxList->EntryAmountUsed >= index;

    if(!isInBounce)
    {
        return PXNull;
    }
#endif

    void PXREF adress = (PXByte*)pxList->Buffer.Data4 + pxList->ValueTypeSize * index;

    return adress;
}

PXBool PXAPI PXListEnqueue(PXList PXREF pxList, void PXREF dataElement)
{
    if(!(pxList && dataElement))
    {
        return PXFalse;
    }

    PXResult pxResult = PXBufferEnsureAdditional(pxList, 1);

    if(PXResultOK != pxResult)
    {
        return PXFalse;
    }

    void* target = pxList->Buffer.Data4 + (pxList->TailIndex * pxList->ValueTypeSize);
    memcpy(target, dataElement, pxList->ValueTypeSize);        

    pxList->TailIndex = (pxList->TailIndex + 1) % pxList->EntryAmountAllocated;
    pxList->EntryAmountUsed++;

    return PXTrue;
}

void* PXAPI PXListDequeue(PXList PXREF pxList)
{
    if(!pxList)
    {
        return PXNull;
    }

    if(!pxList->EntryAmountUsed)
    {
        return PXNull;
    }

    void* source = pxList->Buffer.Data4 + (pxList->HeadIndex * pxList->ValueTypeSize);

    pxList->HeadIndex = (pxList->HeadIndex + 1) % pxList->EntryAmountAllocated;
    pxList->EntryAmountUsed--;

    return source;
}

PXBool PXAPI PXListItemFromStartGet(PXList PXREF pxList, void PXREF dataElement)
{
    return PXResultOK;
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