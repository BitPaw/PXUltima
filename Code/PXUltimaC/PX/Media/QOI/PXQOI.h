#ifndef PXQOIIncluded
#define PXQOIIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXQOI_
{
    unsigned int __dummy__;
}
PXQOI;

PXPublic PXResult PXAPI PXQOILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXQOISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
