#pragma once

#ifndef PXFBXIncluded
#define PXFBXIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXFilmBox_
{
    PXI32U Version;
}
PXFilmBox;

PXPublic PXResult PXAPI PXFilmBoxLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXFilmBoxSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
