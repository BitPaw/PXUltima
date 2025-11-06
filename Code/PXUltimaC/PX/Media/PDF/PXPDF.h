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

PXPublic PXResult PXAPI PXPDFLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPDFSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
