#ifndef PXAACIncluded
#define PXAACIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXAAC_
{
    unsigned int __dummy__;
}
PXAAC;

PXPublic PXResult PXAPI PXAACLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXAACSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
