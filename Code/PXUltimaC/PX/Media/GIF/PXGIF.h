#pragma once

#ifndef PXGIFIncluded
#define PXGIFIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXGIFImageDescriptor_
{
    PXI16U LeftPosition;
    PXI16U TopPosition;
    PXI16U Width;
    PXI16U Height;
    PXI8U Separator;
    PXI8U LocalColorTableSize;
    PXI8U Reserved;
    PXI8U SortFlag;
    PXI8U InterlaceFlag;
    PXI8U LocalColorTableFlag;
}
PXGIFImageDescriptor;

typedef struct PXGIF_
{
    PXI16U Width;
    PXI16U Height;

    PXI8U BackgroundColorIndex;
    PXI8U PixelAspectRatio;

    PXI8U GlobalColorTableSize;
    PXI8U ColorResolution;

    PXBool IsGlobalColorTablePresent;
    PXBool IsSorted;
}
PXGIF;

PXPublic PXSize PXAPI PXGIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

PXPublic PXResult PXAPI PXGIFLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXGIFSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
