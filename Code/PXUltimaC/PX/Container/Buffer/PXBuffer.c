#include "PXBuffer.h"

#include <PX/OS/PXOS.h>
#include <PX/Math/PXMath.h>


PXBool PXAPI PXBufferIsInBounce(const PXBuffer PXREF pxBuffer)
{
    return pxBuffer->CursorOffsetByte < pxBuffer->SizeAllowedToUse;
}

PXBool PXAPI PXBufferIsInBounceWithOffset(const PXBuffer PXREF pxBuffer, const PXSize offset)
{
    return (pxBuffer->CursorOffsetByte + offset) < pxBuffer->SizeAllowedToUse;
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

PXByte* PXAPI PXBufferDataWithOffset(const PXBuffer PXREF pxBuffer, const PXSize offset)
{
    const PXBool isOK = PXBufferIsInBounceWithOffset(pxBuffer, offset);

    if(!isOK)
    {
        return PXNull;
    }

    return &pxBuffer->Data[offset];
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
    pxBuffer->Address = PXMemoryHeapCalloc(PXNull, 1, size);
    pxBuffer->IsOwned = PXTrue;

    //PXMemorySet(pxBuffer->Address, 0xFF, size);

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
    if(!pxBuffer)
    {
        return PXResultRefusedParameterNull;
    }

    pxBuffer->Address = PXMemoryHeapRealloc(PXNull, pxBuffer->Data, size);
    pxBuffer->SizeAllowedToUse = size;
    pxBuffer->SizeAllocated = size;    
    pxBuffer->IsOwned = PXTrue;

    return PXResultOK;
}

PXResult PXAPI PXBufferRelease(PXBuffer PXREF pxBuffer)
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

    pxBuffer->Address = data;
    pxBuffer->SizeAllocated = size;
    pxBuffer->SizeAllowedToUse = size;
    pxBuffer->IsOwned = PXFalse;

    return PXResultOK;
}

PXResult PXAPI PXBufferCopy(PXBuffer PXREF pxBuffer, void* data, const PXSize size)
{
    PXCopyList(PXByte, size, data, pxBuffer->Address);

    return PXResultOK;
}

PXResult PXAPI PXBufferAppend(PXBuffer PXREF pxBuffer, const void* data, const PXSize size, void** out)
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

        pxBuffer->CursorOffsetByte += PXMemoryCopy(data, dataInsert, size);

        if(out)
        {
            *out = dataInsert;
        }

        break;
    }

    return PXResultOK;
}

PXResult PXAPI PXBufferCursorMove(PXBuffer PXREF pxBuffer, const PXI64S move)
{
    pxBuffer->CursorOffsetByte += move;

    pxBuffer->CursorOffsetByte = PXMathMinimumIU(pxBuffer->CursorOffsetByte, pxBuffer->SizeAllowedToUse);

    return PXResultOK;
}

PXResult PXAPI PXBufferStore(PXBuffer PXREF pxBuffer, const void* data, const PXSize sizeTotal, const PXBool isDataConst)
{
    PXResult pxResult = PXResultInvalid;

    if(isDataConst)
    {
        pxResult = PXBufferSet(pxBuffer, data, sizeTotal);

        return pxResult;
    }

    pxResult = PXBufferAllocate(pxBuffer, sizeTotal);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxResult = PXBufferCopy(pxBuffer, data, sizeTotal);

    return pxResult;
}