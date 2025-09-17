#ifndef PXSVGIncluded
#define PXSVGIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXSVG_
{
    unsigned int __dummy__;
}
PXSVG;

PXPublic PXResult PXAPI PXSVGLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSVGSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
