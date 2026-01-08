#include "PXRTexture.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Math/PXMath.h>

const char PXRTextureSignature[4] = "TEX\0";


const PXI32U PXRTextureHeaderList[] =
{
    PXTypeDatax4, // PXTypeSignatureCheck
    PXTypeInt16U,
    PXTypeInt16U,

    PXTypeBool,
    PXTypeInt08U,
    PXTypeInt08U,
    PXTypeInt08U
};
const PXI8U PXRTextureHeaderListSize = sizeof(PXRTextureHeaderList) / sizeof(PXI32U);




#define PXRTextureExtendedTextureFormatInvalid    0
#define PXRTextureExtendedTextureFormatBGRA8      1
#define PXRTextureExtendedTextureFormatETC1       2
#define PXRTextureExtendedTextureFormatETC2_EAC   3
#define PXRTextureExtendedTextureFormatBC1        4
#define PXRTextureExtendedTextureFormatBC3        5

#define PXRTextureResourceTypeTexture 0
#define PXRTextureResourceTypeCubemap 1
#define PXRTextureResourceTypeSurface 2
#define PXRTextureResourceTypeTextureVolume 3

#define PXRTextureFlagsHasMipMaps (1 << 0)




#define PXRTextureCompressionFormatR                        (1<<0)
#define PXRTextureCompressionFormatRg                       (1<<1)
#define PXRTextureCompressionFormatRgb                      (1<<2)
#define PXRTextureCompressionFormatRgba                     (1<<3)
#define PXRTextureCompressionFormatBgra                     (1<<4)
#define PXRTextureCompressionFormatBc1                      (1<<5)
#define PXRTextureCompressionFormatBc1WithAlpha             (1<<6)
#define PXRTextureCompressionFormatBc2                      (1<<7)
#define PXRTextureCompressionFormatBc3                      (1<<8)
#define PXRTextureCompressionFormatBc4                      (1<<9)
#define PXRTextureCompressionFormatBc5                      (1<<10)
#define PXRTextureCompressionFormatBc6U                     (1<<11)
#define PXRTextureCompressionFormatBc6S                     (1<<12)
#define PXRTextureCompressionFormatBc7                      (1<<13)
#define PXRTextureCompressionFormatAtc                      (1<<14)
#define PXRTextureCompressionFormatAtcExplicitAlpha         (1<<15)
#define PXRTextureCompressionFormatAtcInterpolatedAlpha     (1<<16)




typedef struct Bc1Block_
{
    int __Dummy__;
}
Bc1Block;

typedef struct Bc2Block_
{
    int __Dummy__;
}
Bc2Block;

typedef struct Bc3Block_
{
    int __Dummy__;
}
Bc3Block;

typedef struct Bc4Block_
{
    int __Dummy__;
}
Bc4Block;

typedef struct Bc5Block_
{
    int __Dummy__;
}
Bc5Block;

typedef struct Bc6Block_
{
    int __Dummy__;
}
Bc6Block;

typedef struct Bc7Block_
{
    int __Dummy__;
}
Bc7Block;

typedef struct AtcBlock_
{
    int __Dummy__;
}
AtcBlock;


typedef struct AtcExplicitAlphaBlock_
{
    int __Dummy__;
}
AtcExplicitAlphaBlock;


typedef struct AtcInterpolatedAlphaBlock_
{
    int __Dummy__;
}
AtcInterpolatedAlphaBlock;


const PXI8U PXBlockSizeList[17] = 
{
    1,
    2,
    3,
    4,
    4,
    sizeof(Bc1Block),
    sizeof(Bc1Block),
    sizeof(Bc2Block),
    sizeof(Bc3Block),
    sizeof(Bc4Block),
    sizeof(Bc5Block),
    sizeof(Bc6Block),
    sizeof(Bc6Block),
    sizeof(Bc7Block),
    sizeof(AtcBlock),
    sizeof(AtcExplicitAlphaBlock),
    sizeof(AtcInterpolatedAlphaBlock)
};

PXResult PXAPI PXRTextureLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

    PXRTexture pxRTexture;
    PXClear(PXRTexture, &pxRTexture);

    const PXSize amount = PXFileBinding
    (
        pxFile,
        &pxRTexture.Header,
        PXRTextureHeaderList,
        PXRTextureHeaderListSize,
        PXFileBindingRead
    );

    // Translate values
    {
        // Translate compression format
        switch(pxRTexture.Header.ExtendedFormat)
        {
            case 1: 
                pxRTexture.Header.ResourceType = PXRTextureExtendedTextureFormatETC1;
                break;

            case 2: 
            case 3:
                pxRTexture.Header.ResourceType = PXRTextureExtendedTextureFormatETC2_EAC;
                break;

            case 10:
            case 11:
                pxRTexture.Header.ResourceType = PXRTextureExtendedTextureFormatBC1;
                break;

            case 12:
                pxRTexture.Header.ResourceType = PXRTextureExtendedTextureFormatBC3;
                break;

            case 20:
                pxRTexture.Header.ResourceType = PXRTextureExtendedTextureFormatBGRA8;
                break;

            default:
                pxRTexture.Header.ResourceType = PXRTextureExtendedTextureFormatInvalid;
                break;
        }       


        // Compression format
        switch(pxRTexture.Header.ResourceType)
        {
            case PXRTextureExtendedTextureFormatBGRA8:
            {
                pxRTexture.CompressionFormat = PXRTextureCompressionFormatBgra;
                break;
            }
            case PXRTextureExtendedTextureFormatBC1:
            {
                pxRTexture.CompressionFormat = PXRTextureCompressionFormatBc1WithAlpha;
                break;
            }
            case PXRTextureExtendedTextureFormatBC3:
            {
                pxRTexture.CompressionFormat = PXRTextureCompressionFormatBc3;
                break;
            }
            default:
            {
                pxRTexture.CompressionFormat = 0;
                break;
            }
        }

        pxRTexture.BlockSize = PXBlockSizeList[pxRTexture.CompressionFormat];
    }


    if(PXRTextureFlagsHasMipMaps & pxRTexture.Header.Flags)
    {
        const PXI16U maxWidth = PXMathMaximum(pxRTexture.Header.Width, pxRTexture.Header.Height);
        const PXI16U logValue = PXMathLogarithm2(maxWidth);

        pxRTexture.MipMapCount = PXMathFloor(logValue) + 1;
    }
    else
    {
        pxRTexture.MipMapCount = 1;
    }


    // Seek to end because mipmaps are stored in reverse order (from smallest)
    // We will be reading them in reverse
    PXFileCursorToEnd(pxFile);

#if 0

    //Memory2D<ColorRgba32>[] mipMaps = new Memory2D<ColorRgba32>[mipMapCount];
    PXTexture PXREF pxTextureList = PXMemoryHeapCallocT(PXTexture, pxRTexture.MipMapCount);

    for(PXI8U i = 0; i < pxRTexture.MipMapCount; ++i)
    {
        PXTexture PXREF pxTexture = &pxTextureList[i];

        // Calculate dimensions of current mipmap
        PXI16U currentWidth = PXMathMaximum(pxRTexture.Header.Width >> i, 1);
        PXI16U currentHeight = PXMathMaximum(pxRTexture.Header.Height >> i, 1);

        PXI8U blockLength = PXRTextureExtendedTextureFormatBGRA8 & pxRTexture.Header.ExtendedFormat ? 1 : 4;
        PXI16U widthInBlocks = (currentWidth + blockLength - 1) / blockLength;
        PXI16U heightInBlocks = (currentHeight + blockLength - 1) / blockLength;

        PXI32U mipMapSize = widthInBlocks * heightInBlocks * pxRTexture.BlockSize;

      
        // Texture allocate!
        ///pxTexture.


        // Seek to start of mipmap and read it into buffer
        //br.BaseStream.Seek(-mipMapSize, SeekOrigin.Current);
        PXFileCursorRewind(pxFile, mipMapSize);

        // "read"
        void* imageData = PXFileCursorPosition(pxFile);

        // Data formated load???
        
        PXTextureLoadData(pxFile);

        // Decode buffer and seek back
        ColorRgba32[] mipMapData = decoder.DecodeRaw(
            mipMapBufferOwner.Memory,
            currentWidth,
            currentHeight,
            compressionFormat
        );

        // Additional rewind to undo current read
        PXFileCursorRewind(pxFile, mipMapSize);
    }
#endif



    return PXActionSuccessful;
}

PXResult PXAPI PXRTextureSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}