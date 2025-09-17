#ifndef PXPLYIncluded
#define PXPLYIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPLY_
{
    unsigned int __dummy__;
}
PXPLY;

PXPublic PXResult PXAPI PXPLYLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPLYSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
