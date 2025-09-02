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

PXPublic PXResult PXAPI PXBufferAllocate(PXBuffer* const pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferRelese(PXBuffer* const pxBuffer);
PXPublic PXResult PXAPI PXBufferSet(PXBuffer* const pxBuffer, void* data, const PXSize size);

#endif