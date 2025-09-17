#ifndef PXSTEPIncluded
#define PXSTEPIncluded

#include <PX/Engine/PXResource.h>

// (St)andard (E)xchange (P)roduct Data (ISO 10303-2xx)
typedef struct PXSTEP_
{
    unsigned int __dummy__;
}
PXSTEP;

PXPublic PXResult PXAPI PXSTEPLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSTEPSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
