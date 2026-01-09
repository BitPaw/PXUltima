#include "PXDDS.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

const char PXDDSSignature[4] = { 'D', 'D', 'S', ' ' }; // 0x20534444

//--------------------------------------------------------------------------------------
// File: DDSTextureLoader.cpp
//
// Function for loading a DDS texture and creating a Direct3D 11 runtime resource for it
//
// Note this function is useful as a light-weight runtime loader for DDS files. For
// a full-featured DDS file reader, writer, and texture processing pipeline, see
// the 'Texconv' sample and the 'DirectXTex' library.
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------


#if OSUnix
#elif OSWindows
#include <dxgiformat.h>
#endif


//--------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((uint32)(byte)(ch0) | ((uint32)(byte)(ch1) << 8) | ((uint32)(byte)(ch2) << 16) | ((uint32)(byte)(ch3) << 24))
#endif /* defined(MAKEFOURCC) */

typedef struct PXDDSPixelFormat_
{
    PXI32U  size;
    PXI32U  flags;
    PXI32U  fourCC;
    PXI32U  RGBBitCount;
    PXI32U  RBitMask;
    PXI32U  GBitMask;
    PXI32U  BBitMask;
    PXI32U  ABitMask;
}
PXDDSPixelFormat;

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_RGBA        0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_LUMINANCEA  0x00020001  // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define DDS_PAL8        0x00000020  // DDPF_PALETTEINDEXED8

#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH          0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  // DDSD_LINEARSIZE

#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES (DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                              DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                              DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ)

#define DDS_CUBEMAP 0x00000200 // DDSCAPS2_CUBEMAP

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

typedef struct PXDirectDrawTexture_
{
    PXI32U Size;
    PXI32U Flags;
    PXI32U Height;
    PXI32U Width;
    PXI32U pitchOrLinearSize;
    PXI32U Depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
    PXI32U MipMapCount;
    PXI32U ddspf; // DDS_PIXELFORMAT
    PXI32U caps;
    PXI32U caps2;
    PXI32U caps3;
    PXI32U caps4;

    // DDS_HEADER_DXT10 extension
    PXI32U GIFormatID; // DXGI_FORMAT
    PXI32U resourceDimension;
    PXI32U miscFlag; // See D3D11_RESOURCE_MISC_FLAG
    PXI32U arraySize;


    // Extra
#if OSUnix
#elif OSWindows
    DXGI_FORMAT Format;
#endif

    PXI8U BitPerPixel;

    PXBool IsDX10;
}
PXDirectDrawTexture;


#if OSUnix
#elif OSWindows

static PXI8U PXDXGIBitsPerPixel(DXGI_FORMAT fmt)
{
    switch(fmt)
    {
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return 128;

    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return 96;

    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return 64;

    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
    case DXGI_FORMAT_R11G11B10_FLOAT:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return 32;

    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:
        //case DXGI_FORMAT_B4G4R4A4_UNORM:
        return 16;

    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    case DXGI_FORMAT_R8_SINT:
    case DXGI_FORMAT_A8_UNORM:
        return 8;

    case DXGI_FORMAT_R1_UNORM:
        return 1;

    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        return 4;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return 8;

    default:
        return 0;
    }
}

#define ISBITMASK(r, g, b, a) (ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a)

DXGI_FORMAT PXDXGIFormatGet(PXDirectDrawTexture PXREF pxDirectDrawTexture)
{
#if 0
    if(pxDirectDrawTexture->Flags & DDS_RGB)
    {
        // Note that sRGB formats are written using the "DX10" extended header.

        switch(pxDirectDrawTexture->RGBBitCount)
        {
        case 32:
            if(ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
            {
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            }

            if(ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
            {
                return DXGI_FORMAT_B8G8R8A8_UNORM;
            }

            if(ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000))
            {
                return DXGI_FORMAT_B8G8R8X8_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000) aka D3DFMT_X8B8G8R8

            // Note that many common DDS reader/writers (including D3DX) swap the
            // the RED/BLUE masks for 10:10:10:2 formats. We assumme
            // below that the 'backwards' header mask is being used since it is most
            // likely written by D3DX. The more robust solution is to use the 'DX10'
            // header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

            // For 'correct' writers, this should be 0x000003ff, 0x000ffc00, 0x3ff00000 for RGB data
            if(ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
            {
                return DXGI_FORMAT_R10G10B10A2_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000) aka D3DFMT_A2R10G10B10

            if(ISBITMASK(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
            {
                return DXGI_FORMAT_R16G16_UNORM;
            }

            if(ISBITMASK(0xffffffff, 0x00000000, 0x00000000, 0x00000000))
            {
                // Only 32-bit color channel format in D3D9 was R32F.
                return DXGI_FORMAT_R32_PXF32; // D3DX writes this out as a FourCC of 114.
            }
            break;

        case 24:
            // No 24bpp DXGI formats aka D3DFMT_R8G8B8
            break;

        case 16:
            if(ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x8000))
            {
                return DXGI_FORMAT_B5G5R5A1_UNORM;
            }
            if(ISBITMASK(0xf800, 0x07e0, 0x001f, 0x0000))
            {
                return DXGI_FORMAT_B5G6R5_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x0000) aka D3DFMT_X1R5G5B5.
            if(ISBITMASK(0x0f00, 0x00f0, 0x000f, 0xf000))
            {
                return DXGI_FORMAT_B4G4R4A4_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x0f00, 0x00f0, 0x000f, 0x0000) aka D3DFMT_X4R4G4B4.

            // No 3:3:2, 3:3:2:8, or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_R3G3B2, D3DFMT_P8, D3DFMT_A8P8, etc.
            break;
        }
    }
    else if(pxDirectDrawTexture->Flags & DDS_LUMINANCE)
    {
        if(8 == ddpf.RGBBitCount)
        {
            if(ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x00000000))
            {
                return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
            }

            // No DXGI format maps to ISBITMASK(0x0f, 0x00, 0x00, 0xf0) aka D3DFMT_A4L4.
        }

        if(16 == ddpf.RGBBitCount)
        {
            if(ISBITMASK(0x0000ffff, 0x00000000, 0x00000000, 0x00000000))
            {
                return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension.
            }
            if(ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x0000ff00))
            {
                return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension.
            }
        }
    }
    else if(pxDirectDrawTexture->Flags & DDS_ALPHA)
    {
        if(8 == 15) // PXDDSPixelFormat_RGBBitCount
        {
            return DXGI_FORMAT_A8_UNORM;
        }
    }
    else if(pxDirectDrawTexture->Flags & DDS_FOURCC)
    {
        switch(pxDirectDrawTexture->ddspf)
        {
        case PXI32Make('D', 'X', 'T', '1'):
            return DXGI_FORMAT_BC1_UNORM;
        case PXI32Make('D', 'X', 'T', '3'):
            return DXGI_FORMAT_BC2_UNORM;
        case PXI32Make('D', 'X', 'T', '5'):
            return DXGI_FORMAT_BC3_UNORM;
        // While pre-mulitplied alpha isn't directly supported by the DXGI formats,
        // they are basically the same as these BC formats so they can be mapped
        case PXI32Make('D', 'X', 'T', '2'):
            return DXGI_FORMAT_BC2_UNORM;
        case PXI32Make('D', 'X', 'T', '4'):
            return DXGI_FORMAT_BC3_UNORM;
        case PXI32Make('B', 'C', '4', 'U'):
        case PXI32Make('A', 'T', 'I', '1'):
            return DXGI_FORMAT_BC4_UNORM;
        case PXI32Make('B', 'C', '4', 'S'):
            return DXGI_FORMAT_BC4_SNORM;
        case PXI32Make('A', 'T', 'I', '2'):
        case PXI32Make('B', 'C', '5', 'U'):
            return DXGI_FORMAT_BC5_UNORM;
        case PXI32Make('B', 'C', '5', 'S'):
            return DXGI_FORMAT_BC5_SNORM;

        // BC6H and BC7 are written using the "DX10" extended header
        case PXI32Make('R', 'G', 'B', 'G'):
            return DXGI_FORMAT_R8G8_B8G8_UNORM;

        default:
            break;
        }



        // Check for D3DFORMAT enums being set here.
        switch(pxDirectDrawTexture->ddspf)
        {
        case 36: // D3DFMT_A16B16G16R16
            return DXGI_FORMAT_R16G16B16A16_UNORM;

        case 110: // D3DFMT_Q16W16V16U16
            return DXGI_FORMAT_R16G16B16A16_SNORM;

        case 111: // D3DFMT_R16F
            return DXGI_FORMAT_R16_PXF32;

        case 112: // D3DFMT_G16R16F
            return DXGI_FORMAT_R16G16_PXF32;

        case 113: // D3DFMT_A16B16G16R16F
            return DXGI_FORMAT_R16G16B16A16_PXF32;

        case 114: // D3DFMT_R32F
            return DXGI_FORMAT_R32_PXF32;

        case 115: // D3DFMT_G32R32F
            return DXGI_FORMAT_R32G32_PXF32;

        case 116: // D3DFMT_A32B32G32R32F
            return DXGI_FORMAT_R32G32B32A32_PXF32;
        }
    }

    return DXGI_FORMAT_UNKNOWN;
#endif
}

#endif




/*

    _In_ ID3D11Device* d3dDevice,
    _In_reads_bytes_(ddsDataSize) const byte* ddsData,
    _In_ size_t ddsDataSize,
    _Out_opt_ ID3D11Resource** texture,
    _Out_opt_ ID3D11ShaderResourceView** textureView,
    _In_ size_t maxsize

*/

PXResult PXAPI PXDDSLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXTexture PXREF pxTexture = (PXTexture*)pxResourceLoadInfo->ResourceTarget;

    if(!pxTexture)
    {
        return PXResultOK;
    }

    PXDirectDrawTexture pxDirectDrawTexture;
    PXClear(PXDirectDrawTexture, &pxDirectDrawTexture);
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

    // Check header signature
    {
        const PXBool valid = PXFileReadAndCompare(pxFile, PXDDSSignature, sizeof(PXDDSSignature));

        if(!valid)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    // Read Header
    {
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&pxDirectDrawTexture.Size,PXTypeInt32ULE},
            {&pxDirectDrawTexture.Flags,PXTypeInt32ULE},
            {&pxDirectDrawTexture.Height,PXTypeInt32ULE},
            {&pxDirectDrawTexture.Width,PXTypeInt32ULE},
            {&pxDirectDrawTexture.pitchOrLinearSize,PXTypeInt32ULE},
            {&pxDirectDrawTexture.Depth,PXTypeInt32ULE},
            {&pxDirectDrawTexture.MipMapCount,PXTypeInt32ULE},
            {PXNull, PXTypePadding(sizeof(PXI32U) * 11u)},
            {&pxDirectDrawTexture.ddspf,PXTypeInt32ULE},
            {&pxDirectDrawTexture.caps,PXTypeInt32ULE},
            {&pxDirectDrawTexture.caps2,PXTypeInt32ULE},
            {&pxDirectDrawTexture.caps3,PXTypeInt32ULE},
            {&pxDirectDrawTexture.caps4,PXTypeInt32ULE},
            {PXNull, PXTypePadding(sizeof(PXI32U) * 2u)}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

#if OSUnix
#elif OSWindows
        pxDirectDrawTexture.Format = PXDXGIFormatGet(&pxDirectDrawTexture);
#endif

        pxDirectDrawTexture.IsDX10 = PXI32Make('D', 'X', '1', '0') == pxDirectDrawTexture.ddspf;

        if(pxDirectDrawTexture.MipMapCount == 0)
        {
            pxDirectDrawTexture.MipMapCount = 1;
        }
    }

    // Parse extended header
    if(pxDirectDrawTexture.IsDX10)
    {
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&pxDirectDrawTexture.GIFormatID,PXTypeInt32ULE},
            {&pxDirectDrawTexture.resourceDimension,PXTypeInt32ULE},
            {&pxDirectDrawTexture.miscFlag,PXTypeInt32ULE},
            {&pxDirectDrawTexture.arraySize,PXTypeInt32ULE},
            {PXNull, PXTypePadding(sizeof(PXI32U))}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

#if OSWindows
        pxDirectDrawTexture.BitPerPixel = PXDXGIBitsPerPixel(pxDirectDrawTexture.GIFormatID);
#endif
    }


    //uint32 resDim = D3D11_RESOURCE_DIMENSION_UNKNOWN;
    //  size_t arraySize = 1;
    //  DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
    //  bool isCubeMap = false;


    if(pxDirectDrawTexture.IsDX10)
    {
        switch(pxDirectDrawTexture.resourceDimension)
        {
        case 2: // D3D10_RESOURCE_DIMENSION_TEXTURE1D:
        {
            pxTexture->Type = PXTextureType1D;

            // D3DX writes 1D textures with a fixed Height of 1.
            if((pxDirectDrawTexture.Flags & DDS_HEIGHT) && pxDirectDrawTexture.Height != 1)
            {
                return PXResultInvalid;
            }

            break;
        }
        case 3: // D3D10_RESOURCE_DIMENSION_TEXTURE2D:
        {
            if(pxDirectDrawTexture.miscFlag & 4) // D3D11_RESOURCE_MISC_TEXTURECUBE
            {
                pxTexture->Type = PXTextureTypeCube;
                // arraySize *= 6;
                // isCubeMap = true;
            }
            else
            {
                pxTexture->Type = PXTextureType2D;
            }

            break;
        }
        case 4: // D3D10_RESOURCE_DIMENSION_TEXTURE3D:
        {
            if(!(pxDirectDrawTexture.Flags & DDS_HEADER_FLAGS_VOLUME))
            {
                return PXResultInvalid;
            }

            if(pxDirectDrawTexture.arraySize > 1)
            {
                return PXResultInvalid;
            }
            break;
        }
        default:
            return PXResultInvalid;
        }
    }
    else
    {
        if(pxDirectDrawTexture.Flags & DDS_HEADER_FLAGS_VOLUME)
        {
            pxTexture->Type = PXTextureType3D;
        }
        else
        {
            if(pxDirectDrawTexture.caps2 & DDS_CUBEMAP)
            {
                // We require all six faces to be defined.
                if((pxDirectDrawTexture.caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
                {
                    return PXResultInvalid;
                }
                pxTexture->Type = PXTextureTypeCube;

                //arraySize = 6;
                //isCubeMap = true;
            }
            else
            {
                pxTexture->Type = PXTextureType2D;
            }

            // depth = 1;
            // resDim = D3D11_RESOURCE_DIMENSION_TEXTURE2D;

            // Note there's no way for a legacy Direct3D 9 DDS to express a '1D' texture.
        }

        //assert(BitsPerPixel(format) != 0);
    }


#if 0

    // Bound sizes (For security purposes, we don't trust DDS file
    // metadata larger than the D3D 11.x hardware requirements.)
    if(mipCount > D3D11_REQ_MIP_LEVELS)
    {
        return throw ref new Platform::FailureException();
    }

    switch(resDim)
    {
    case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
        if((arraySize > D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) ||               (width > D3D11_REQ_TEXTURE1D_U_DIMENSION))
        {
            return throw ref new Platform::FailureException();
        }
        break;

    case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
        if(isCubeMap)
        {
            // This is the right bound because we set arraySize to (NumCubes*6) above.
            if((arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||                   (width > D3D11_REQ_TEXTURECUBE_DIMENSION) ||
                    (height > D3D11_REQ_TEXTURECUBE_DIMENSION))
            {
                return throw ref new Platform::FailureException();
            }
        }
        else if((arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||                    (width > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) ||                    (height > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION))
        {
            return throw ref new Platform::FailureException();
        }
        break;

    case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
        if((arraySize > 1) ||               (width > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||               (height > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||               (depth > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
        {
            return throw ref new Platform::FailureException();
        }
        break;
    }


    // Create the texture.
    std::unique_ptr<D3D11_SUBRESOURCE_DATA> initData(new D3D11_SUBRESOURCE_DATA[mipCount * arraySize]);

    size_t skipMip = 0;
    size_t twidth = 0;
    size_t theight = 0;
    size_t tdepth = 0;
    FillInitData(width, height, depth, mipCount, arraySize, format, maxsize, bitSize, bitData, twidth, theight, tdepth, skipMip, initData.get());

    hr = CreateD3DResources(d3dDevice, resDim, twidth, theight, tdepth, mipCount - skipMip, arraySize, format, isCubeMap, initData.get(), texture, textureView);

    if(FAILED(hr) && !maxsize && (mipCount > 1))
    {
        // Retry with a maxsize determined by feature level.
        switch(d3dDevice->GetFeatureLevel())
        {
        case D3D_FEATURE_LEVEL_9_1:
        case D3D_FEATURE_LEVEL_9_2:
            if(isCubeMap)
            {
                maxsize = D3D_FL9_1_REQ_TEXTURECUBE_DIMENSION;
            }
            else
            {
                maxsize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
                          ? D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION
                          : D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION;
            }
            break;

        case D3D_FEATURE_LEVEL_9_3:
            maxsize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
                      ? D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION
                      : D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION;
            break;

        default: // D3D_FEATURE_LEVEL_10_0 & D3D_FEATURE_LEVEL_10_1
            maxsize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
                      ? D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION
                      : D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION;
            break;
        }

        hr = CreateD3DResources(d3dDevice, resDim, twidth, theight, tdepth, mipCount - skipMip, arraySize, format, isCubeMap, initData.get(), texture, textureView);
    }
#endif


    return PXResultOK;
}

PXResult PXAPI PXDDSSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
