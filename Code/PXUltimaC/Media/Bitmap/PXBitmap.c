#include "PXBitmap.h"

#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

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

PXActionResult PXAPI PXBitmapLoadFromFile(PXImage* const image, PXFile* const pxFile)
{
    PXBitmap bmp;

    PXBitmapConstruct(&bmp);

    //---[ Parsing Header ]----------------------------------------------------
    {
        PXInt16UCluster byteCluster;

        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {byteCluster.Data, PXDataTypeDatax2},
            {&bmp.SizeOfFile, PXDataTypeInt32ULE},
            {&bmp.ReservedBlock, PXDataTypeInt32ULE},
            {&bmp.DataOffset, PXDataTypeInt32ULE},
            {&bmp.InfoHeader.HeaderSize, PXDataTypeInt32ULE}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        bmp.Type = PXBitmapTypeFromID(byteCluster.Value);
        bmp.InfoHeaderType = PXBitmapInfoHeaderTypeFromID(bmp.InfoHeader.HeaderSize);

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
        switch(bmp.InfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
            {
                const PXFileDataElementType pxDataStreamElementList[] =
                {
                    {&bmp.InfoHeader.Width,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.Height,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.NumberOfColorPlanes, PXDataTypeInt16ULE},
                    {&bmp.InfoHeader.NumberOfBitsPerPixel, PXDataTypeInt16ULE},
                    {&bmp.InfoHeader.ExtendedInfo.BitMapInfo.CompressionMethod, PXDataTypeInt32ULE},
                    {&bmp.InfoHeader.ExtendedInfo.BitMapInfo.ImageSize, PXDataTypeInt32ULE},
                    {&bmp.InfoHeader.ExtendedInfo.BitMapInfo.HorizontalResolution,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.ExtendedInfo.BitMapInfo.VerticalResolution,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfColorsInTheColorPalette, PXDataTypeInt32ULE},
                    {&bmp.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfImportantColorsUsed,PXDataTypeInt32ULE},
                };

                PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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
                        {&height,PXDataTypeInt16ULE},
                        {&width,PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.NumberOfColorPlanes,PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.NumberOfBitsPerPixel,PXDataTypeInt16ULE}
                    };

                    PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                }

                if(bmp.InfoHeaderType == PXBitmapHeaderOS22XBitMapHeader)
                {
                    unsigned short paddingBytes = 0; // Padding.Ignored and should be zero

                    const PXFileDataElementType pxDataStreamElementList[] =
                    {
                        {&bmp.InfoHeader.ExtendedInfo.OS22XBitMap.HorizontalandVerticalResolutions, PXDataTypeInt16ULE},
                        {&paddingBytes, PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.ExtendedInfo.OS22XBitMap.DirectionOfBits, PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.ExtendedInfo.OS22XBitMap.halftoningAlgorithm, PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.ExtendedInfo.OS22XBitMap.HalftoningParameterA, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.ExtendedInfo.OS22XBitMap.HalftoningParameterB, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.ExtendedInfo.OS22XBitMap.ColorEncoding, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.ExtendedInfo.OS22XBitMap.ApplicationDefinedByte, PXDataTypeInt32ULE}
                    };

                    PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
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

PXActionResult PXAPI PXBitmapSaveToFile(const PXImage* const image, PXFile* const pxFile)
{
    PXBitmap bitMap;

    PXBitmapConstruct(&bitMap);

    //-----------------------------------------------------
    // Header - Write
    //-----------------------------------------------------
    {
        PXInt16UCluster byteCluster;
        PXInt32U sizeOfFile = pxFile->DataSize;
        PXInt32U reservedBlock = 0;
        PXInt32U dataOffset = 54u;

        byteCluster.Value = PXBitmapTypeToID(PXBitmapWindows);

        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {byteCluster.Data, PXDataTypeDatax2},
            {sizeOfFile, PXDataTypeInt32ULE},
            {reservedBlock, PXDataTypeInt32ULE},
            {dataOffset, PXDataTypeInt32ULE}
        };

        PXFileWriteMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
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
                    {&bitMap.InfoHeader.Width, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.Height, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.NumberOfColorPlanes, PXDataTypeInt16ULE},
                    {&bitMap.InfoHeader.NumberOfBitsPerPixel, PXDataTypeInt16ULE},
                    {&bitMap.InfoHeader.ExtendedInfo.BitMapInfo.CompressionMethod, PXDataTypeInt32ULE},
                    {&bitMap.InfoHeader.ExtendedInfo.BitMapInfo.ImageSize, PXDataTypeInt32ULE},
                    {&bitMap.InfoHeader.ExtendedInfo.BitMapInfo.HorizontalResolution, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.ExtendedInfo.BitMapInfo.VerticalResolution, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfColorsInTheColorPalette, PXDataTypeInt32ULE},
                    {&bitMap.InfoHeader.ExtendedInfo.BitMapInfo.NumberOfImportantColorsUsed, PXDataTypeInt32ULE},
                };

                PXFileWriteMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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

void PXAPI PXBitmapConstruct(PXBitmap* const bmp)
{
    PXClear(PXBitmap, bmp);
}

void PXAPI PXBitmapDestruct(PXBitmap* const bmp)
{
    PXDeleteList(PXByte, bmp->PixelData, bmp->PixelDataSize);

    bmp->PixelData = PXNull;
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

PXSize PXAPI PXBitmapFilePredictSize(const PXSize width, const PXSize height, const PXSize bitsPerPixel)
{
    const PXSize sizePXBitmapHeader = 14u;
    const PXSize sizePXBitmapDIP = 40u;
    const PXSize imageDataSize = (PXMathFloorD((width * bitsPerPixel + 31u) / 32.0f) * 4u) * height;
    const PXSize fullSize = sizePXBitmapHeader + sizePXBitmapDIP + imageDataSize+512u;

    return fullSize;
}