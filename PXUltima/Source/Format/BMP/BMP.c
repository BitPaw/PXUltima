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
            return OS21XBitMapHeader;

        case 16u:
            return OS22XBitMapHeader;

        case 40u:
            return BitMapInfoHeader;

        case 52u:
            return BitMapV2InfoHeader;

        case 56u:
            return BitMapV3InfoHeader;

        case 108u:
            return BitMapV4Header;

        case 124u:
            return BitMapV5Header;

        default:
            return UnkownOrInvalid;
    }
}

unsigned int ConvertFromBMPInfoHeaderType(const BMPInfoHeaderType infoHeaderType)
{
    switch(infoHeaderType)
    {
        default:
        case UnkownOrInvalid:
            return -1;

        case BitMapCoreHeader:
        case OS21XBitMapHeader:
            return 12u;

        case OS22XBitMapHeader:
            return 16u;

        case BitMapInfoHeader:
            return 40u;

        case BitMapV2InfoHeader:
            return 52u;

        case BitMapV3InfoHeader:
            return 56u;

        case BitMapV4Header:
            return 108u;

        case BitMapV5Header:
            return 124u;
    }
}

PXActionResult BMPParseToImage(Image* const image, PXDataStream* const dataStream)
{
    BMP bmp;
    
    BMPConstruct(&bmp);

    unsigned int sizeOfFile = 0;
    unsigned int reservedBlock = 0;
    unsigned int dataOffset = 0;

    //---[ Parsing Header ]----------------------------------------------------
    {
        ClusterShort byteCluster;
        unsigned int valueList[3];  

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
        PXDataStreamReadI32U(dataStream, &bmp.InfoHeader.HeaderSize, EndianLittle);

        bmp.InfoHeaderType = ConvertToBMPInfoHeaderType(bmp.InfoHeader.HeaderSize);

        switch(bmp.InfoHeaderType)
        {
            case BitMapInfoHeader:
            {
                const PXDataStreamElementType pxDataStreamElementList[] =
                {
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.Width},
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.Height},
                    {PXDataTypeLEInt16U, &bmp.InfoHeader.NumberOfColorPlanes},
                    {PXDataTypeLEInt16U, &bmp.InfoHeader.NumberOfBitsPerPixel},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.CompressionMethod},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.ImageSize},
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.HorizontalResolution},
                    {PXDataTypeLEInt32S, &bmp.InfoHeader.VerticalResolution},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.NumberOfColorsInTheColorPalette},
                    {PXDataTypeLEInt32U, &bmp.InfoHeader.NumberOfImportantColorsUsed},
                };
                const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXDataStreamElementType);

                PXDataStreamReadMultible(dataStream, pxDataStreamElementList, pxDataStreamElementListSize);

                break;
            }
            case OS21XBitMapHeader:
            case OS22XBitMapHeader:
            {
                {
                    PXInt16U valueList[4u];

                    PXDataStreamReadI16UVE(dataStream, valueList, 4u, EndianLittle);

                    bmp.InfoHeader.Width = valueList[0];
                    bmp.InfoHeader.Height = valueList[1];
                    bmp.InfoHeader.NumberOfColorPlanes = valueList[2];
                    bmp.InfoHeader.NumberOfBitsPerPixel = valueList[3];
                }

                if(bmp.InfoHeaderType == OS22XBitMapHeader)
                {
                    unsigned short paddingBytes = 0; // Padding.Ignored and should be zero

                    const PXDataStreamElementType pxDataStreamElementList[] =
                    {
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.HorizontalandVerticalResolutions},
                        {PXDataTypeLEInt16U, &paddingBytes},
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.DirectionOfBits},
                        {PXDataTypeLEInt16U, &bmp.InfoHeader.halftoningAlgorithm},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.HalftoningParameterA},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.HalftoningParameterB},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.ColorEncoding},
                        {PXDataTypeLEInt32U, &bmp.InfoHeader.ApplicationDefinedByte}
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
    BMPInfoHeader bmpInfoHeader;

    PXDataStreamConstruct(dataStream);

    //---<Header>-----
    {
        ClusterShort byteCluster;
        unsigned int sizeOfFile = dataStream->DataSize;
        unsigned int reservedBlock = 0;
        unsigned int dataOffset = 54u;

        byteCluster.Value = ConvertFromBMPType(BMPWindows);

        PXDataStreamWriteP(dataStream, byteCluster.Data, 2u);
        PXDataStreamWriteIU(dataStream, sizeOfFile, EndianLittle);
        PXDataStreamWriteIU(dataStream, reservedBlock, EndianLittle);
        PXDataStreamWriteIU(dataStream, dataOffset, EndianLittle);
    }
    //----------------

    //---<DIP>
    {
        const BMPInfoHeaderType bmpInfoHeaderType = BitMapInfoHeader; 
        
        //---<Shared>----------------------------------------------------------
        bmpInfoHeader.HeaderSize = ConvertFromBMPInfoHeaderType(bmpInfoHeaderType);
        bmpInfoHeader.NumberOfBitsPerPixel = ImageBytePerPixel(image->Format) * 8u;
        bmpInfoHeader.NumberOfColorPlanes = 1;
        bmpInfoHeader.Width = image->Width;
        bmpInfoHeader.Height = image->Height;
        //---------------------------------------------------------------------

        //---<BitMapInfoHeader ONLY>-------------------------------------------	
        bmpInfoHeader.CompressionMethod = 0; // [4-Bytes] compression method being used.See the next table for a list of possible values	
        bmpInfoHeader.ImageSize = 0; 	// [4-Bytes] image size.This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
        bmpInfoHeader.HorizontalResolution = 1u;	
        bmpInfoHeader.VerticalResolution = 1u;
        bmpInfoHeader.NumberOfColorsInTheColorPalette = 0;
        bmpInfoHeader.NumberOfImportantColorsUsed = 0;
        //---------------------------------------------------------------------

        PXDataStreamWriteIU(dataStream, bmpInfoHeader.HeaderSize, EndianLittle);

        switch (bmpInfoHeaderType)
        {
            case BitMapInfoHeader:
            {

                PXDataStreamWriteI(dataStream, bmpInfoHeader.Width, EndianLittle);
                PXDataStreamWriteI(dataStream, bmpInfoHeader.Height, EndianLittle);
                PXDataStreamWriteSU(dataStream, bmpInfoHeader.NumberOfColorPlanes, EndianLittle);
                PXDataStreamWriteSU(dataStream, bmpInfoHeader.NumberOfBitsPerPixel, EndianLittle);
                PXDataStreamWriteIU(dataStream, bmpInfoHeader.CompressionMethod, EndianLittle);
                PXDataStreamWriteIU(dataStream, bmpInfoHeader.ImageSize, EndianLittle);
                PXDataStreamWriteI(dataStream, bmpInfoHeader.HorizontalResolution, EndianLittle);
                PXDataStreamWriteI(dataStream, bmpInfoHeader.VerticalResolution, EndianLittle);
                PXDataStreamWriteIU(dataStream, bmpInfoHeader.NumberOfColorsInTheColorPalette, EndianLittle);
                PXDataStreamWriteIU(dataStream, bmpInfoHeader.NumberOfImportantColorsUsed, EndianLittle);
                break;
            }
        }
    }
    //------------
    
    {
        BMPImageDataLayout imageDataLayout;

        BMPImageDataLayoutCalculate(&imageDataLayout, bmpInfoHeader.Width, bmpInfoHeader.Height, bmpInfoHeader.NumberOfBitsPerPixel);  

        for (PXSize row = imageDataLayout.RowAmount-1; row != (PXSize)-1  ; --row)
        {
            const PXByte* const dataInsertPoint = (const PXByte* const)image->PixelData + (imageDataLayout.RowImageDataSize * row);

            for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += 3) // Will result in RGB Pixel Data
            {
                PXByte pixelBuffer[3u];

                pixelBuffer[2u] = dataInsertPoint[i]; // Blue
                pixelBuffer[1u] = dataInsertPoint[i+1u]; // Green 
                pixelBuffer[0u] = dataInsertPoint[i+2u]; // Red

                PXDataStreamWriteP(dataStream, pixelBuffer, 3u);
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