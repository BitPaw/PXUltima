#include "PXBuffer.h"

#include <PX/OS/PXOS.h>

PXResult PXAPI PXBufferAllocate(PXBuffer PXREF pxBuffer, const PXSize size)
{
    pxBuffer->Size = size;
    pxBuffer->Data = PXMemoryHeapCalloc(PXNull, 1, size);

    return PXActionSuccessful;
}

PXResult PXAPI PXBufferResize(PXBuffer PXREF pxBuffer, const PXSize size)
{


    return PXActionSuccessful;
}

PXResult PXAPI PXBufferRelese(PXBuffer PXREF pxBuffer)
{
    PXMemoryHeapFree(PXNull, pxBuffer->Data);

    pxBuffer->Data = 0;
    pxBuffer->Size = 0;

    return PXActionSuccessful;
}

PXResult PXAPI PXBufferSet(PXBuffer PXREF pxBuffer, void* data, const PXSize size)
{
    return PXActionSuccessful;
}