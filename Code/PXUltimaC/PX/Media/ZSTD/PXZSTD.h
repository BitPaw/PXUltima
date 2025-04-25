#ifndef PXZSTDINCLUDE
#define PXZSTDINCLUDE

#include "../PXResource.h"

PXPublic PXActionResult PXAPI PXZSTDCompress(PXFile* const pxFileInput, PXFile* const pxFileOutput, const PXInt8U compressionLevel);
PXPublic PXActionResult PXAPI PXZSTDDecompress(PXFile* const pxFileInput, PXFile* const pxFileOutput);

#endif