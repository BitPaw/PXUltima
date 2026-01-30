#include "PXBuffer.h"

#include <PX/OS/PXOS.h>

PXResult PXAPI PXBufferAllocate(PXBuffer PXREF pxBuffer, const PXSize size)
{
    pxBuffer->SizeAllocated = size;
    pxBuffer->SizeAllowedToUse = size;
    pxBuffer->Adress = PXMemoryHeapCalloc(PXNull, 1, size);
    pxBuffer->IsOwned = PXTrue;

    PXMemorySet(pxBuffer->Adress, 0xFF, size);

    return PXResultOK;
}

PXResult PXAPI PXBufferEnsure(PXBuffer PXREF pxBuffer, const PXSize size)
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

    return PXBufferResize(pxBuffer, size);
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
    pxBuffer->Adress = data;
    pxBuffer->SizeAllocated = size;
    pxBuffer->SizeAllowedToUse = size;

    return PXResultOK;
}

PXResult PXAPI PXBufferCopy(PXBuffer PXREF pxBuffer, void* data, const PXSize size)
{
    PXCopyList(PXByte, size, data, pxBuffer->Adress);

    return PXResultOK;
}