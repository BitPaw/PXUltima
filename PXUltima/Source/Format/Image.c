#include "Image.h"

#include <Memory/PXMemory.h>
#include <File/File.h>
#include <Text/Text.h>
#include <File/DataStream.h>
#include <Math/PXMath.h>

#include <Format/BMP/BMP.h>
#include <Format/GIF/GIF.h>
#include <Format/JPEG/JPEG.h>
#include <Format/PNG/PNG.h>
#include <Format/TGA/TGA.h>
#include <Format/TIFF/TIFF.h>
#include <Format/FNT/FNT.h>
#include <Format/Font.h>

PXSize ImageBitDepth(const ImageDataFormat imageDataFormat)
{
    switch (imageDataFormat)
    {
    default:
    case ImageDataFormatInvalid:
        return 0;

    case ImageDataFormatAlphaMask:
    case ImageDataFormatAlphaMaskBinary:
        return 1;

    case ImageDataFormatBGR8:
    case ImageDataFormatRGB8:
    case ImageDataFormatRGBA8:
    case ImageDataFormatBGRA8:
        return 8;

    case ImageDataFormatRGB16:
    case ImageDataFormatRGBA16:
        return 16;
    }
}

PXSize ImageBytePerPixel(const ImageDataFormat imageDataFormat)
{
    switch(imageDataFormat)
    {
        default:
        case ImageDataFormatInvalid:
            return 0;

        case ImageDataFormatAlphaMask:
        case ImageDataFormatAlphaMaskBinary:
            return 1;

        case ImageDataFormatBGR8:
        case ImageDataFormatRGB8:
            return 3;

        case ImageDataFormatRGBA8:
        case ImageDataFormatBGRA8:
            return 4;
    }
}

PXSize ImageBitsPerPixel(const ImageDataFormat imageDataFormat)
{
    return ImageBytePerPixel(imageDataFormat) * 8u;
}

void ImageConstruct(Image* const image)
{
    MemorySet(image, sizeof(Image), 0);
}

void ImageDestruct(Image* const image)
{
    MemoryRelease(image->PixelData, image->PixelDataSize);

    ImageConstruct(image);
}

ActionResult ImageLoadA(Image* image, const char* filePath)
{
    wchar_t filePathW[PathMaxSize];

    TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

    ActionResult actionResult = ImageLoadW(image, filePathW);

    return actionResult;  
}

ActionResult ImageLoadW(Image* const image, const wchar_t* const filePath)
{
    DataStream dataStream;

    DataStreamConstruct(&dataStream);    
    ImageConstruct(image);

    {
        const ActionResult fileLoadingResult = DataStreamMapToMemoryW(&dataStream, filePath, 0, MemoryReadOnly);
        const PXBool sucessful = fileLoadingResult == ActionSuccessful;

        if(!sucessful)
        {
            return fileLoadingResult;
        }
    }

    {
        const FileFormatExtension imageFormatHint = FilePathExtensionDetectTryW(filePath, PathMaxSize);
        const ActionResult fileParsingResult = ImageLoadD(image, &dataStream, imageFormatHint);
        const PXBool success = fileParsingResult == ActionSuccessful;

        if(success)
        {
            return ActionSuccessful;
        }

        ActionResult fileGuessResult = ActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = ImageLoadD(image, dataStream.Data, dataStream.DataSize, imageFileFormat);

            fileFormatID++;
        }
        while(fileGuessResult == ActionInvalidHeaderSignature);

        return fileGuessResult;
    }

    DataStreamDestruct(&dataStream);
}

ActionResult ImageLoadD(Image* const image, DataStream* const dataStream, const FileFormatExtension guessedFormat)
{
    PXSize bytesRead = 0;
    ParseToImage parseToImage = 0;

    switch(guessedFormat)
    {
        case FileFormatBitMap:
        {
            parseToImage = BMPParseToImage;
            break;
        }
        case FileFormatGIF:
        {
            parseToImage = GIFParseToImage;
            break;
        }
        case FileFormatJPEG:
        {
            parseToImage = JPEGParseToImage;
            break;
        }
        case FileFormatPNG:
        {
            parseToImage = PNGParseToImage;
            break;
        }
        case FileFormatTGA:
        {
            parseToImage = TGAParseToImage;
            break;
        }
        case FileFormatTagImage:
        {
            parseToImage = TIFFParseToImage;
            break;
        }
        default:
        {
            return ResultFormatNotSupported;
        }
    }

    const ActionResult actionResult = parseToImage(image, dataStream);

    return actionResult;
}

ActionResult ImageSaveA(Image* const image, const char* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat)
{
    wchar_t filePathW[PathMaxSize];

    TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

    ActionResult actionResult = ImageSaveW(image, filePathW, fileFormat, dataFormat);

    return actionResult;
}

ActionResult ImageSaveW(Image* const image, const wchar_t* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat)
{
    wchar_t filePathW[PathMaxSize];
    wchar_t* fileExtension = 0;

    PXSize fileSize = 0;
    PXSize writtenBytes = 0;
    DataStream dataStream;  

    SerializeFromImage serializeFromImageFunction = 0;

    DataStreamConstruct(&dataStream);

    switch(fileFormat)
    {    
        case FileFormatBitMap:
        {
            fileSize = BMPFilePredictSize(image->Width, image->Height, ImageBitsPerPixel(dataFormat));
            serializeFromImageFunction = BMPSerializeFromImage;
            fileExtension = L"bmp";
            break;
        }
        case FileFormatPNG:
        {
            fileSize = PNGFilePredictSize(image->Width, image->Height, ImageBitsPerPixel(dataFormat));
            serializeFromImageFunction = PNGSerializeFromImage;
            fileExtension = L"png";
            break;
        }
        case FileFormatTGA:
        {
            fileSize = TGAFilePredictSize(image->Width, image->Height, ImageBitsPerPixel(dataFormat));
            serializeFromImageFunction = TGASerializeFromImage;
            fileExtension = L"tga";
            break;
        }
        case FileFormatJPEG:
        {
            fileSize = JPEGFilePredictSize(image->Width, image->Height, ImageBitsPerPixel(dataFormat));
            serializeFromImageFunction = JPEGSerializeFromImage;
            fileExtension = L"jpg";
            break;
        }
        case FileFormatTagImage:
        {
            fileSize = TIFFFilePredictSize(image->Width, image->Height, ImageBitsPerPixel(dataFormat));
            serializeFromImageFunction = TIFFSerializeFromImage;
            fileExtension = L"tiff";
            break;
        }
        case FileFormatGIF:
        {
            fileSize = GIFFilePredictSize(image->Width, image->Height, ImageBitsPerPixel(dataFormat));
            serializeFromImageFunction = GIFSerializeFromImage;
            fileExtension = L"gif";
            break;
        }
        default:
            return ResultFormatNotSupported;
    }


    // Chnage file extension
    {
        FilePathSwapExtensionW(filePath, filePathW, fileExtension);
    }

    {
        const ActionResult mappingResult = DataStreamMapToMemoryW(&dataStream, filePathW, fileSize, MemoryWriteOnly);
        const unsigned char sucessful = ActionSuccessful == mappingResult;

        if(!sucessful)
        {
            DataStreamDestruct(&dataStream);
            return mappingResult;
        }
    }

    {
        const ActionResult serializeResult = serializeFromImageFunction(image, dataStream.Data, dataStream.DataSize, &dataStream.DataCursor);
        const unsigned char sucessful = ActionSuccessful == serializeResult;

        if(!sucessful)
        {
            DataStreamDestruct(&dataStream);
            return serializeResult;
        }
    }

    DataStreamDestruct(&dataStream);

    return ActionSuccessful;
}

ActionResult ImageSaveD(Image* const image, void* const data, const PXSize dataSize, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat)
{
    return ActionInvalid;
}

PXBool ImageResize(Image* const image, const ImageDataFormat format, const PXSize width, const PXSize height)
{
    const PXSize bbp = ImageBytePerPixel(format);
    const PXSize newSize = width * height * bbp;
    const PXSize oldSize = image->PixelDataSize;

    // Do we need to realloc?
    {
        const PXBool isSizeAlreadyOK = newSize == oldSize;

        if (isSizeAlreadyOK)
        {
            return PXYes;
        }
    }

    // reallocate
    {
        const void* newadress = MemoryReallocate(image->PixelData, sizeof(PXByte) * newSize);

        if (!newadress)
        {
            return PXFalse;
        }

        image->Format = format;
        image->Width = width;
        image->Height = height;
        image->PixelData = newadress;
        image->PixelDataSize = newSize;
    } 

    return PXTrue;
} 

void ImageFlipHorizontal(Image* image)
{
    const PXSize bbp = ImageBytePerPixel(image->Format);
    const PXSize rowSize = (image->Width * bbp);
    const PXSize length = (image->Width * bbp) / 2;

    for(PXSize x = 0; x < length; x += bbp) // 
    {
        const PXSize xB = rowSize - x - bbp;

        for(PXSize y = 0; y < image->Height; y++)
        {
            const PXSize indexA = x + (y * rowSize);
            const PXSize indexB = xB + (y * rowSize);
            unsigned char tempByte[4] = { 0,0,0,0 };
            unsigned char* pixelA = (unsigned char*)image->PixelData + indexA;
            unsigned char* pixelB = (unsigned char*)image->PixelData + indexB;

            MemoryCopy(pixelA, 4, tempByte, bbp);
            MemoryCopy(pixelB, 4, pixelA, bbp);
            MemoryCopy(tempByte, 4, pixelB, bbp);
        }
    }
}

void ImageFlipVertical(Image* image)
{
    const PXSize bbp = ImageBytePerPixel(image->Format);;
    const PXSize scanLineWidthSize = image->Width * bbp;
    const PXSize scanLinesToSwap = image->Height / 2u;
    unsigned char* copyBufferRow = MemoryAllocate(sizeof(unsigned char) * scanLineWidthSize);

    if(!copyBufferRow)
    {
        return;
    }

    for(PXSize scanlineIndex = 0; scanlineIndex < scanLinesToSwap; scanlineIndex++)
    {
        unsigned char* bufferA = (unsigned char*)image->PixelData + (scanlineIndex * scanLineWidthSize);
        unsigned char* bufferB = (unsigned char*)image->PixelData + ((image->Height - scanlineIndex) * scanLineWidthSize) - scanLineWidthSize;

        MemoryCopy(bufferB, scanLineWidthSize, copyBufferRow, scanLineWidthSize); // A -> Buffer 'Save A'
        MemoryCopy(bufferA, scanLineWidthSize, bufferB, scanLineWidthSize); // B -> A 'Move B to A(override)'
        MemoryCopy(copyBufferRow, scanLineWidthSize, bufferA, scanLineWidthSize); // Buffer -> B 'Move SaveCopy (A) to B'
    }

    MemoryRelease(copyBufferRow, scanLineWidthSize);
}

void ImageRemoveColor(Image* image, unsigned char red, unsigned char green, unsigned char blue)
{
}

void ImageFillColorRGBA8(Image* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha)
{
    for (PXSize i = 0; i < image->PixelDataSize; i += 4u)
    {
        unsigned char* const data = (unsigned char*)image->PixelData + i;

        data[0] = red;
        data[1] = green;
        data[2] = blue;
        data[3] = alpha;
    }
}

void* ImageDataPoint(const Image* const image, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = ImageBytePerPixel(image->Format);
    const PXSize index = x * bytesPerPixel + y * image->Width;

    return (unsigned char*)image->PixelData + index;
}

PXSize ImagePixelPosition(const Image* const image, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = ImageBytePerPixel(image->Format);
    const PXSize index = x * bytesPerPixel + y * bytesPerPixel * image->Width;

    return index;
}

void ImagePixelSetRGB8
(
    Image* const image, 
    const PXSize x,
    const PXSize y,
    const unsigned char red, 
    const unsigned char green,
    const unsigned char blue
)
{
    const PXSize index = ImagePixelPosition(image, x, y);
    unsigned char* const pixelData = (unsigned char* const)image->PixelData + index;
    
    pixelData[0] = red;
    pixelData[1] = green;
    pixelData[2] = blue;
}

void ImageDrawRectangle
(
    Image* const image, 
    const PXSize x,
    const PXSize y,
    const PXSize width, 
    const PXSize height, 
    const unsigned char red, 
    const unsigned char green, 
    const unsigned char blue, 
    const unsigned char alpha
)
{
    //unsigned char* data = ImageDataPoint(image, x, y);

    const PXSize mimimumInBoundsX = MathMinimum(x + width, image->Width);
    const PXSize mimimumInBoundsY = MathMinimum(y + height, image->Height);
    const PXSize bytePerPixel = ImageBytePerPixel(image->Format);

    for (PXSize cy = y; cy < mimimumInBoundsY; ++cy)
    {
        for (PXSize cx = x; cx < mimimumInBoundsX; ++cx)
        {
            const PXSize index = cx * bytePerPixel + cy * image->Width* bytePerPixel;

            ((unsigned char*)image->PixelData)[index+0] = red;
            ((unsigned char*)image->PixelData)[index+1] = green;
            ((unsigned char*)image->PixelData)[index+2] = blue;
        }
    }      
}

void ImageDrawTextA(Image* const image, const PXSize x, const PXSize y, const PXSize width, const PXSize height, const PXFont* const font, const char* text)
{
    wchar_t textW[1024];

    TextCopyAW(text, 1024, textW, 1024);

    ImageDrawTextW(image, x, y, width, height, font, textW);
}

void ImageDrawTextW
(
    Image* const image,
    const PXSize x,
    const PXSize y,
    const PXSize width,
    const PXSize height,
    const PXFont* const font,
    const wchar_t* text
)
{
    float fontSize = 0.002;
    float lastPositionX = x;

    for (PXSize i = 0; (i < 1024u) && (text[i] != '\0'); ++i)
    {
        const wchar_t character = text[i];
        const FNTCharacter* fntCharacter = FNTGetCharacter(&font->FontElement[0], character);

        if (!fntCharacter)
        {
            // Add red box?

            continue;
        }

        float offsetX = fntCharacter->Offset[0];
        float offsetY = fntCharacter->Offset[1];
        float positionX = fntCharacter->Position[0];
        float positionY = fntCharacter->Position[1];
        float sizeX = fntCharacter->Size[0];
        float sizeY = fntCharacter->Size[1];
             
#if 0
        ImageDrawRectangle
        (
            image, 
            lastPositionX+ offsetX, 
            y+ offsetY,
            sizeX,
            sizeY, 
            0xFF,
            0,
            0xFF, 
            0
        );
#endif
        ImageMerge
        (
            image,
            lastPositionX + offsetX, 
            y + offsetY, 
            positionX,
            positionY,
            sizeX,
            sizeY,       
            &font->FontElement[0].FontPageList[0].FontTextureMap
        );

        lastPositionX += fntCharacter->XAdvance;
        //lastPositionX += 15;
    }
}

void ImageMerge
(
    Image* const image,
    const PXSize x,
    const PXSize y,
    const PXSize insertX,
    const PXSize insertY,
    const PXSize insertWidth,
    const PXSize insertHeight,
    const Image* const imageInsert
)
{

    //unsigned char* data = ImageDataPoint(image, x, y);

    const PXSize sourceMimimumInBoundsX = MathMinimum(x + insertWidth, image->Width);
    const PXSize sourceMimimumInBoundsY = MathMinimum(y + insertHeight, image->Height);
    const PXSize targetMimimumInBoundsX = MathMinimum(insertX + insertWidth, imageInsert->Width);
    const PXSize targetMimimumInBoundsY = MathMinimum(insertY + insertHeight, imageInsert->Height);

    const PXSize sourceBytesPerPixel = ImageBytePerPixel(image->Format);
    const PXSize targetBytesPerPixel = ImageBytePerPixel(imageInsert->Format);
    const PXSize bytesPerPixel = MathMinimum(sourceBytesPerPixel, targetBytesPerPixel);

    for (PXSize cy = 0u; cy < insertHeight; ++cy)
    {
        for (PXSize cx = 0u; cx < insertWidth; ++cx)
        {
            const PXSize indexSource = (cx + x) * sourceBytesPerPixel + (cy + y) * image->Width * sourceBytesPerPixel;
            const PXSize indexTarget = (cx + insertX) * targetBytesPerPixel + (cy + insertY) * imageInsert->Width * targetBytesPerPixel;

            unsigned char* source = ((unsigned char*)image->PixelData) + indexSource;
            unsigned char* target = ((unsigned char*)imageInsert->PixelData) + indexTarget;

            if(targetBytesPerPixel == 4)
            {
                if(target[3] == 0)
                {
                    continue;
                }
            }

            for(PXSize i = 0; i < bytesPerPixel; ++i)
            {
                source[i] = target[i];
            }
        }
    }
}