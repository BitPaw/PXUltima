#pragma once

#ifndef PXZSTDIncluded
#define PXZSTDIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXZSTDCompress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput, const PXI8U compressionLevel);
PXPublic PXResult PXAPI PXZSTDDecompress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput);

#endif