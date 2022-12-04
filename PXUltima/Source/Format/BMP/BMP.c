#include "BMP.h"

#include <Container/ClusterValue.h>
#include <Container/ClusterValue.h>
#include <File/DataStream.h>
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

ActionResult BMPParse(BMP* const bmp, DataStream* const dataStream)
{
    BMPConstruct(bmp);

    //---[ Parsing Header ]----------------------------------------------------
    {
        ClusterShort byteCluster;
        unsigned int sizeOfFile = 0;
        unsigned int reservedBlock = 0;
        unsigned int dataOffset = 0;

        DataStreamReadP(&dataStream, byteCluster.Data, 2u);
        DataStreamReadIU(&dataStream, &sizeOfFile, EndianLittle);
        DataStreamReadIU(&dataStream, &reservedBlock, EndianLittle);
        DataStreamReadIU(&dataStream, &dataOffset, EndianLittle);

        const BMPType type = ConvertToBMPType(byteCluster.Value);

        {
            const unsigned char isValidType = type != BMPInvalid;

            if(!isValidType)
            {
                return ActionInvalidHeaderSignature;
            }
        }

        bmp->Type = type;
    }
    //-------------------------------------------------------------------------

    //---[ DIP ]---------------------------------------------------------------
    {
        DataStreamReadIU(&dataStream, &bmp->InfoHeader.HeaderSize, EndianLittle);

        bmp->InfoHeaderType = ConvertToBMPInfoHeaderType(bmp->InfoHeader.HeaderSize);

        switch(bmp->InfoHeaderType)
        {
            case BitMapInfoHeader:
            {
                DataStreamReadI(&dataStream, &bmp->InfoHeader.Width, EndianLittle);
                DataStreamReadI(&dataStream, &bmp->InfoHeader.Height, EndianLittle);
                DataStreamReadSU(&dataStream, &bmp->InfoHeader.NumberOfColorPlanes, EndianLittle);
                DataStreamReadSU(&dataStream, &bmp->InfoHeader.NumberOfBitsPerPixel, EndianLittle);
                DataStreamReadIU(&dataStream, &bmp->InfoHeader.CompressionMethod, EndianLittle);
                DataStreamReadIU(&dataStream, &bmp->InfoHeader.ImageSize, EndianLittle);
                DataStreamReadI(&dataStream, &bmp->InfoHeader.HorizontalResolution, EndianLittle);
                DataStreamReadI(&dataStream, &bmp->InfoHeader.VerticalResolution, EndianLittle);
                DataStreamReadIU(&dataStream, &bmp->InfoHeader.NumberOfColorsInTheColorPalette, EndianLittle);
                DataStreamReadIU(&dataStream, &bmp->InfoHeader.NumberOfImportantColorsUsed, EndianLittle);

                break;
            }
            case OS21XBitMapHeader:
            case OS22XBitMapHeader:
            {
                unsigned short width = 0;
                unsigned short height = 0;

                DataStreamReadSU(&dataStream, &width, EndianLittle);
                DataStreamReadSU(&dataStream, &height, EndianLittle);
                DataStreamReadSU(&dataStream, &bmp->InfoHeader.NumberOfColorPlanes, EndianLittle);
                DataStreamReadSU(&dataStream, &bmp->InfoHeader.NumberOfBitsPerPixel, EndianLittle);

                bmp->InfoHeader.Width = width;
                bmp->InfoHeader.Height = height;

                if(bmp->InfoHeaderType == OS22XBitMapHeader)
                {
                    unsigned short paddingBytes = 0; // Padding.Ignored and should be zero

                    DataStreamReadSU(&dataStream, &bmp->InfoHeader.HorizontalandVerticalResolutions, EndianLittle);
                    DataStreamReadSU(&dataStream, &paddingBytes, EndianLittle);
                    DataStreamReadSU(&dataStream, &bmp->InfoHeader.DirectionOfBits, EndianLittle);
                    DataStreamReadSU(&dataStream, &bmp->InfoHeader.halftoningAlgorithm, EndianLittle);

                    DataStreamReadIU(&dataStream, &bmp->InfoHeader.HalftoningParameterA, EndianLittle);
                    DataStreamReadIU(&dataStream, &bmp->InfoHeader.HalftoningParameterB, EndianLittle);
                    DataStreamReadIU(&dataStream, &bmp->InfoHeader.ColorEncoding, EndianLittle);
                    DataStreamReadIU(&dataStream, &bmp->InfoHeader.ApplicationDefinedByte, EndianLittle);
                }

                break;
            }
            default:
            {
                // Unkown Header
                return ResultFormatNotSupported;
            }
        }
    }
    //-----------------------------------------------------------

    bmp->PixelDataSize = bmp->InfoHeader.Width * bmp->InfoHeader.Height * (bmp->InfoHeader.NumberOfBitsPerPixel / 8);
    bmp->PixelData = MemoryAllocate(sizeof(unsigned char) * bmp->PixelDataSize);

    // Check if enogh memory
    {
        const unsigned char allocationSucessful = bmp->PixelData;

        if(!allocationSucessful)
        {
            return ActionSystemOutOfMemory;
        }
    }

    //---[ Pixel Data ]--------------------------------------------------------
    BMPImageDataLayout imageDataLayout;
    PXSize pixelDataOffset = 0;

    BMPImageDataLayoutCalculate(&imageDataLayout, bmp->InfoHeader.Width, bmp->InfoHeader.Height, bmp->InfoHeader.NumberOfBitsPerPixel);

    while(imageDataLayout.RowAmount--)
    {      
        unsigned char* data = bmp->PixelData + pixelDataOffset;

        pixelDataOffset += DataStreamReadP(&dataStream, data, imageDataLayout.RowImageDataSize);
        DataStreamCursorAdvance(&dataStream, imageDataLayout.RowPaddingSize);
    }

    return ActionSuccessful;
}

ActionResult BMPParseToImage(Image* const image, DataStream* const dataStream)
{
    BMP bmp;
    
    BMPConstruct(&bmp);

    //---[ Parsing Header ]----------------------------------------------------
    {
        ClusterShort byteCluster;
        unsigned int sizeOfFile = 0;
        unsigned int reservedBlock = 0;
        unsigned int dataOffset = 0;

        DataStreamReadP(dataStream, byteCluster.Data, 2u);
        DataStreamReadIU(dataStream, &sizeOfFile, EndianLittle);
        DataStreamReadIU(dataStream, &reservedBlock, EndianLittle);
        DataStreamReadIU(dataStream, &dataOffset, EndianLittle);

        const BMPType type = ConvertToBMPType(byteCluster.Value);

        {
            const PXBool isValidType = type != BMPInvalid;

            if(!isValidType)
            {
                return ActionInvalidHeaderSignature;
            }
        }

        bmp.Type = type;
    }
    //-------------------------------------------------------------------------

    //---[ DIP ]---------------------------------------------------------------
    {
        DataStreamReadIU(dataStream, &bmp.InfoHeader.HeaderSize, EndianLittle);

        bmp.InfoHeaderType = ConvertToBMPInfoHeaderType(bmp.InfoHeader.HeaderSize);

        switch(bmp.InfoHeaderType)
        {
            case BitMapInfoHeader:
            {
                DataStreamReadI(dataStream, &bmp.InfoHeader.Width, EndianLittle);
                DataStreamReadI(dataStream, &bmp.InfoHeader.Height, EndianLittle);
                DataStreamReadSU(dataStream, &bmp.InfoHeader.NumberOfColorPlanes, EndianLittle);
                DataStreamReadSU(dataStream, &bmp.InfoHeader.NumberOfBitsPerPixel, EndianLittle);
                DataStreamReadIU(dataStream, &bmp.InfoHeader.CompressionMethod, EndianLittle);
                DataStreamReadIU(dataStream, &bmp.InfoHeader.ImageSize, EndianLittle);
                DataStreamReadI(dataStream, &bmp.InfoHeader.HorizontalResolution, EndianLittle);
                DataStreamReadI(dataStream, &bmp.InfoHeader.VerticalResolution, EndianLittle);
                DataStreamReadIU(dataStream, &bmp.InfoHeader.NumberOfColorsInTheColorPalette, EndianLittle);
                DataStreamReadIU(dataStream, &bmp.InfoHeader.NumberOfImportantColorsUsed, EndianLittle);
                break;
            }
            case OS21XBitMapHeader:
            case OS22XBitMapHeader:
            {
                unsigned short width = 0;
                unsigned short height = 0;

                DataStreamReadSU(dataStream, &width, EndianLittle);
                DataStreamReadSU(dataStream, &height, EndianLittle);
                DataStreamReadSU(dataStream, &bmp.InfoHeader.NumberOfColorPlanes, EndianLittle);
                DataStreamReadSU(dataStream, &bmp.InfoHeader.NumberOfBitsPerPixel, EndianLittle);

                bmp.InfoHeader.Width = width;
                bmp.InfoHeader.Height = height;

                if(bmp.InfoHeaderType == OS22XBitMapHeader)
                {
                    unsigned short paddingBytes = 0; // Padding.Ignored and should be zero

                    DataStreamReadSU(dataStream, &bmp.InfoHeader.HorizontalandVerticalResolutions, EndianLittle);
                    DataStreamReadSU(dataStream, &paddingBytes, EndianLittle);
                    DataStreamReadSU(dataStream, &bmp.InfoHeader.DirectionOfBits, EndianLittle);
                    DataStreamReadSU(dataStream, &bmp.InfoHeader.halftoningAlgorithm, EndianLittle);

                    DataStreamReadIU(dataStream, &bmp.InfoHeader.HalftoningParameterA, EndianLittle);
                    DataStreamReadIU(dataStream, &bmp.InfoHeader.HalftoningParameterB, EndianLittle);
                    DataStreamReadIU(dataStream, &bmp.InfoHeader.ColorEncoding, EndianLittle);
                    DataStreamReadIU(dataStream, &bmp.InfoHeader.ApplicationDefinedByte, EndianLittle);
                }

                break;
            }
            default:
            {
                // Unkown Header
                return ResultFormatNotSupported;
            }
        }
    }
    //-----------------------------------------------------------


    // Generate imagedata
    {
        const PXBool allocationSuccess = ImageResize(image, ImageDataFormatRGB8, bmp.InfoHeader.Width, bmp.InfoHeader.Height);

        if(!allocationSuccess)
        {
            return ActionSystemOutOfMemory;
        }
    }

    //---[ Pixel Data ]--------------------------------------------------------
    BMPImageDataLayout imageDataLayout;

    BMPImageDataLayoutCalculate(&imageDataLayout, bmp.InfoHeader.Width, bmp.InfoHeader.Height, bmp.InfoHeader.NumberOfBitsPerPixel);

    while(imageDataLayout.RowAmount--) // loop through each image row
    {
        PXByte* const data = (PXByte* const)image->PixelData + (imageDataLayout.RowFullSize * imageDataLayout.RowAmount); // Get the starting point of each row

        DataStreamReadP(dataStream, data, imageDataLayout.RowImageDataSize); // Read/Write image data
        DataStreamCursorAdvance(dataStream, imageDataLayout.RowPaddingSize); // Skip padding

        for(PXSize i = 0; i < imageDataLayout.RowImageDataSize; i += 3)
        {
            const PXByte swap = data[i]; // Save Blue(current)

            data[i] = data[i + 2]; // Override Blue(current) with Red(new)
            data[i+2] = swap; // Override Red(current) with Blue(new) 
        }
    }

    return ActionSuccessful;
}

ActionResult BMPSerialize(const BMP* const bmp, DataStream* const dataStream)
{
    return ActionSuccessful;
}

ActionResult BMPSerializeFromImage(const Image* const image, DataStream* const dataStream)
{
    BMPInfoHeader bmpInfoHeader;

    DataStreamConstruct(dataStream);

    //---<Header>-----
    {
        ClusterShort byteCluster;
        unsigned int sizeOfFile = dataStream->DataSize;
        unsigned int reservedBlock = 0;
        unsigned int dataOffset = 54u;

        byteCluster.Value = ConvertFromBMPType(BMPWindows);

        DataStreamWriteP(dataStream, byteCluster.Data, 2u);
        DataStreamWriteIU(dataStream, sizeOfFile, EndianLittle);
        DataStreamWriteIU(dataStream, reservedBlock, EndianLittle);
        DataStreamWriteIU(dataStream, dataOffset, EndianLittle);
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

        DataStreamWriteIU(dataStream, bmpInfoHeader.HeaderSize, EndianLittle);

        switch (bmpInfoHeaderType)
        {
            case BitMapInfoHeader:
            {
                DataStreamWriteI(dataStream, bmpInfoHeader.Width, EndianLittle);
                DataStreamWriteI(dataStream, bmpInfoHeader.Height, EndianLittle);
                DataStreamWriteSU(dataStream, bmpInfoHeader.NumberOfColorPlanes, EndianLittle);
                DataStreamWriteSU(dataStream, bmpInfoHeader.NumberOfBitsPerPixel, EndianLittle);
                DataStreamWriteIU(dataStream, bmpInfoHeader.CompressionMethod, EndianLittle);
                DataStreamWriteIU(dataStream, bmpInfoHeader.ImageSize, EndianLittle);
                DataStreamWriteI(dataStream, bmpInfoHeader.HorizontalResolution, EndianLittle);
                DataStreamWriteI(dataStream, bmpInfoHeader.VerticalResolution, EndianLittle);
                DataStreamWriteIU(dataStream, bmpInfoHeader.NumberOfColorsInTheColorPalette, EndianLittle);
                DataStreamWriteIU(dataStream, bmpInfoHeader.NumberOfImportantColorsUsed, EndianLittle);
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

                DataStreamWriteP(dataStream, pixelBuffer, 3u);
            }

            DataStreamWriteFill(dataStream, 0, imageDataLayout.RowPaddingSize);
        }
    }

    return ActionSuccessful;
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