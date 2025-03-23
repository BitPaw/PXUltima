#ifndef PXMSIINCLUDE
#define PXMSIINCLUDE

#include "../PXResource.h"

typedef struct PXMSI_
{
    int x;
}
PXMSI;

PXPublic PXActionResult PXAPI PXMSILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXMSISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
