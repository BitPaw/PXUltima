#ifndef PXPHPIncluded
#define PXPHPIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPHP_
{
    int x;
}
PXPHP;

PXPublic PXResult PXAPI PXPHPLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPHPSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
