#ifndef PXWEBMIncluded
#define PXWEBMIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXWEBM_
{
    unsigned int __dummy__;
}
PXWEBM;

PXPublic PXResult PXAPI PXWEBMLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWEBMSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
