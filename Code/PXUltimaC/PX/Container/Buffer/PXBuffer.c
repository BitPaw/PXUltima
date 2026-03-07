#include "PXBuffer.h"

#include <PX/OS/PXOS.h>


PXBool PXAPI PXBufferIsInBounce(const PXBuffer PXREF pxBuffer)
{
    return pxBuffer->CursorOffsetByte < pxBuffer->SizeAllowedToUse;
}

PXByte* PXAPI PXBufferData(const PXBuffer PXREF pxBuffer)
{
    const PXBool isOK = PXBufferIsInBounce(pxBuffer);

    if(!isOK)
    {
        return PXNull;
    }

    return &pxBuffer->Data[pxBuffer->CursorOffsetByte];
}

PXSize PXAPI PXBufferRemaining(const PXBuffer PXREF pxBuffer)
{
    const PXBool isOK = PXBufferIsInBounce(pxBuffer);

    if(!isOK)
    {
        return 0;
    }

    return pxBuffer->SizeAllowedToUse - pxBuffer->CursorOffsetByte;
}

PXResult PXAPI PXBufferAllocate(PXBuffer PXREF pxBuffer, const PXSize size)
{
    pxBuffer->SizeAllocated = size;
    pxBuffer->SizeAllowedToUse = size;
    pxBuffer->Adress = PXMemoryHeapCalloc(PXNull, 1, size);
    pxBuffer->IsOwned = PXTrue;

    //PXMemorySet(pxBuffer->Adress, 0xFF, size);

    return PXResultOK;
}

PXResult PXAPI PXBufferEnsureTotal(PXBuffer PXREF pxBuffer, const PXSize size)
{
    if(!pxBuffer->Data) // Do we have data?
    {
        return PXBufferAllocate(pxBuffer, size);
    }

    // is Big enough?
    if(pxBuffer->SizeAllocated >= size)
    {
        return PXResultOK;
    }

    if(!pxBuffer->IsOwned)
    {
        return PXResultRefusedNotEnoughMemory;
    }

    return PXBufferResize(pxBuffer, size);
}

PXResult PXAPI PXBufferEnsureAdditional(PXBuffer PXREF pxBuffer, const PXSize sizeAdditionalData)
{
    PXSize requiredTotalFreeSize = pxBuffer->SizeAllowedToUse + sizeAdditionalData;

    PXResult pxResult = PXBufferEnsureTotal(pxBuffer, requiredTotalFreeSize);

    return pxResult;
}

PXResult PXAPI PXBufferResize(PXBuffer PXREF pxBuffer, const PXSize size)
{
    pxBuffer->Adress = PXMemoryHeapRealloc(PXNull, pxBuffer->Data, size);
    pxBuffer->SizeAllocated = size;    
    pxBuffer->IsOwned = PXTrue;

    return PXResultOK;
}

PXResult PXAPI PXBufferRelese(PXBuffer PXREF pxBuffer)
{
    if(pxBuffer->IsOwned)
    {
        PXMemoryHeapFree(PXNull, pxBuffer->Data);      
    }

    PXClear(PXBuffer, pxBuffer);

    return PXResultOK;
}

PXResult PXAPI PXBufferSet(PXBuffer PXREF pxBuffer, void* data, const PXSize size)
{
    PXClear(PXBuffer, pxBuffer);

    pxBuffer->Adress = data;
    pxBuffer->SizeAllocated = size;
    pxBuffer->SizeAllowedToUse = size;
    pxBuffer->IsOwned = PXFalse;

    return PXResultOK;
}

PXResult PXAPI PXBufferCopy(PXBuffer PXREF pxBuffer, void* data, const PXSize size)
{
    PXCopyList(PXByte, size, data, pxBuffer->Adress);

    return PXResultOK;
}

PXResult PXAPI PXBufferAppend(PXBuffer PXREF pxBuffer, void* data, const PXSize size)
{
    if(!(pxBuffer && data && size))
    {
        return PXResultRefusedParameterNull;
    }

    PXBool didAllocate = PXFalse;

    for(;;)
    {
        PXByte* dataInsert = PXBufferData(pxBuffer);
        PXSize remainingSize = PXBufferRemaining(pxBuffer);

        PXBool enoughSpace = remainingSize >= size;

        if(!enoughSpace)
        {
            if(didAllocate)
            {
                return PXResultRefusedNoPlaceToStore;
            }

            const PXSize missingSpace = pxBuffer->CursorOffsetByte + size;
            PXResult pxResult = PXBufferEnsureTotal(pxBuffer, missingSpace);

            // If this didnt work, yeet
            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            didAllocate = PXTrue;

            continue;
        }

        pxBuffer->CursorOffsetByte += PXMemoryCopy(dataInsert, data, size);
        break;
    }

    return PXResultOK;
}