#pragma once

#ifndef PXFastFileIncluded
#define PXFastFileIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXFastFile_
{
    PXI32U Version;
}
PXFastFile;

PXPublic PXResult PXAPI PXFastFileLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXFastFileSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
