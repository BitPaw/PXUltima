#ifndef PXLZ4Included
#define PXLZ4Included

#include <PX/Engine/PXResource.h>

PXPrivate PXResult PXAPI  PXLZ4DecompressChunk(PXFile* const pxFileInput, PXFile* const pxFileOutput);

PXPublic PXResult PXAPI PXLZ4Compress(PXFile* const pxFileInput, PXFile* const pxFileOutput);
PXPublic PXResult PXAPI PXLZ4Decompress(PXFile* const pxFileInput, PXFile* const pxFileOutput);

#endif