#pragma once

#ifndef PXVRMLIncluded
#define PXVRMLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXVRML_
{
    unsigned int __dummy__;
}
PXVRML;

PXPublic PXResult PXAPI PXVRMLLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXVRMLSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
