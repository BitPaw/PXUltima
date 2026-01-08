#pragma once

#ifndef PXFBXIncluded
#define PXFBXIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXFilmBox_
{
    PXI32U Version;
}
PXFilmBox;

PXPublic PXResult PXAPI PXFilmBoxLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXFilmBoxSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
