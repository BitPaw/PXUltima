#include "PXBitmap.h"

#include <OS/Memory/PXMemory.h>
#include <PXMath/PXPXMath.h>
#include <Media/PXImage.h>

#define PXBitmapHeaderIDWindows                  PXInt16Make('B', 'M')
#define PXBitmapHeaderIDOS2StructBitmapArray     PXInt16Make('B', 'A')
#define PXBitmapHeaderIDOS2StructColorIcon       PXInt16Make('C', 'I')
#define PXBitmapHeaderIDOS2ConstColorPointer     PXInt16Make('C', 'P')
#define PXBitmapHeaderIDOS2StructIcon            PXInt16Make('I', 'C')
#define PXBitmapHeaderIDOS2Pointer               PXInt16Make('P', 'T')

PXBitmapType ConvertToPXBitmapType(const unsigned short bmpTypeID)
{
    switch(bmpTypeID)
    {
        case PXBitmapHeaderIDOS2StructBitmapArray:
            return PXBitmapOS2StructBitmapArray;

        case PXBitmapHeaderIDWindows:
            return PXBitmapWindows;

        case PXBitmapHeaderIDOS2StructColorIcon:
            return PXBitmapOS2StructColorIcon;

        case PXBitmapHeaderIDOS2ConstColorPointer:
            return PXBitmapOS2ConstColorPointer;

        case PXBitmapHeaderIDOS2StructIcon:
            return PXBitmapOS2StructIcon;

        case PXBitmapHeaderIDOS2Pointer:
            return PXBitmapOS2Pointer;

        default:
            return PXBitmapInvalid;
    }
}

unsigned short ConvertFromPXBitmapType(const PXBitmapType headerType)
{
    switch(headerType)
    {
        default:
        case PXBitmapInvalid:
            return 0;

        case PXBitmapWindows:
            return PXBitmapHeaderIDWindows;

        case PXBitmapOS2StructBitmapArray:
            return PXBitmapHeaderIDOS2StructBitmapArray;

        case PXBitmapOS2StructColorIcon:
            return PXBitmapHeaderIDOS2StructColorIcon;

        case PXBitmapOS2ConstColorPointer:
            return PXBitmapHeaderIDOS2ConstColorPointer;

        case PXBitmapOS2StructIcon:
            return PXBitmapHeaderIDOS2StructIcon;

        case PXBitmapOS2Pointer:
            return PXBitmapHeaderIDOS2Pointer;
    }
}

PXBitmapInfoHeaderType ConvertToPXBitmapInfoHeaderType(const unsigned int infoHeaderType)
{
    switch(infoHeaderType)
    {
        case 12u:
            //const unsigned char bitMapCoreHeaderSize = 12;
            return PXBitmapHeaderOS21XBitMapHeader;

        case 16u:
            return PXBitmapHeaderOS22XBitMapHeader;

        case 40u:
            return PXBitmapHeaderBitMapInfoHeader;

        case 52u:
            return PXBitmapHeaderBitMapV2InfoHeader;

        case 56u:
            return PXBitmapHeaderBitMapV3InfoHeader;

        case 108u:
            return PXBitmapHeaderBitMapV4Header;

        case 124u:
            return PXBitmapHeaderBitMapV5Header;

        default:
            return PXBitmapHeaderUnkownOrInvalid;
    }
}

unsigned int ConvertFromPXBitmapInfoHeaderType(const PXBitmapInfoHeaderType infoHeaderType)
{
    switch(infoHeaderType)
    {
        default:
        case PXBitmapHeaderUnkownOrInvalid:
            return -1;

        case PXBitmapHeaderBitMapCoreHeader:
        case PXBitmapHeaderOS21XBitMapHeader:
            return 12u;

        case PXBitmapHeaderOS22XBitMapHeader:
            return 16u;

        case PXBitmapHeaderBitMapInfoHeader:
            return 40u;

        case PXBitmapHeaderBitMapV2InfoHeader:
            return 52u;

        case PXBitmapHeaderBitMapV3InfoHeader:
            return 56u;

        case PXBitmapHeaderBitMapV4Header:
            return 108u;

        case PXBitmapHeaderBitMapV5Header:
            return 124u;
    }
}

PXActionResult PXBitmapParseToImage(PXImage* const image, PXDataStream* const dataStream)
{
    PXBitmap bmp;

    PXBitmapConstruct(&bmp);

    PXInt32U sizeOfFile = 0;
    PXInt32U reservedBlock = 0;
    PXInt32U dataOffset = 0;

    //---[ Parsing Header ]----------------------------------------------------
    {
        PXInt16UCluster byteCluster;
        PXInt32U valueList[3];

        PXDataStreamReadB(dataStream, byteCluster.Data, 2u);
        PXDataStreamReadI32UVE(dataStream, valueList, 3u, EndianLittle);

        sizeOfFile = valueList[0];
        reservedBlock = valueList[1];
        dataOffset = valueList[2];

        const PXBitmapType type = ConvertToPXBitmapType(byteCluster.Value);

        {
            const PXBool isValidType = type != PXBitmapInvalid;

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

        bmp.InfoHeaderType = ConvertToPXBitmapInfoHeaderType(bmp.InfoHeader.HeaderSize);

        switch(bmp.InfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
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
            case PXBitmapHeaderOS21XBitMapHeader:
            case PXBitmapHeaderOS22XBitMapHeader:
            {
                {
                    PXInt16U valueList[4u];

                    PXDataStreamReadI16UVE(dataStream, valueList, 4u, EndianLittle);

                    bmp.InfoHeader.Width = valueList[0];
                    bmp.InfoHeader.Height = valueList[1];
                    bmp.InfoHeader.NumberOfColorPlanes = valueList[2];
                    bmp.InfoHeader.NumberOfBitsPerPixel = valueList[3];
                }

                if(bmp.InfoHeaderType == PXBitmapHeaderOS22XBitMapHeader)
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
        const PXBool allocationSuccess = PXImageResize(image, PXColorFormatRGBI8, bmp.InfoHeader.Width, bmp.InfoHeader.Height);

        if(!allocationSuccess)
        {
            return PXActionFailedAllocation;
        }
    }

    //---[ Pixel Data ]--------------------------------------------------------
    PXBitmapImageDataLayout imageDataLayout;

    PXBitmapImageDataLayoutCalculate(&imageDataLayout, bmp.InfoHeader.Width, bmp.InfoHeader.Height, bmp.InfoHeader.NumberOfBitsPerPixel);

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

PXActionResult PXBitmapSerialize(const PXBitmap* const bmp, PXDataStream* const dataStream)
{
    return PXActionSuccessful;
}

PXActionResult PXBitmapSerializeFromImage(const PXImage* const image, PXDataStream* const dataStream)
{
    PXBitmap bitMap;

    PXBitmapConstruct(&bitMap);

    //---<Header>-----
    {
        PXInt16UCluster byteCluster;
        unsigned int sizeOfFile = dataStream->DataSize;
        unsigned int reservedBlock = 0;
        unsigned int dataOffset = 54u;

        byteCluster.Value = ConvertFromPXBitmapType(PXBitmapWindows);

        PXDataStreamWriteB(dataStream, byteCluster.Data, 2u);
        PXDataStreamWriteI32UE(dataStream, sizeOfFile, EndianLittle);
        PXDataStreamWriteI32UE(dataStream, reservedBlock, EndianLittle);
        PXDataStreamWriteI32UE(dataStream, dataOffset, EndianLittle);
    }
    //----------------

    //---<DIP>
    {
        const PXBitmapInfoHeaderType bmpInfoHeaderType = PXBitmapHeaderBitMapInfoHeader;

        //---<Shared>----------------------------------------------------------
        bitMap.InfoHeader.HeaderSize = ConvertFromPXBitmapInfoHeaderType(bmpInfoHeaderType);
        bitMap.InfoHeader.NumberOfBitsPerPixel = PXColorFormatBitsPerPixel(image->Format);
        bitMap.InfoHeader.NumberOfColorPlanes = 1;
        bitMap.InfoHeader.Width = image->Width;
        bitMap.InfoHeader.Height = image->Height;
        //---------------------------------------------------------------------

        PXDataStreamWriteI32UE(dataStream, bitMap.InfoHeader.HeaderSize, EndianLittle);

        switch (bmpInfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
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
        PXBitmapImageDataLayout imageDataLayout;

        PXBitmapImageDataLayoutCalculate(&imageDataLayout, bitMap.InfoHeader.Width, bitMap.InfoHeader.Height, bitMap.InfoHeader.NumberOfBitsPerPixel);

        for (PXSize row = imageDataLayout.RowAmount-1; row != (PXSize)-1 ; --row)
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

void PXBitmapConstruct(PXBitmap* const bmp)
{
    MemoryClear(bmp, sizeof(PXBitmap));
}

void PXBitmapDestruct(PXBitmap* const bmp)
{
    MemoryRelease(bmp->PixelData, bmp->PixelDataSize);

    bmp->PixelData = 0;
    bmp->PixelDataSize = 0;
}

void PXBitmapImageDataLayoutCalculate(PXBitmapImageDataLayout* const bmpImageDataLayout, const PXSize width, const PXSize height, const PXSize bbp)
{
    bmpImageDataLayout->RowImageDataSize = width * (bbp / 8u);
    bmpImageDataLayout->ImageSize = bmpImageDataLayout->RowImageDataSize * height;
    bmpImageDataLayout->RowFullSize = PXMathFloorD((width * bbp + 31u) / 32.0f) * 4u;
    const int paddingSUM = (int)bmpImageDataLayout->RowFullSize - (int)bmpImageDataLayout->RowImageDataSize;
    bmpImageDataLayout->RowPaddingSize = PXMathAbsoluteI(paddingSUM);
    bmpImageDataLayout->RowAmount = PXMathCeilingF(bmpImageDataLayout->ImageSize / (float)bmpImageDataLayout->RowFullSize);
}

PXSize PXBitmapFilePredictSize(const PXSize width, const PXSize height, const PXSize bitsPerPixel)
{
    const PXSize sizePXBitmapHeader = 14u;
    const PXSize sizePXBitmapDIP = 40u;
    const PXSize imageDataSize = (PXMathFloorD((width * bitsPerPixel + 31u) / 32.0f) * 4u) * height;
    const PXSize fullSize = sizePXBitmapHeader + sizePXBitmapDIP + imageDataSize+512u;

    return fullSize;
}