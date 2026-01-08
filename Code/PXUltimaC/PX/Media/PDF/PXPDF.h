#pragma once

#ifndef PXPDFIncluded
#define PXPDFIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPDF_
{
    PXI8U VersionMajor;
    PXI8U VersionMinor;
}
PXPDF;

PXPublic PXResult PXAPI PXPDFLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPDFSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
