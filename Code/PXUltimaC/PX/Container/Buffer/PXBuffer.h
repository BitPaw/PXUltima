#pragma once

#ifndef PXBufferDEFINED
#define PXBufferDEFINED

#include <PX/OS/Error/PXActionResult.h>

// Container to store data.
// Basically just an array with combined length
typedef struct PXBuffer_
{
    void* Data; // Contains the whole data.
    PXSize Size;
}
PXBuffer;

PXPublic PXResult PXAPI PXBufferAllocate(PXBuffer PXREF pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferResize(PXBuffer PXREF pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferRelese(PXBuffer PXREF pxBuffer);
PXPublic PXResult PXAPI PXBufferSet(PXBuffer PXREF pxBuffer, void* data, const PXSize size);

#endif