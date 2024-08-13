#ifndef PXSTEPINCLUDE
#define PXSTEPINCLUDE

#include "../PXResource.h"

// (St)andard (E)xchange (P)roduct Data (ISO 10303-2xx)
typedef struct PXSTEP_
{
    unsigned int __dummy__;
}
PXSTEP;

PXPublic PXActionResult PXAPI PXSTEPLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXSTEPSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif