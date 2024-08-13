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

void PXAPI PXImageConstruct(PXImage* const image)
{
    PXClear(PXImage, image);
}

void PXAPI PXImageDestruct(PXImage* const image)
{
    PXDeleteList(PXByte, image->PixelDataSize, &image->PixelData, &image->PixelDataSize);

    PXImageConstruct(image);
}

void PXAPI PXImageCopyAsIs(PXImage* const pxImageTarget, const PXImage* const pxImageSource)
{
    PXCopy(PXImage, pxImageSource, pxImageTarget);
}

void PXAPI PXImageCopyAsNew(PXImage* const pxImageTarget, const PXImage* const pxImageSource)
{
    PXNewList(PXInt8U, pxImageSource->PixelDataSize, &pxImageTarget->PixelData, &pxImageTarget->PixelDataSize);
    PXCopyList(PXInt8U, pxImageSource->PixelDataSize, pxImageSource->PixelData, pxImageTarget->PixelData);

    pxImageTarget->Width = pxImageSource->Width;
    pxImageTarget->Height = pxImageSource->Height;
    pxImageTarget->Depth = pxImageSource->Depth;
    pxImageTarget->Format = pxImageSource->Format;
}

PXBool PXAPI PXImageResize(PXImage* const image, const PXColorFormat dataFormat, const PXSize width, const PXSize height)
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
        image->PixelDataSize = newSize;
        image->PixelData = PXMemoryMallocT(PXByte, newSize);
        image->Format = dataFormat;
        image->Width = width;
        image->Height = height;

        /*

        // Set image data, so we dont have a random fully empty image with no alpha
        // Causes wierd problems if you dont expect that.
        if(pxMemoryHeapReallocateEventData.WasSizeIncreased)
        {
            switch (bbp)
            {
                case 3:
                {
                    for (PXSize i = 0; i < pxMemoryHeapReallocateEventData.PointOfNewDataSize; i += 3)
                    {
                        char* data = &((char*)pxMemoryHeapReallocateEventData.PointOfNewData)[i];

                        data[0] = 0xFF;
                        data[1] = 0xFF;
                        data[2] = 0xFF;
                    }
                    break;
                }

                case 4:
                {
                    for (PXSize i = 0; i < pxMemoryHeapReallocateEventData.PointOfNewDataSize; i += 4)
                    {
                        char* data = &((char*)pxMemoryHeapReallocateEventData.PointOfNewData)[i];

                        data[0] = 0xFF;
                        data[1] = 0xFF;
                        data[2] = 0xFF;
                        data[3] = 0xFF;
                    }
                    break;
                }
            }      
        }
        */

    }

    return PXTrue;
}

void PXAPI PXImageFlipHorizontal(PXImage* image)
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

void PXAPI PXImageFlipVertical(PXImage* image)
{
    const PXSize bbp = PXColorFormatBytePerPixel(image->Format);;
    const PXSize scanLineWidthSize = image->Width * bbp;
    const PXSize scanLinesToSwap = image->Height / 2u;
    PXByte* copyBufferRow = PXNull;
    
    PXNewList(PXByte, scanLineWidthSize, &copyBufferRow, PXNull);

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

    PXDeleteList(PXByte, scanLineWidthSize, &copyBufferRow, PXNull);
}

void PXAPI PXImageRemoveColor(PXImage* image, unsigned char red, unsigned char green, unsigned char blue)
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

void PXAPI PXImageFillColorRGBA8(PXImage* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha)
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

void* PXAPI PXImageDataPoint(const PXImage* const image, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(image->Format);
    const PXSize index = x * bytesPerPixel + y * image->Width;

    return (unsigned char*)image->PixelData + index;
}

PXSize PXAPI PXImagePixelPosition(const PXImage* const image, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBitsPerPixel(image->Format);
    const PXSize index = x * bytesPerPixel + y * bytesPerPixel * image->Width;

    return index;
}

void PXAPI PXImagePixelSetRGB8
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