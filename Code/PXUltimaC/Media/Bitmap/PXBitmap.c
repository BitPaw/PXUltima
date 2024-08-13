#include "PXBitmap.h"

#include <Math/PXMath.h>
#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

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

PXActionResult PXAPI PXBitmapLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceLoadInfo->ResourceTarget;

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

        PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        bmp.Type = PXBitmapTypeFromID(byteCluster.Value);
        bmp.InfoHeaderType = PXBitmapInfoHeaderTypeFromID(bmp.InfoHeader.HeaderSize);

        {
            const PXBool isValidType = PXBitmapInvalid != bmp.Type;

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
            case PXBitmapHeaderBitMapV5Header:
            {
                const PXFileDataElementType pxDataStreamElementList[] =
                {
                    {&bmp.InfoHeader.Width,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.Height,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.NumberOfColorPlanes, PXDataTypeInt16ULE},
                    {&bmp.InfoHeader.NumberOfBitsPerPixel, PXDataTypeInt16ULE},
                    {&bmp.InfoHeader.BitMapInfo.CompressionMethod, PXDataTypeInt32ULE},
                    {&bmp.InfoHeader.BitMapInfo.ImageSize, PXDataTypeInt32ULE},
                    {&bmp.InfoHeader.BitMapInfo.HorizontalResolution,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.BitMapInfo.VerticalResolution,PXDataTypeInt32SLE},
                    {&bmp.InfoHeader.BitMapInfo.NumberOfColorsInTheColorPalette, PXDataTypeInt32ULE},
                    {&bmp.InfoHeader.BitMapInfo.NumberOfImportantColorsUsed,PXDataTypeInt32ULE},
                };

                PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                if(PXBitmapHeaderBitMapV5Header == bmp.InfoHeaderType)
                {
                    const PXFileDataElementType pxDataStreamElementList[] =
                    {
                        {&bmp.InfoHeader.HeaderV5.ClrUsed, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.ClrImportant, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.RedMask, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.GreenMask, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.BlueMask, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.AlphaMask, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.CSType, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointRedX, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointRedY, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointRedZ, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointGreenX, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointGreenY, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointGreenZ, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointBlueX, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointBlueY, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.EndpointBlueZ, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.GammaRed, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.GammaGreen, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.GammaBlue, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.Intent, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.ProfileData, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.HeaderV5.ProfileSize, PXDataTypeInt32ULE},
                        {PXNull, PXDataTypePadding(4)},
                    };

                    PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                }

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

                    PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                }

                if(bmp.InfoHeaderType == PXBitmapHeaderOS22XBitMapHeader)
                {
                    unsigned short paddingBytes = 0; // Padding.Ignored and should be zero

                    const PXFileDataElementType pxDataStreamElementList[] =
                    {
                        {&bmp.InfoHeader.OS22XBitMap.HorizontalandVerticalResolutions, PXDataTypeInt16ULE},
                        {&paddingBytes, PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.OS22XBitMap.DirectionOfBits, PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.OS22XBitMap.halftoningAlgorithm, PXDataTypeInt16ULE},
                        {&bmp.InfoHeader.OS22XBitMap.HalftoningParameterA, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.OS22XBitMap.HalftoningParameterB, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.OS22XBitMap.ColorEncoding, PXDataTypeInt32ULE},
                        {&bmp.InfoHeader.OS22XBitMap.ApplicationDefinedByte, PXDataTypeInt32ULE}
                    };

                    PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
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
        PXColorFormat pxColorFormat;

        switch(bmp.InfoHeader.NumberOfBitsPerPixel)
        {
            case 4:
                bmp.InfoHeader.NumberOfBitsPerPixel = 24;
                pxColorFormat = PXColorFormatRGBI8;
                break;

            case 8:
                bmp.InfoHeader.NumberOfBitsPerPixel = 24;
                pxColorFormat = PXColorFormatRGBI8;
                break;

            case 24:
                pxColorFormat = PXColorFormatRGBI8;
                break;

            case 32:
                pxColorFormat = PXColorFormatRGBAI8;
                break;

            default:
                pxColorFormat = PXColorFormatInvalid;
                break;
        }

        const PXBool allocationSuccess = PXImageResize(pxImage, pxColorFormat, bmp.InfoHeader.Width, bmp.InfoHeader.Height);

        if(!allocationSuccess)
        {
            return PXActionFailedMemoryAllocation;
        }
    }

    //---[ Pixel Data ]--------------------------------------------------------
    const PXSize bbp = PXColorFormatBytePerPixel(pxImage->Format);

    PXBitmapImageDataLayout imageDataLayout;

    PXBitmapImageDataLayoutCalculate(&imageDataLayout, bmp.InfoHeader.Width, bmp.InfoHeader.Height, bmp.InfoHeader.NumberOfBitsPerPixel);

    while(imageDataLayout.RowAmount--) // loop through each image row
    { 
        PXByte* const data = (PXByte* const)pxImage->PixelData + (imageDataLayout.RowImageDataSize * imageDataLayout.RowAmount); // Get the starting point of each row

        PXFileReadB(pxResourceLoadInfo->FileReference, data, imageDataLayout.RowImageDataSize); // Read/Write image data
        PXFileCursorAdvance(pxResourceLoadInfo->FileReference, imageDataLayout.RowPaddingSize); // Skip padding

        for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += bbp)
        {
            PXQuickSwap(data[i], data[i+2]) // BGR -> RGB, just swap Blue and Red, Green stays.
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXBitmapSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceSaveInfo->ResourceTarget;

    PXBitmap bitMap;

    PXBitmapConstruct(&bitMap);

    //-----------------------------------------------------
    // Header - Write
    //-----------------------------------------------------
    {
        PXInt16UCluster byteCluster;
        PXInt32U sizeOfFile = PXBitmapFilePredictSize(pxImage->Width, pxImage->Height, PXColorFormatBitsPerPixel(pxImage->Format)) - 14u;
        PXInt32U reservedBlock = 0;
        PXInt32U dataOffset = 54u;

        byteCluster.Value = PXBitmapTypeToID(PXBitmapWindows);

        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {byteCluster.Data, PXDataTypeDatax2},
            {&sizeOfFile, PXDataTypeInt32ULE},
            {&reservedBlock, PXDataTypeInt32ULE},
            {&dataOffset, PXDataTypeInt32ULE}
        };

        PXFileWriteMultible(pxResourceSaveInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }
    //-----------------------------------------------------


    //-----------------------------------------------------
    // DIP - Write
    //-----------------------------------------------------
    {
        const PXBitmapInfoHeaderType bmpInfoHeaderType = PXBitmapHeaderBitMapInfoHeader;

        //---<Shared>----------------------------------------------------------
        bitMap.InfoHeader.HeaderSize = PXBitmapInfoHeaderTypeToID(bmpInfoHeaderType);
        bitMap.InfoHeader.NumberOfBitsPerPixel = PXColorFormatBitsPerPixel(pxImage->Format);
        bitMap.InfoHeader.NumberOfColorPlanes = 1;
        bitMap.InfoHeader.Width = pxImage->Width;
        bitMap.InfoHeader.Height = pxImage->Height;
        //---------------------------------------------------------------------

        PXFileWriteI32UE(pxResourceSaveInfo->FileReference, bitMap.InfoHeader.HeaderSize, PXEndianLittle);

        switch (bmpInfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
            {
                bitMap.InfoHeader.BitMapInfo.HorizontalResolution = 1u;
                bitMap.InfoHeader.BitMapInfo.VerticalResolution = 1u;

                const PXFileDataElementType pxDataStreamElementList[] =
                {
                    {&bitMap.InfoHeader.Width, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.Height, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.NumberOfColorPlanes, PXDataTypeInt16ULE},
                    {&bitMap.InfoHeader.NumberOfBitsPerPixel, PXDataTypeInt16ULE},
                    {&bitMap.InfoHeader.BitMapInfo.CompressionMethod, PXDataTypeInt32ULE},
                    {&bitMap.InfoHeader.BitMapInfo.ImageSize, PXDataTypeInt32ULE},
                    {&bitMap.InfoHeader.BitMapInfo.HorizontalResolution, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.BitMapInfo.VerticalResolution, PXDataTypeInt32SLE},
                    {&bitMap.InfoHeader.BitMapInfo.NumberOfColorsInTheColorPalette, PXDataTypeInt32ULE},
                    {&bitMap.InfoHeader.BitMapInfo.NumberOfImportantColorsUsed, PXDataTypeInt32ULE},
                };

                PXFileWriteMultible(pxResourceSaveInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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
            const PXByte* const dataInsertPoint = (const PXByte* const)pxImage->PixelData + (imageDataLayout.RowImageDataSize * row);

            for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += 3) // Will result in RGB Pixel Data
            {
                const PXByte pixelBuffer[3u] = 
                {                 
                    dataInsertPoint[i+2u], 
                    dataInsertPoint[i+1u],
                    dataInsertPoint[i]
                };

                PXFileWriteB(pxResourceSaveInfo->FileReference, pixelBuffer, 3u);
            }

            PXFileWriteFill(pxResourceSaveInfo->FileReference, 0, imageDataLayout.RowPaddingSize);
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
    PXDeleteList(PXByte, bmp->PixelDataSize, &bmp->PixelData, &bmp->PixelDataSize);
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
    const PXSize sizeBitmapHeader = 14u;
    const PXSize sizeBitmapDIP = 40u;
    const PXSize imageDataSize = (PXMathFloorD((width * bitsPerPixel + 31u) / 32.0f) * 4u) * height; // (PXMathFloorD((width * bitsPerPixel + 31u) / 32.0f) * 4u) * height;
    const PXSize fullSize = sizeBitmapHeader + sizeBitmapDIP + imageDataSize+512u;

    return fullSize;
}