#ifndef PXBufferDEFINED
#define PXBufferDEFINED

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>

// Container to store data.
// Basically just an array with combined length
typedef struct PXBuffer_
{
    void* Data; // Contains the whole data.
    PXSize Size;
}
PXBuffer;

PXPublic PXResult PXAPI PXBufferAllocate(PXBuffer* const pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferResize(PXBuffer* const pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferRelese(PXBuffer* const pxBuffer);
PXPublic PXResult PXAPI PXBufferSet(PXBuffer* const pxBuffer, void* data, const PXSize size);

#endif