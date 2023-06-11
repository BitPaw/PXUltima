#include "PXImage.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>

#include <Media/Bitmap/PXBitmap.h>
#include <Media/GIF/PXGIF.h>
#include <Media/JPEG/PXJPEG.h>
#include <Media/PNG/PXPNG.h>
#include <Media/TGA/PXTGA.h>
#include <Media/TIFF/PXTIFF.h>
#include <Media/SpriteFont/PXSpriteFont.h>
#include <Media/PXFont.h>

void PXImageConstruct(PXImage* const image)
{
    PXMemoryClear(image, sizeof(PXImage));
}

void PXImageDestruct(PXImage* const image)
{
    PXMemoryRelease(image->PixelData, image->PixelDataSize);

    PXImageConstruct(image);
}

PXActionResult PXImageLoad(PXImage* const image, const PXText* const filePath)
{
    PXFile dataStream;

    PXImageConstruct(image);

    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.Text = *filePath;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        const PXActionResult fileLoadingResult = PXFileOpenFromPath(&dataStream, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(fileLoadingResult);
    }

    //dataStream.FilePath = filePath;

    {
        {
            const FileFormatExtension imageForPXMathint = PXFilePathExtensionDetectTry(filePath);
            const PXActionResult fileParsingResult = PXImageLoadD(image, &dataStream, imageForPXMathint);

            PXActionReturnOnSuccess(fileParsingResult);
        }



        PXActionResult fileGuessResult = PXActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = PXImageLoadD(image, &dataStream, imageFileFormat);

            fileFormatID++;
        }
        while(fileGuessResult == PXActionRefusedInvalidHeaderSignature);

        PXFileDestruct(&dataStream);

        return fileGuessResult;
    }
}

PXActionResult PXImageLoadD(PXImage* const image, PXFile* const dataStream, const FileFormatExtension guessedFormat)
{
    PXSize bytesRead = 0;
    PXImageTranslateFunction parseToImage = 0;

    switch(guessedFormat)
    {
        case FileFormatBitMap:
        {
            parseToImage = PXBitmapParseToImage;
            break;
        }
        case FileFormatGIF:
        {
            parseToImage = PXGIFParseToImage;
            break;
        }
        case FileFormatJPEG:
        {
            parseToImage = PXJPEGParseToImage;
            break;
        }
        case FileFormatPNG:
        {
            parseToImage = PXPNGParseToImage;
            break;
        }
        case FileFormatTGA:
        {
            parseToImage = PXTGAParseToImage;
            break;
        }
        case FileFormatTagImage:
        {
            parseToImage = PXTIFFParseToImage;
            break;
        }
        default:
        {
            return PXActionRefusedFormatNotSupported;
        }
    }

    const PXActionResult actionResult = parseToImage(image, dataStream);

    return actionResult;
}

PXActionResult PXImageSave(PXImage* const image, const PXText* const filePath, const FileFormatExtension fileFormat, const PXColorFormat pxColorFormat)
{
    PXByte filePathW[PathMaxSize];
    const char* fileExtension = 0;

    PXSize fileSize = 0;
    PXSize writtenBytes = 0;
    PXFile dataStream;

    PXImageTranslateFunction serializeFromImageFunction = 0;

    const PXInt8U bitPerPixel = PXColorFormatBitsPerPixel(pxColorFormat);

    switch (fileFormat)
    {
        case FileFormatBitMap:
        {
            fileSize = PXBitmapFilePredictSize(image->Width, image->Height, bitPerPixel);
            serializeFromImageFunction = PXBitmapSerializeFromImage;
            fileExtension = "bmp";
            break;
        }
        case FileFormatPNG:
        {
            fileSize = PXPNGFilePredictSize(image->Width, image->Height, bitPerPixel);
            serializeFromImageFunction = PXPNGSerializeFromImage;
            fileExtension = "png";
            break;
        }
        case FileFormatTGA:
        {
            fileSize = PXTGAFilePredictSize(image->Width, image->Height, bitPerPixel);
            serializeFromImageFunction = PXTGASerializeFromImage;
            fileExtension = "tga";
            break;
        }
        case FileFormatJPEG:
        {
            fileSize = PXJPEGFilePredictSize(image->Width, image->Height, bitPerPixel);
            serializeFromImageFunction = PXJPEGSerializeFromImage;
            fileExtension = "jpg";
            break;
        }
        case FileFormatTagImage:
        {
            fileSize = PXTIFFFilePredictSize(image->Width, image->Height, bitPerPixel);
            serializeFromImageFunction = PXTIFFSerializeFromImage;
            fileExtension = "tiff";
            break;
        }
        case FileFormatGIF:
        {
            fileSize = PXGIFFilePredictSize(image->Width, image->Height, bitPerPixel);
            serializeFromImageFunction = PXGIFSerializeFromImage;
            fileExtension = "gif";
            break;
        }
        default:
            return PXActionRefusedFormatNotSupported;
    }


    // Chnage file extension
    {
        //PXFilePathSwapExtension(filePath, filePathW, fileExtension);
    }

    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.Text = *filePath;
        pxFileOpenFromPathInfo.FileSize = fileSize;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeWriteOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXTrue;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXTrue;

        const PXActionResult mappingResult = PXFileOpenFromPath(&dataStream, &pxFileOpenFromPathInfo);
        const PXBool sucessful = PXActionSuccessful == mappingResult;

        if (!sucessful)
        {
            PXFileDestruct(&dataStream);
            return mappingResult;
        }
    }

    {
        const PXActionResult serializeResult = serializeFromImageFunction(image, &dataStream);
        const PXBool sucessful = PXActionSuccessful == serializeResult;

        if (!sucessful)
        {
            PXFileDestruct(&dataStream);
            return serializeResult;
        }
    }

    PXFileDestruct(&dataStream);

    return PXActionSuccessful;
}

PXActionResult PXImageSaveD(PXImage* const image, PXFile* const PXFile, const FileFormatExtension fileFormat, const PXColorFormat dataFormat)
{
    return PXActionInvalid;
}

PXBool PXImageResize(PXImage* const image, const PXColorFormat dataFormat, const PXSize width, const PXSize height)
{
    const PXSize bbp = PXColorFormatBytePerPixel(dataFormat);
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
        const void* newadress = PXMemoryReallocate(image->PixelData, sizeof(PXByte) * newSize);

        if (!newadress)
        {
            return PXFalse;
        }

        image->Format = dataFormat;
        image->Width = width;
        image->Height = height;
        image->PixelData = newadress;
        image->PixelDataSize = newSize;
    }

    return PXTrue;
}

void PXImageFlipHorizontal(PXImage* image)
{
    const PXSize bbp = PXColorFormatBytePerPixel(image->Format);
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

            PXMemoryCopy(pixelA, 4, tempByte, bbp);
            PXMemoryCopy(pixelB, 4, pixelA, bbp);
            PXMemoryCopy(tempByte, 4, pixelB, bbp);
        }
    }
}

void PXImageFlipVertical(PXImage* image)
{
    const PXSize bbp = PXColorFormatBytePerPixel(image->Format);;
    const PXSize scanLineWidthSize = image->Width * bbp;
    const PXSize scanLinesToSwap = image->Height / 2u;
    PXByte* copyBufferRow = PXMemoryAllocateType(PXByte, scanLineWidthSize);

    if(!copyBufferRow)
    {
        return;
    }

    for(PXSize scanlineIndex = 0; scanlineIndex < scanLinesToSwap; scanlineIndex++)
    {
        PXByte* bufferA = (PXByte*)image->PixelData + (scanlineIndex * scanLineWidthSize);
        PXByte* bufferB = (PXByte*)image->PixelData + ((image->Height - scanlineIndex) * scanLineWidthSize) - scanLineWidthSize;

        PXMemoryCopy(bufferB, scanLineWidthSize, copyBufferRow, scanLineWidthSize); // A -> Buffer 'Save A'
        PXMemoryCopy(bufferA, scanLineWidthSize, bufferB, scanLineWidthSize); // B -> A 'Move B to A(override)'
        PXMemoryCopy(copyBufferRow, scanLineWidthSize, bufferA, scanLineWidthSize); // Buffer -> B 'Move SaveCopy (A) to B'
    }

    PXMemoryRelease(copyBufferRow, scanLineWidthSize);
}

void PXImageRemoveColor(PXImage* image, unsigned char red, unsigned char green, unsigned char blue)
{
    // DO we have alpha? If not, make one

    const PXSize currentOffset = image->PixelDataSize;
    PXSize dataOffset = 0;

    PXImageResize(image, PXColorFormatRGBAI8, image->Width, image->Height);

    dataOffset = image->PixelDataSize;

    const PXSize length = image->Width * image->Height;

    for (PXSize pixelIndex = 0; pixelIndex < length; ++pixelIndex)
    {
        PXByte* const oldData = (PXByte*)image->PixelData + currentOffset - ((pixelIndex + 1) * 3u);
        PXByte* const newData = (PXByte*)image->PixelData + dataOffset - ((pixelIndex + 1) * 4u);

        newData[0] = oldData[0];
        newData[1] = oldData[1];
        newData[2] = oldData[2];
        newData[3] = !(oldData[0] == red) && (oldData[1] == green) && (oldData[2] == blue);
    }
}

void PXImageFillColorRGBA8(PXImage* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha)
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

void* PXImageDataPoint(const PXImage* const image, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(image->Format);
    const PXSize index = x * bytesPerPixel + y * image->Width;

    return (unsigned char*)image->PixelData + index;
}

PXSize PXImagePixelPosition(const PXImage* const image, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBitsPerPixel(image->Format);
    const PXSize index = x * bytesPerPixel + y * bytesPerPixel * image->Width;

    return index;
}

void PXImagePixelSetRGB8
(
    PXImage* const image,
    const PXSize x,
    const PXSize y,
    const unsigned char red,
    const unsigned char green,
    const unsigned char blue
)
{
    const PXSize index = PXImagePixelPosition(image, x, y);
    unsigned char* const pixelData = (unsigned char* const)image->PixelData + index;

    pixelData[0] = red;
    pixelData[1] = green;
    pixelData[2] = blue;
}