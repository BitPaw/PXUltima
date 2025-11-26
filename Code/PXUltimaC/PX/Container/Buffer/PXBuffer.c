#include "PXBuffer.h"

#include <PX/OS/PXOS.h>

PXResult PXAPI PXBufferAllocate(PXBuffer PXREF pxBuffer, const PXSize size)
{
    pxBuffer->SizeAllocated = size;
    pxBuffer->Adress = PXMemoryHeapCalloc(PXNull, 1, size);

    return PXActionSuccessful;
}

PXResult PXAPI PXBufferResize(PXBuffer PXREF pxBuffer, const PXSize size)
{


    return PXActionSuccessful;
}

PXResult PXAPI PXBufferRelese(PXBuffer PXREF pxBuffer)
{
    PXMemoryHeapFree(PXNull, pxBuffer->Data);

    PXClear(PXBuffer, pxBuffer);

    return PXActionSuccessful;
}

PXResult PXAPI PXBufferSet(PXBuffer PXREF pxBuffer, void* data, const PXSize size)
{
    pxBuffer->Adress = data;
    pxBuffer->SizeAllocated = size;
    pxBuffer->SizeAllowedToUse = size;

    return PXActionSuccessful;
}