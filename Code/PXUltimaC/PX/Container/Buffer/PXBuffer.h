#ifndef PXBufferDEFINED
#define PXBufferDEFINED

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>

// NO STATE!
typedef struct PXBuffer_
{
    void* Data;
    PXSize Size;
}
PXBuffer;

PXPublic PXActionResult PXAPI PXBufferAllocate(PXBuffer* const pxBuffer, const PXSize size);
PXPublic PXActionResult PXAPI PXBufferRelese(PXBuffer* const pxBuffer);
PXPublic PXActionResult PXAPI PXBufferSet(PXBuffer* const pxBuffer, void* data, const PXSize size);

#endif