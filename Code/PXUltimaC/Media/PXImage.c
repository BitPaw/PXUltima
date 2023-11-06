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

void PXImageConstruct(PXImage* const image)
{
    PXMemoryClear(image, sizeof(PXImage));
}

void PXImageDestruct(PXImage* const image)
{
    PXDeleteList(PXByte, image->PixelData, image->PixelDataSize);

    PXImageConstruct(image);
}

void PXImageCopyAsIs(PXImage* const pxImageTarget, const PXImage* const pxImageSource)
{
    PXMemoryCopy(pxImageSource, sizeof(PXImage), pxImageTarget, sizeof(PXImage));
}

void PXImageCopyAsNew(PXImage* const pxImageTarget, const PXImage* const pxImageSource)
{
    
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
        const void* newadress = PXResizeList(PXByte, &image->PixelData, &image->PixelDataSize, newSize);

        if (!newadress)
        {
            return PXFalse;
        }

        image->Format = dataFormat;
        image->Width = width;
        image->Height = height;
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
    PXByte* copyBufferRow = PXNewList(PXByte, scanLineWidthSize);

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

    PXDeleteList(PXByte, copyBufferRow, scanLineWidthSize);
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