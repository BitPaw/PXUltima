#ifndef PXZSTDIncluded
#define PXZSTDIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXZSTDCompress(PXFile* const pxFileInput, PXFile* const pxFileOutput, const PXI8U compressionLevel);
PXPublic PXResult PXAPI PXZSTDDecompress(PXFile* const pxFileInput, PXFile* const pxFileOutput);

#endif