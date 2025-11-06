#pragma once

#ifndef PXFastFileIncluded
#define PXFastFileIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXFastFile_
{
    PXI32U Version;
}
PXFastFile;

PXPublic PXResult PXAPI PXFastFileLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXFastFileSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
