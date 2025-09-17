#ifndef PXVOBLIncluded
#define PXVOBLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXVOB_
{
    unsigned int __dummy__;
}
PXVOB;

PXPublic PXResult PXAPI PXVOBLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXVOBLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif