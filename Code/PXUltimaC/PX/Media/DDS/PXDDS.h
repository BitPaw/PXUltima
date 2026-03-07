#pragma once

#ifndef PXDDSIncluded
#define PXDDSIncluded

#include <PX/Engine/PXResource.h>



typedef struct PXDDSHeader_
{
    PXI32U Size;
    PXI32U Flags;
    PXI32U Height;
    PXI32U Width;
    PXI32U PitchOrLinearSize;
    PXI32U Depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
    PXI32U MipMapCount;
    char Name[44];
    PXI32U PixelFormatSize;
    PXI32U PixelFormatFlags;
    char PixelFormatID[4];  // DDS_PIXELFORMAT
    PXI32U RGBBitCount;
    PXI32U RBitMask;
    PXI32U GBitMask;
    PXI32U BBitMask;
    PXI32U ABitMask;
    PXI32U Caps1;
    PXI32U Caps2;
    PXI32U Caps3;
    PXI32U Caps4;
    PXI32U Reserved2;

#if 1
    // DDS_HEADER_DXT10 extension
    PXI32U GIFormatID; // DXGI_FORMAT
    PXI32U resourceDimension;
    PXI32U miscFlag; // See D3D11_RESOURCE_MISC_FLAG
    PXI32U arraySize;


    // Extra
#if OSUnix
#elif OSWindows
    //DXGI_FORMAT Format;
#endif

    PXI8U BitPerPixel;

    PXBool IsDX10;
#endif
} 
PXDDSHeader;

PXPublic PXColorFormat PXAPI PXDDSTypeDetect(const char* signature);

PXPublic PXResult PXAPI PXDDSLoadFromFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXDDSSaveToFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
