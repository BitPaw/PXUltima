#ifndef PXLZ4Include
#define PXLZ4Include

#include "../PXResource.h"

PXPublic PXActionResult PXAPI PXLZ4Compress(PXFile* const pxFileInput, PXFile* const pxFileOutput);
PXPublic PXActionResult PXAPI PXLZ4Decompress(PXFile* const pxFileInput, PXFile* const pxFileOutput);

#endif