#pragma once

#ifndef PXMatroskaIncluded
#define PXMatroskaIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMatroska_
{
    unsigned int __dummy__;
}
PXMatroska;

PXPublic PXResult PXAPI PXMatroskaLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMatroskaSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
