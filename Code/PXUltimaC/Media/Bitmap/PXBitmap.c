#include "PXBitmap.h"

#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>
#include <Media/PXImage.h>

#define PXQuickSwap(a, b) \
a = a + b; \
b = a - b; \
a = a - b;


#define PXBitmapHeaderIDWindows                  PXInt16Make('B', 'M')
#define PXBitmapHeaderIDOS2StructBitmapArray     PXInt16Make('B', 'A')
#define PXBitmapHeaderIDOS2StructColorIcon       PXInt16Make('C', 'I')
#define PXBitmapHeaderIDOS2ConstColorPointer     PXInt16Make('C', 'P')
#define PXBitmapHeaderIDOS2StructIcon            PXInt16Make('I', 'C')
#define PXBitmapHeaderIDOS2Pointer               PXInt16Make('P', 'T')

PXBitmapType PXBitmapTypeFromID(const PXInt16U bmpTypeID)
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

PXInt16U PXBitmapTypeToID(const PXBitmapType headerType)
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

PXBitmapInfoHeaderType PXBitmapInfoHeaderTypeFromID(const PXInt8U infoHeaderType)
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

PXInt8U PXBitmapInfoHeaderTypeToID(const PXBitmapInfoHeaderType infoHeaderType)
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

PXActionResult PXBitmapLoadFromFile(PXImage* const image, PXFile* const pxFile)
{
    PXBitmap bmp;

    PXBitmapConstruct(&bmp);

    PXInt32U sizeOfFile = 0;
    PXInt32U reservedBlock = 0;
    PXInt32U dataOffset = 0;

    //---[ Parsing Header ]----------------------------------------------------
    {
        PXInt16UCluster byteCluster;
        PXInt32U* valueList[3] = { &sizeOfFile, &reservedBlock, &dataOffset };

        PXFileReadB(pxFile, byteCluster.Data, 2u);
        PXFileReadI32UVE(pxFile, valueList, 3u, PXEndianLittle);

        bmp.Type = PXBitmapTypeFromID(byteCluster.Value);

        {
            const PXBool isValidType = bmp.Type != PXBitmapInvalid;

            if(!isValidType)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }
    }
    //-------------------------------------------------------------------------

    //---[ DIP ]---------------------------------------------------------------
    {
        PXFileReadI32UE(pxFile, &bmp.InfoHeader.HeaderSize, PXEndianLittle);

        bmp.InfoHeaderType = PXBitmapInfoHeaderTypeFromID(bmp.InfoHeader.HeaderSize);

        switch(bmp.InfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
            {
                const PXFileDataElementType pxDataStreamElementList[] =
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
                const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

                PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

                break;
            }
            case PXBitmapHeaderOS21XBitMapHeader:
            case PXBitmapHeaderOS22XBitMapHeader:
            {
                {
                    PXInt16U height;
                    PXInt16U width;

                    const PXFileDataElementType pxDataStreamElementList[] =
                    {
                        {PXDataTypeLEInt16U, &height},
                        {PXDataTypeLEInt16U, &width},
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.NumberOfColorPlanes},
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.NumberOfBitsPerPixel}
                    };
                    const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

                    PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);
                }

                if(bmp.InfoHeaderType == PXBitmapHeaderOS22XBitMapHeader)
                {
                    unsigned short paddingBytes = 0; // Padding.Ignored and should be zero

                    const PXFileDataElementType pxDataStreamElementList[] =
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
                    const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

                    PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);
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
            return PXActionFailedMemoryAllocation;
        }
    }

    //---[ Pixel Data ]--------------------------------------------------------
    PXBitmapImageDataLayout imageDataLayout;

    PXBitmapImageDataLayoutCalculate(&imageDataLayout, bmp.InfoHeader.Width, bmp.InfoHeader.Height, bmp.InfoHeader.NumberOfBitsPerPixel);

    while(imageDataLayout.RowAmount--) // loop through each image row
    {
        PXByte* const data = (PXByte* const)image->PixelData + (imageDataLayout.RowFullSize * imageDataLayout.RowAmount); // Get the starting point of each row

        PXFileReadB(pxFile, data, imageDataLayout.RowImageDataSize); // Read/Write image data
        PXFileCursorAdvance(pxFile, imageDataLayout.RowPaddingSize); // Skip padding

        for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += 3u)
        {
            PXQuickSwap(data[i], data[i+2]) // BGR -> RGB, just swap Blue and Red, Green stays.
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXBitmapSerialize(const PXBitmap* const bmp, PXFile* const pxFile)
{
    return PXActionSuccessful;
}

PXActionResult PXBitmapSaveToFile(const PXImage* const image, PXFile* const pxFile)
{
    PXBitmap bitMap;

    PXBitmapConstruct(&bitMap);

    //-----------------------------------------------------
    // Header - Write
    //-----------------------------------------------------
    {
        {
            PXInt16UCluster byteCluster;
            
            byteCluster.Value = PXBitmapTypeToID(PXBitmapWindows);

            PXFileWriteB(pxFile, byteCluster.Data, 2u);
        }
     
        {
            const PXInt32U data[3] =
            {
                pxFile->DataSize, // sizeOfFile
                0, // reservedBlock
                54u // dataOffset
            };

            PXFileWriteI32UVE(pxFile, data, 3u, PXEndianLittle);
        }      
    }
    //-----------------------------------------------------


    //-----------------------------------------------------
    // DIP - Write
    //-----------------------------------------------------
    {
        const PXBitmapInfoHeaderType bmpInfoHeaderType = PXBitmapHeaderBitMapInfoHeader;

        //---<Shared>----------------------------------------------------------
        bitMap.InfoHeader.HeaderSize = PXBitmapInfoHeaderTypeFromID(bmpInfoHeaderType);
        bitMap.InfoHeader.NumberOfBitsPerPixel = PXColorFormatBitsPerPixel(image->Format);
        bitMap.InfoHeader.NumberOfColorPlanes = 1;
        bitMap.InfoHeader.Width = image->Width;
        bitMap.InfoHeader.Height = image->Height;
        //---------------------------------------------------------------------

        PXFileWriteI32UE(pxFile, bitMap.InfoHeader.HeaderSize, PXEndianLittle);

        switch (bmpInfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
            {
                bitMap.InfoHeader.ExtendedInfo.BitMapInfo.HorizontalResolution = 1u;
                bitMap.InfoHeader.ExtendedInfo.BitMapInfo.VerticalResolution = 1u;

                const PXFileDataElementType pxDataStreamElementList[] =
                {
                    {PXDataTypeLEInt32S, &bitMap.InfoHeader.Width},
                    {PXDataTypeLEInt32S, &bitMap.InfoHeader.Height},
                    {PXDataTypeLEInt16U, &bitMap.InfoHeader.NumberOfColorPlanes},
                    {PXDataTypeLEInt16U, &bitMap.InfoHeader.NumberOfBitsPerPixel},
                    {PXDataTypeLEInt32U, &bitMap.InfoHeader.ExtendedInfo.BitMapInfo.CompressionMethod},
                    {PXDataTypeLEInt32U, &bitMap.InfoHeader.ExtendedInfo.BitMapInfo.ImageSize},
                    {PXDataTypeLEInt32S, &bitMap.InfoHeader.ExtendedInfo.BitMapInfo.HorizontalResolution},
                    {PXDataTypeLEInt32S, &bitMap.InfoHeader.ExtendedInfo.BitMapInfo.VerticalResolution},
                    {PXDataTypeLEInt32U, &bitMap.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfColorsInTheColorPalette},
                    {PXDataTypeLEInt32U, &bitMap.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfImportantColorsUsed},
                };
                const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

                PXFileWriteMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

                break;
            }
        }
    }
    //-----------------------------------------------------

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

                PXFileWriteB(pxFile, pixelBuffer, 3u);
            }

            PXFileWriteFill(pxFile, 0, imageDataLayout.RowPaddingSize);
        }
    }

    return PXActionSuccessful;
}

void PXBitmapConstruct(PXBitmap* const bmp)
{
    PXClear(PXBitmap, bmp);
}

void PXBitmapDestruct(PXBitmap* const bmp)
{
    PXMemoryRelease(bmp->PixelData, bmp->PixelDataSize);

    bmp->PixelData = 0;
    bmp->PixelDataSize = 0;
}

void PXBitmapImageDataLayoutCalculate(PXBitmapImageDataLayout* const bmpImageDataLayout, const PXSize width, const PXSize height, const PXSize bbp)
{
    bmpImageDataLayout->RowImageDataSize = width * (bbp / 8u);
    bmpImageDataLayout->ImageSize = bmpImageDataLayout->RowImageDataSize * height;
    bmpImageDataLayout->RowFullSize = PXMathFloorD((width * bbp + 31u) / 32.0f) * 4u;
    const int paddingSUM = (int)bmpImageDataLayout->RowFullSize - (int)bmpImageDataLayout->RowImageDataSize;
    bmpImageDataLayout->RowPaddingSize = PXMathAbsoluteI32(paddingSUM);
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