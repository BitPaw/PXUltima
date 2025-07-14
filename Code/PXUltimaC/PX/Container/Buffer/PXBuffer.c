#include "PXBuffer.h"

#include <PX/OS/PXOS.h>

PXActionResult PXAPI PXBufferAllocate(PXBuffer* const pxBuffer, const PXSize size)
{
    pxBuffer->Size = size;
    pxBuffer->Data = PXMemoryHeapCalloc(PXNull, 1, size);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXBufferRelese(PXBuffer* const pxBuffer)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXBufferSet(PXBuffer* const pxBuffer, void* data, const PXSize size)
{
    return PXActionSuccessful;
}