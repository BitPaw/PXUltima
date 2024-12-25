#include "PXBitmap.h"

#include <Math/PXMath.h>
#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

#define PXQuickSwap(a, b) \
a = a + b; \
b = a - b; \
a = a - b;

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

//-------------------------------------
// Layout of bitmap file
//-------------------------------------
const PXInt32U PXBitMapHeader[] =
{
    PXDataTypeText(2),
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE
};
const PXInt8U PXBitMapHeaderSize = sizeof(PXBitMapHeader) / sizeof(PXInt32U);





const PXInt32U PXBitMapInfoHeaderTypeList[] =
{
    PXDataTypeInt32SLE,
    PXDataTypeInt32SLE,
    PXDataTypeInt16ULE,
    PXDataTypeInt16ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32SLE,
    PXDataTypeInt32SLE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE
};
const PXInt8U PXBitMapInfoHeaderTypeListSize = sizeof(PXBitMapInfoHeaderTypeList) / sizeof(PXInt32U);


const PXInt32U PXBitMapV5HeaderTypeList[] =
{
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypePadding(4)
};
const PXInt8U PXBitMapV5HeaderTypeListSize = sizeof(PXBitMapV5HeaderTypeList) / sizeof(PXInt32U);




const PXInt32U PXBitmapHeaderOS21XBitMapHeaderTypeList[] =
{
    PXDataTypeInt16ULE,
    PXDataTypeInt16ULE,
    PXDataTypeInt16ULE,
    PXDataTypeInt16ULE
};
const PXInt8U PXBitmapHeaderOS21XBitMapHeaderTypeListSize = sizeof(PXBitmapHeaderOS21XBitMapHeaderTypeList) / sizeof(PXInt32U);


const PXInt32U PXBitmapHeaderOS22XList[] =
{
    PXDataTypeInt16ULE,
    PXDataTypePadding(2),
    PXDataTypeInt16ULE,
    PXDataTypeInt16ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE,
    PXDataTypeInt32ULE
};
const PXInt8U PXBitmapHeaderOS22XListSize = sizeof(PXBitmapHeaderOS22XList) / sizeof(PXInt32U);




PXActionResult PXAPI PXBitmapPeekFromFile(PXResourceTransphereInfo* const pxResourceTransphereInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceTransphereInfo->ResourceTarget;

    PXBitmap* bmp = PXMemoryCallocT(PXBitmap, 1);

    pxResourceTransphereInfo->ResourceSource = bmp;

    //---[ Parsing Header ]----------------------------------------------------
    {
        PXFileBinding(pxResourceTransphereInfo->FileReference, &bmp->HeaderData, PXBitMapHeader, PXBitMapHeaderSize, PXFileBindingRead);

        bmp->Type = (PXBitmapType)PXInt16Make(bmp->HeaderData.Type[0], bmp->HeaderData.Type[1]);
        bmp->InfoHeaderType = PXBitmapInfoHeaderTypeFromID(bmp->HeaderData.HeaderSize);

        {
            const PXBool isValidType = PXBitmapInvalid != bmp->Type;

            if(!isValidType)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }
    }
    //-------------------------------------------------------------------------

    //---[ DIP ]---------------------------------------------------------------
    {
        switch(bmp->InfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
            case PXBitmapHeaderBitMapV5Header:
            {
                PXFileBinding
                (
                    pxResourceTransphereInfo->FileReference,
                    &bmp->InfoHeader, 
                    PXBitMapInfoHeaderTypeList,
                    PXBitMapInfoHeaderTypeListSize, 
                    PXFileBindingRead
                );

                if(PXBitmapHeaderBitMapV5Header == bmp->InfoHeaderType)
                {
                    PXFileBinding
                    (
                        pxResourceTransphereInfo->FileReference,
                        &bmp->HeaderV5,
                        PXBitMapV5HeaderTypeList,
                        PXBitMapV5HeaderTypeListSize,
                        PXFileBindingRead
                    );
                }

                break;
            }
            case PXBitmapHeaderOS21XBitMapHeader:
            case PXBitmapHeaderOS22XBitMapHeader:
            {
                PXFileBinding
                (
                    pxResourceTransphereInfo->FileReference,
                    &bmp->HeaderOS21X, 
                    PXBitmapHeaderOS21XBitMapHeaderTypeList,
                    PXBitmapHeaderOS21XBitMapHeaderTypeListSize,
                    PXFileBindingRead
                );

                if(PXBitmapHeaderOS22XBitMapHeader == bmp->InfoHeaderType)
                {
                    PXFileBinding
                    (
                        pxResourceTransphereInfo->FileReference,
                        &bmp->HeaderOS22X,
                        PXBitmapHeaderOS22XList,
                        PXBitmapHeaderOS22XListSize,
                        PXFileBindingRead
                    );
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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXBitmapLoadFromFile(PXResourceTransphereInfo* const pxResourceTransphereInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceTransphereInfo->ResourceTarget;

    PXBitmap* bmp = (PXBitmap*)pxResourceTransphereInfo->ResourceSource;

    // Generate imagedata
    {
        PXColorFormat pxColorFormat;

        switch(bmp->InfoHeader.NumberOfBitsPerPixel)
        {
            case 4:
                bmp->InfoHeader.NumberOfBitsPerPixel = 24;
                pxColorFormat = PXColorFormatRGBI8;
                break;

            case 8:
                bmp->InfoHeader.NumberOfBitsPerPixel = 24;
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

        const PXBool allocationSuccess = PXImageResize(pxImage, pxColorFormat, bmp->InfoHeader.Width, bmp->InfoHeader.Height);

        if(!allocationSuccess)
        {
            return PXActionFailedMemoryAllocation;
        }
    }

    //---[ Pixel Data ]--------------------------------------------------------
    const PXSize bbp = PXColorFormatBytePerPixel(pxImage->Format);

    PXBitmapImageDataLayout imageDataLayout;

    PXBitmapImageDataLayoutCalculate(&imageDataLayout, bmp->InfoHeader.Width, bmp->InfoHeader.Height, bmp->InfoHeader.NumberOfBitsPerPixel);

    while(imageDataLayout.RowAmount--) // loop through each image row
    {
        PXByte* const data = (PXByte* const)pxImage->PixelData + (imageDataLayout.RowImageDataSize * imageDataLayout.RowAmount); // Get the starting point of each row

        PXFileReadB(pxResourceTransphereInfo->FileReference, data, imageDataLayout.RowImageDataSize); // Read/Write image data
        PXFileCursorAdvance(pxResourceTransphereInfo->FileReference, imageDataLayout.RowPaddingSize); // Skip padding

        for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += bbp)
        {
            PXQuickSwap(data[i], data[i + 2]) // BGR -> RGB, just swap Blue and Red, Green stays.
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXBitmapSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceSaveInfo->ResourceTarget;


    PXBitmap bitMap;

    //-----------------------------------------------------
    // Header - Write
    //-----------------------------------------------------
    {
        //bitMap.HeaderData.Type = IIIIIIIIIIIIIIIIII;
        bitMap.HeaderData.SizeOfFile = PXBitmapFilePredictSize(pxImage->Width, pxImage->Height, PXColorFormatBitsPerPixel(pxImage->Format)) - 14u;
        bitMap.HeaderData.ReservedBlock = 0;
        bitMap.HeaderData.DataOffset = 54u;
        //byteCluster.Value = PXBitmapTypeToID(PXBitmapWindows);

        PXFileBinding
        (
            pxResourceSaveInfo->FileReference,
            &bitMap.HeaderData,
            PXBitMapHeader,
            PXBitMapHeaderSize,
            PXFileBindingWrite
        );
    }
    //-----------------------------------------------------


    //-----------------------------------------------------
    // DIP - Write
    //-----------------------------------------------------
    {
        const PXBitmapInfoHeaderType bmpInfoHeaderType = PXBitmapHeaderBitMapInfoHeader;

        //---<Shared>----------------------------------------------------------
     //   bitMap.InfoHeader.HeaderSize = PXBitmapInfoHeaderTypeToID(bmpInfoHeaderType);
        bitMap.InfoHeader.NumberOfBitsPerPixel = PXColorFormatBitsPerPixel(pxImage->Format);
        bitMap.InfoHeader.NumberOfColorPlanes = 1;
        bitMap.InfoHeader.Width = pxImage->Width;
        bitMap.InfoHeader.Height = pxImage->Height;
        //---------------------------------------------------------------------

     //   PXFileWriteI32UE(pxResourceSaveInfo->FileReference, bitMap.InfoHeader.HeaderSize, PXEndianLittle);

        switch(bmpInfoHeaderType)
        {
            case PXBitmapHeaderBitMapInfoHeader:
            {
             //   bitMap.InfoHeader.BitMapInfo.HorizontalResolution = 1u;
             //   bitMap.InfoHeader.BitMapInfo.VerticalResolution = 1u;

                PXFileBinding
                (
                    pxResourceSaveInfo->FileReference,
                    &bitMap.InfoHeader,
                    PXBitMapInfoHeaderTypeList,
                    PXBitMapInfoHeaderTypeListSize,
                    PXFileBindingWrite
                );

                break;
            }
        }
    }
    //-----------------------------------------------------

    {
        PXBitmapImageDataLayout imageDataLayout;

        PXBitmapImageDataLayoutCalculate(&imageDataLayout, bitMap.InfoHeader.Width, bitMap.InfoHeader.Height, bitMap.InfoHeader.NumberOfBitsPerPixel);

        for(PXSize row = imageDataLayout.RowAmount - 1; row != (PXSize)-1; --row)
        {
            const PXByte* const dataInsertPoint = (const PXByte* const)pxImage->PixelData + (imageDataLayout.RowImageDataSize * row);

            for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += 3) // Will result in RGB Pixel Data
            {
                const PXByte pixelBuffer[3u] =
                {
                    dataInsertPoint[i + 2u],
                    dataInsertPoint[i + 1u],
                    dataInsertPoint[i]
                };

                PXFileWriteB(pxResourceSaveInfo->FileReference, pixelBuffer, 3u);
            }

            PXFileWriteFill(pxResourceSaveInfo->FileReference, 0, imageDataLayout.RowPaddingSize);
        }
    }

    return PXActionSuccessful;
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
    const PXSize fullSize = sizeBitmapHeader + sizeBitmapDIP + imageDataSize + 512u;

    return fullSize;
}
