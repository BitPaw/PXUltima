#pragma once

#ifndef PXLZ4Included
#define PXLZ4Included

#include <PX/Engine/PXResource.h>

PXPrivate PXResult PXAPI PXLZ4DecompressChunk(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput);

PXPublic PXResult PXAPI PXLZ4Compress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput);
PXPublic PXResult PXAPI PXLZ4Decompress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput);

#endif