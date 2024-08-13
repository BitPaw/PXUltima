#ifndef PXPDFINCLUDE
#define PXPDFINCLUDE

#include "../PXResource.h"

typedef struct PXPDF_
{
    PXInt8U VersionMajor;
    PXInt8U VersionMinor;
}
PXPDF;

PXPublic PXActionResult PXAPI PXPDFLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXPDFSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif