#include "PXBuffer.h"

#include <PX/OS/PXOS.h>

PXResult PXAPI  PXBufferAllocate(PXBuffer* const pxBuffer, const PXSize size)
{
    pxBuffer->Size = size;
    pxBuffer->Data = PXMemoryHeapCalloc(PXNull, 1, size);

    return PXActionSuccessful;
}

PXResult PXAPI PXBufferResize(PXBuffer* const pxBuffer, const PXSize size)
{


    return PXActionSuccessful;
}

PXResult PXAPI  PXBufferRelese(PXBuffer* const pxBuffer)
{
    PXMemoryHeapFree(PXNull, pxBuffer->Data);

    pxBuffer->Data = 0;
    pxBuffer->Size = 0;

    return PXActionSuccessful;
}

PXResult PXAPI  PXBufferSet(PXBuffer* const pxBuffer, void* data, const PXSize size)
{
    return PXActionSuccessful;
}