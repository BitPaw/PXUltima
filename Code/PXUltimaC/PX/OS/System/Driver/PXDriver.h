#ifndef PXDriverINCLUDE
#define PXDriverINCLUDE

#include <PX/Media/PXResource.h>

typedef struct PXDriver_
{
    char Name[64];
    void* ImageBaseAdress;
}
PXDriver;

PXPublic PXActionResult PXAPI PXDriverListFetch(PXDriver** const pxDriverList, PXSize* const listSize);

#endif