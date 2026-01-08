#pragma once

#ifndef PXRTextureIncluded
#define PXRTextureIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXRTextureHeader_
{
    char Signature[4];
    PXI16U Width;
    PXI16U Height;

    PXBool IsExtendedFormat;
    PXI8U ExtendedFormat;
    PXI8U ResourceType;
    PXI8U Flags;
}
PXRTextureHeader;

typedef struct PXRTexture
{
    PXRTextureHeader Header;

    PXI8U BlockSize;
    PXI8U MipMapCount; // 1-16
    PXI8U CompressionFormat;
}
PXRTexture;

PXPublic PXResult PXAPI PXRTextureLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXRTextureSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
