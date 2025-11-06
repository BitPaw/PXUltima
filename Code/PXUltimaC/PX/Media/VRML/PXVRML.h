#pragma once

#ifndef PXVRMLIncluded
#define PXVRMLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXVRML_
{
    unsigned int __dummy__;
}
PXVRML;

PXPublic PXResult PXAPI PXVRMLLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXVRMLSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
