#include "BMP.h"

#include <Container/ClusterValue.h>
#include <Container/ClusterValue.h>
#include <File/PXDataStream.h>
#include <Memory/PXMemory.h>
#include <Math/PXMath.h>
#include <Format/Image.h>

#define BMPHeaderIDWindows                  MakeShort('B', 'M')
#define BMPHeaderIDOS2StructBitmapArray     MakeShort('B', 'A')
#define BMPHeaderIDOS2StructColorIcon       MakeShort('C', 'I')
#define BMPHeaderIDOS2ConstColorPointer     MakeShort('C', 'P')
#define BMPHeaderIDOS2StructIcon            MakeShort('I', 'C')
#define BMPHeaderIDOS2Pointer               MakeShort('P', 'T')


BMPType ConvertToBMPType(const unsigned short bmpTypeID)
{
    switch(bmpTypeID)
    {
        case BMPHeaderIDOS2StructBitmapArray:
            return BMPOS2StructBitmapArray;

        case BMPHeaderIDWindows:
            return BMPWindows;

        case BMPHeaderIDOS2StructColorIcon:
            return BMPOS2StructColorIcon;

        case BMPHeaderIDOS2ConstColorPointer:
            return BMPOS2ConstColorPointer;

        case BMPHeaderIDOS2StructIcon:
            return BMPOS2StructIcon;

        case BMPHeaderIDOS2Pointer:
            return BMPOS2Pointer;

        default:
            return BMPInvalid;
    }
}

unsigned short ConvertFromBMPType(const BMPType headerType)
{
    switch(headerType)
    {
        default:
        case BMPInvalid:
            return 0;

        case BMPWindows:
            return BMPHeaderIDWindows;

        case BMPOS2StructBitmapArray:
            return BMPHeaderIDOS2StructBitmapArray;

        case BMPOS2StructColorIcon:
            return BMPHeaderIDOS2StructColorIcon;

        case BMPOS2ConstColorPointer:
            return BMPHeaderIDOS2ConstColorPointer;

        case BMPOS2StructIcon:
            return BMPHeaderIDOS2StructIcon;

        case BMPOS2Pointer:
            return BMPHeaderIDOS2Pointer;
    }
}

BMPInfoHeaderType ConvertToBMPInfoHeaderType(const unsigned int infoHeaderType)
{
    switch(infoHeaderType)
    {
        case 12u:
            //const unsigned char bitMapCoreHeaderSize = 12;
            return BMPHeaderOS21XBitMapHeader;

        case 16u:
            return BMPHeaderOS22XBitMapHeader;

        case 40u:
            return BMPHeaderBitMapInfoHeader;

        case 52u:
            return BMPHeaderBitMapV2InfoHeader;

        case 56u:
            return BMPHeaderBitMapV3InfoHeader;

        case 108u:
            return BMPHeaderBitMapV4Header;

        case 124u:
            return BMPHeaderBitMapV5Header;

        default:
            return BMPHeaderUnkownOrInvalid;
    }
}

unsigned int ConvertFromBMPInfoHeaderType(const BMPInfoHeaderType infoHeaderType)
{
    switch(infoHeaderType)
    {
        default:
        case BMPHeaderUnkownOrInvalid:
            return -1;

        case BMPHeaderBitMapCoreHeader:
        case BMPHeaderOS21XBitMapHeader:
            return 12u;

        case BMPHeaderOS22XBitMapHeader:
            return 16u;

        case BMPHeaderBitMapInfoHeader:
            return 40u;

        case BMPHeaderBitMapV2InfoHeader:
            return 52u;

        case BMPHeaderBitMapV3InfoHeader:
            return 56u;

        case BMPHeaderBitMapV4Header:
            return 108u;

        case BMPHeaderBitMapV5Header:
            return 124u;
    }
}

PXActionResult BMPParseToImage(Image* const image, PXDataStream* const dataStream)
{
    BMP bmp;

    BMPConstruct(&bmp);

    PXInt32U sizeOfFile = 0;
    PXInt32U reservedBlock = 0;
    PXInt32U dataOffset = 0;

    //---[ Parsing Header ]----------------------------------------------------
    {
        ClusterShort byteCluster;
        PXInt32U valueList[3];

        PXDataStreamReadB(dataStream, byteCluster.Data, 2u);
        PXDataStreamReadI32UVE(dataStream, valueList, 3u, EndianLittle);

        sizeOfFile = valueList[0];
        reservedBlock = valueList[1];
        dataOffset = valueList[2];

        const BMPType type = ConvertToBMPType(byteCluster.Value);

        {
            const PXBool isValidType = type != BMPInvalid;

            if(!isValidType)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }

        bmp.Type = type;
    }
    //-------------------------------------------------------------------------

    //---[ DIP ]---------------------------------------------------------------
    {
        PXDataStreamReadI32UE(dataStream, &bmp.InfoHeader.HeaderSize, EndianLittle);

        bmp.InfoHeaderType = ConvertToBMPInfoHeaderType(bmp.InfoHeader.HeaderSize);

        switch(bmp.InfoHeaderType)
        {
            case BMPHeaderBitMapInfoHeader:
            {
                const PXDataStreamElementType pxDataStreamElementList[] =
                {
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.Width},
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.Height},
                    {PXDataTypeLEInt16U, &bmp.InfoHeader.NumberOfColorPlanes},
                    {PXDataTypeLEInt16U, &bmp.InfoHeader.NumberOfBitsPerPixel},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.BitMapInfo.CompressionMethod},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.BitMapInfo.ImageSize},
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.ExtendedInfo.BitMapInfo.HorizontalResolution},
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.ExtendedInfo.BitMapInfo.VerticalResolution},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfColorsInTheColorPalette},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfImportantColorsUsed},
                };
                const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXDataStreamElementType);

                PXDataStreamReadMultible(dataStream, pxDataStreamElementList, pxDataStreamElementListSize);

                break;
            }
            case BMPHeaderOS21XBitMapHeader:
            case BMPHeaderOS22XBitMapHeader:
            {
                {
                    PXInt16U valueList[4u];

                    PXDataStreamReadI16UVE(dataStream, valueList, 4u, EndianLittle);

                    bmp.InfoHeader.Width = valueList[0];
                    bmp.InfoHeader.Height = valueList[1];
                    bmp.InfoHeader.NumberOfColorPlanes = valueList[2];
                    bmp.InfoHeader.NumberOfBitsPerPixel = valueList[3];
                }

                if(bmp.InfoHeaderType == BMPHeaderOS22XBitMapHeader)
                {
                    unsigned short paddingBytes = 0; // Padding.Ignored and should be zero

                    const PXDataStreamElementType pxDataStreamElementList[] =
                    {
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.ExtendedInfo.OS22XBitMap.HorizontalandVerticalResolutions},
                        {PXDataTypeLEInt16U, &paddingBytes},
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.ExtendedInfo.OS22XBitMap.DirectionOfBits},
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.ExtendedInfo.OS22XBitMap.halftoningAlgorithm},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.OS22XBitMap.HalftoningParameterA},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.OS22XBitMap.HalftoningParameterB},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.OS22XBitMap.ColorEncoding},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.ExtendedInfo.OS22XBitMap.ApplicationDefinedByte}
                    };
                    const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXDataStreamElementType);

                    PXDataStreamReadMultible(dataStream, pxDataStreamElementList, pxDataStreamElementListSize);
                }

                break;
            }
            default:
            {
                // Unkown Header
                return PXActionRefusedFormatNotSupported;
            }
        }
    }
    //-----------------------------------------------------------


    // Generate imagedata
    {
        const PXBool allocationSuccess = ImageResize(image, ImageDataFormatRGB8, bmp.InfoHeader.Width, bmp.InfoHeader.Height);

        if(!allocationSuccess)
        {
            return PXActionFailedAllocation;
        }
    }

    //---[ Pixel Data ]--------------------------------------------------------
    BMPImageDataLayout imageDataLayout;

    BMPImageDataLayoutCalculate(&imageDataLayout, bmp.InfoHeader.Width, bmp.InfoHeader.Height, bmp.InfoHeader.NumberOfBitsPerPixel);

    while(imageDataLayout.RowAmount--) // loop through each image row
    {
        PXByte* const data = (PXByte* const)image->PixelData + (imageDataLayout.RowFullSize * imageDataLayout.RowAmount); // Get the starting point of each row

        PXDataStreamReadB(dataStream, data, imageDataLayout.RowImageDataSize); // Read/Write image data
        PXDataStreamCursorAdvance(dataStream, imageDataLayout.RowPaddingSize); // Skip padding

        for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += 3)
        {
            const PXByte swap = data[i]; // Save Blue(current)

            data[i] = data[i + 2]; // Override Blue(current) with Red(new)
            data[i+2] = swap; // Override Red(current) with Blue(new)
        }
    }

    return PXActionSuccessful;
}

PXActionResult BMPSerialize(const BMP* const bmp, PXDataStream* const dataStream)
{
    return PXActionSuccessful;
}

PXActionResult BMPSerializeFromImage(const Image* const image, PXDataStream* const dataStream)
{
    BMP bitMap;

    BMPConstruct(&bitMap);

    //---<Header>-----
    {
        ClusterShort byteCluster;
        unsigned int sizeOfFile = dataStream->DataSize;
        unsigned int reservedBlock = 0;
        unsigned int dataOffset = 54u;

        byteCluster.Value = ConvertFromBMPType(BMPWindows);

        PXDataStreamWriteB(dataStream, byteCluster.Data, 2u);
        PXDataStreamWriteI32UE(dataStream, sizeOfFile, EndianLittle);
        PXDataStreamWriteI32UE(dataStream, reservedBlock, EndianLittle);
        PXDataStreamWriteI32UE(dataStream, dataOffset, EndianLittle);
    }
    //----------------

    //---<DIP>
    {
        const BMPInfoHeaderType bmpInfoHeaderType = BMPHeaderBitMapInfoHeader;

        //---<Shared>----------------------------------------------------------
        bitMap.InfoHeader.HeaderSize = ConvertFromBMPInfoHeaderType(bmpInfoHeaderType);
        bitMap.InfoHeader.NumberOfBitsPerPixel = ImageBytePerPixel(image->Format) * 8u;
        bitMap.InfoHeader.NumberOfColorPlanes = 1;
        bitMap.InfoHeader.Width = image->Width;
        bitMap.InfoHeader.Height = image->Height;
        //---------------------------------------------------------------------

        PXDataStreamWriteI32UE(dataStream, bitMap.InfoHeader.HeaderSize, EndianLittle);

        switch (bmpInfoHeaderType)
        {
            case BMPHeaderBitMapInfoHeader:
            {
                bitMap.InfoHeader.ExtendedInfo.BitMapInfo.HorizontalResolution = 1u;
                bitMap.InfoHeader.ExtendedInfo.BitMapInfo.VerticalResolution = 1u;

                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.Width, EndianLittle);
                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.Height, EndianLittle);
                PXDataStreamWriteI16UE(dataStream, bitMap.InfoHeader.NumberOfColorPlanes, EndianLittle);
                PXDataStreamWriteI16UE(dataStream, bitMap.InfoHeader.NumberOfBitsPerPixel, EndianLittle);
                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.ExtendedInfo.BitMapInfo.CompressionMethod, EndianLittle);
                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.ExtendedInfo.BitMapInfo.ImageSize, EndianLittle);
                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.ExtendedInfo.BitMapInfo.HorizontalResolution, EndianLittle);
                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.ExtendedInfo.BitMapInfo.VerticalResolution, EndianLittle);
                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfColorsInTheColorPalette, EndianLittle);
                PXDataStreamWriteI32SE(dataStream, bitMap.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfImportantColorsUsed, EndianLittle);
                break;
            }
        }
    }
    //------------

    {
        BMPImageDataLayout imageDataLayout;

        BMPImageDataLayoutCalculate(&imageDataLayout, bitMap.InfoHeader.Width, bitMap.InfoHeader.Height, bitMap.InfoHeader.NumberOfBitsPerPixel);

        for (PXSize row = imageDataLayout.RowAmount-1; row != (PXSize)-1  ; --row)
        {
            const PXByte* const dataInsertPoint = (const PXByte* const)image->PixelData + (imageDataLayout.RowImageDataSize * row);

            for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += 3) // Will result in RGB Pixel Data
            {
                const PXByte pixelBuffer[3u] = 
                {                 
                    dataInsertPoint[i+2u], 
                    dataInsertPoint[i+1u],
                    dataInsertPoint[i]
                };

                PXDataStreamWriteB(dataStream, pixelBuffer, 3u);
            }

            PXDataStreamWriteFill(dataStream, 0, imageDataLayout.RowPaddingSize);
        }
    }

    return PXActionSuccessful;
}

void BMPConstruct(BMP* const bmp)
{
    MemoryClear(bmp, sizeof(BMP));
}

void BMPDestruct(BMP* const bmp)
{
    MemoryRelease(bmp->PixelData, bmp->PixelDataSize);

    bmp->PixelData = 0;
    bmp->PixelDataSize = 0;
}

void BMPImageDataLayoutCalculate(BMPImageDataLayout* const bmpImageDataLayout, const PXSize width, const PXSize height, const PXSize bbp)
{
    bmpImageDataLayout->RowImageDataSize = width * (bbp / 8u);
    bmpImageDataLayout->ImageSize = bmpImageDataLayout->RowImageDataSize * height;
    bmpImageDataLayout->RowFullSize = MathFloorD((width * bbp + 31u) / 32.0f) * 4u;
    const int paddingSUM = (int)bmpImageDataLayout->RowFullSize - (int)bmpImageDataLayout->RowImageDataSize;
    bmpImageDataLayout->RowPaddingSize = MathAbsoluteI(paddingSUM);
    bmpImageDataLayout->RowAmount = MathCeilingF(bmpImageDataLayout->ImageSize / (float)bmpImageDataLayout->RowFullSize);
}

PXSize BMPFilePredictSize(const PXSize width, const PXSize height, const PXSize bitsPerPixel)
{
    const PXSize sizeBMPHeader = 14u;
    const PXSize sizeBMPDIP = 40u;
    const PXSize imageDataSize = (MathFloorD((width * bitsPerPixel + 31u) / 32.0f) * 4u) * height;
    const PXSize fullSize = sizeBMPHeader + sizeBMPDIP + imageDataSize+512u;

    return fullSize;
}