#include "PXResourceManager.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

PXResourceManager _pxResourceManager;

const char PXResourceText[] = "Resource";



const PXF32 PXVertexDataTriangle[] =
{
    -1.0f, -1.0f,
        1.0f, -1.0f,
        0.5f,  1.0f
};

const PXI8S PXIndexDataTriangle[] =
{
    0,1,2
};

// This only works if we dont use any texturepositions
const PXF32 PXVertexDataRectangle[] =
{
    -1, -1,
     1, -1,
     1,  1,
    -1,  1
};

const PXF32 PXVertexDataRectangleTX[] =
{
    -1, -1, 1, 1, // Bottom-left
     1, -1, 0, 1, // Bottom-right
     1,  1, 0, 0,  // Top-right
    -1,  1, 1, 0   // Top-left
};

const PXI8U PXIndexDataRectangle[] =
{
    0,1,2,
    2,3,0
};

const PXF32 PXVertexDataCube[] = // PXI8S
{
    -1, -1, -1,
        1, -1, -1,
        -1,  1, -1,
        1,  1, -1,
        -1, -1,  1,
        1, -1,  1,
        -1,  1,  1,
        1,  1,  1,
};

const PXI8U PXIndexDataCube[] =
{
#if 0
    0,1,2,3, // Left
    0,2,4,6, // Back
    2,3,7,6, // Top
    4,5,6,7, // Right
    0,1,4,5, // Bottom
    1,3,5,7  // Font
#else

    // Left
    0,1,2,
    2,3,1,

    // Right
    4,5,6,
    6,7,5,

    // Back
    0,2,4,
    4,6,2,

    // Font
    1,3,5,
    5,7,3,

    // Bottom
    0,1,4,
    4,5,1,

    // Top
    2,3,7,
    7,6,2

#endif
};













PXI32U PXAPI PXResourceManagerGenerateUniqeID()
{
    // assert(pxResourceManager);

    return 1000 + ++_pxResourceManager.UniqeIDCounter;
}

PXResult PXAPI PXResourceAdd(PXResourceInfo* const pxResourceInfo, void* payload)
{
    return PXActionSuccessful;
}

PXResult PXAPI PXResourceRemove(PXResourceInfo* const pxResourceInfo)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourcePropertyE(PXResourceProperty* const pxResourceProperty, const PXBool doWrite)
{
    // NULL Check
    if(!pxResourceProperty)
    {
        return PXActionRefusedArgumentNull;
    }

    if(doWrite)
    {
        const PXBool callValid = 0;// name && !(0 == nameSize || (PXSize)-1 == nameSize);

        if(!callValid)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceText,
                "Store-Name",
                "PXID:%i, Name can't be stored!",
                0//pxResourceInfo->ID
            );
#endif

            return PXActionRefusedArgumentInvalid;
        }


        PXSize length = 0;// nameSize;

        //if(-1 == nameSize)
        {
            length = 0;// PXTextLengthA(name, nameSize);
        }

#if PXLogEnable && 0
        char buffer[256];

      //  PXTextCopyA(name, nameSize, buffer, length);

        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceText,
            "Store-Name",
            "PXID:%i, %s (%i)",
            0//pxResourceInfo->ID,
            buffer,
            length
        );
#endif

        //PXListDynamicAdd(&_pxResourceManager.NameCache, &pxResourceInfo->ID, name, length);

       // pxResourceInfo->Behaviour |= PXResourceInfoHasName;

        return PXActionSuccessful;
    }

    return PXActionInvalid;
}







PXResult PXAPI PXGUIIconLoad(PXIcon* const pxIcon)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    pxIcon->Info.Handle.IconHandle = LoadIcon(NULL, IDI_EXCLAMATION);
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXRectangleLTRBI32ToXYWHI32(const PXRectangleLTRBI32* const pxRectangleLTRBI32, PXRectangleXYWHI32* const pxRectangleXYWHI32)
{
    pxRectangleXYWHI32->X = pxRectangleLTRBI32->Left;
    pxRectangleXYWHI32->Y = pxRectangleLTRBI32->Top;
    pxRectangleXYWHI32->Width = pxRectangleLTRBI32->Right - pxRectangleLTRBI32->Left;
    pxRectangleXYWHI32->Height = pxRectangleLTRBI32->Bottom - pxRectangleLTRBI32->Top;
}

void PXAPI PXRectangleXYWHI32ToLTRBI32(const PXRectangleXYWHI32* const pxRectangleXYWHI32, PXRectangleLTRBI32* const pxRectangleLTRBI32)
{
    pxRectangleLTRBI32->Left = pxRectangleXYWHI32->X;
    pxRectangleLTRBI32->Top = pxRectangleXYWHI32->Y;
    pxRectangleLTRBI32->Right = pxRectangleXYWHI32->X + pxRectangleXYWHI32->Width;
    pxRectangleLTRBI32->Bottom = pxRectangleXYWHI32->Y + pxRectangleXYWHI32->Height;
}

PXResult PXAPI PXTextureRelease(PXTexture* const pxTexture)
{
    PXMemoryHeapFree(PXNull, pxTexture->PixelData);

    return PXActionInvalid;
}

PXResult PXAPI PXTextureCopyAsIs(PXTexture* const pxTexture, const PXTexture* const pxTextureSource)
{
  //  PXCopy(PXImage, pxTexture, pxTextureSource);

    return PXActionInvalid;
}

PXResult PXAPI PXTextureCopyAsNew(PXTexture* const pxTexture, const PXTexture* const pxTextureSource)
{
    pxTexture->PixelData = PXMemoryHeapCallocT(PXByte, pxTextureSource->PixelDataSize);
    PXCopyList(PXByte, pxTexture->PixelDataSize, pxTexture->PixelData, pxTextureSource->PixelData);

    pxTexture->Width = pxTextureSource->Width;
    pxTexture->Height = pxTextureSource->Height;
    pxTexture->Depth = pxTextureSource->Depth;
    pxTexture->Format = pxTextureSource->Format;

    return PXActionInvalid;
}

PXResult PXAPI PXTextureResize(PXTexture* const pxTexture, const PXColorFormat format, const PXSize width, const PXSize height)
{
    const PXSize bbp = PXColorFormatBytePerPixel(format);
    const PXSize newSize = width * height * bbp;
    const PXSize oldSize = pxTexture->PixelDataSize;

    // Do we need to realloc?
    {
        const PXBool isSizeAlreadyOK = newSize == oldSize;

        if(isSizeAlreadyOK)
        {
            return PXYes;
        }
    }

    // reallocate
    {
        pxTexture->PixelDataSize = newSize;
        pxTexture->PixelData = PXMemoryHeapCallocT(PXByte, newSize);
        pxTexture->Format = format;
        pxTexture->Width = width;
        pxTexture->Height = height;

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

    return PXActionSuccessful;
}

PXResult PXAPI PXTextureFlipHorizontal(PXTexture* const pxTexture)
{
    const PXSize bbp = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize rowSize = (pxTexture->Width * bbp);
    const PXSize length = (pxTexture->Width * bbp) / 2;

    for(PXSize x = 0; x < length; x += bbp) //
    {
        const PXSize xB = rowSize - x - bbp;

        for(PXSize y = 0; y < pxTexture->Height; y++)
        {
            const PXSize indexA = x + (y * rowSize);
            const PXSize indexB = xB + (y * rowSize);
            PXByte tempByte[4] = { 0,0,0,0 };
            PXByte* pixelA = (PXByte*)pxTexture->PixelData + indexA;
            PXByte* pixelB = (PXByte*)pxTexture->PixelData + indexB;

            PXMemoryCopy(pixelA, 4, tempByte, bbp);
            PXMemoryCopy(pixelB, 4, pixelA, bbp);
            PXMemoryCopy(tempByte, 4, pixelB, bbp);
        }
    }

    return PXActionInvalid;
}

PXResult PXAPI PXTextureFlipVertical(PXTexture* const pxTexture)
{
    const PXSize bbp = PXColorFormatBytePerPixel(pxTexture->Format);;
    const PXSize scanLineWidthSize = pxTexture->Width * bbp;
    const PXSize scanLinesToSwap = pxTexture->Height / 2u;
    PXByte* copyBufferRow = PXMemoryHeapCallocT(PXByte, scanLineWidthSize);

    if(!copyBufferRow)
    {
        return;
    }

    for(PXSize scanlineIndex = 0; scanlineIndex < scanLinesToSwap; scanlineIndex++)
    {
        PXByte* bufferA = (PXByte*)pxTexture->PixelData + (scanlineIndex * scanLineWidthSize);
        PXByte* bufferB = (PXByte*)pxTexture->PixelData + ((pxTexture->Height - scanlineIndex) * scanLineWidthSize) - scanLineWidthSize;

        PXMemoryCopy(bufferB, scanLineWidthSize, copyBufferRow, scanLineWidthSize); // A -> Buffer 'Save A'
        PXMemoryCopy(bufferA, scanLineWidthSize, bufferB, scanLineWidthSize); // B -> A 'Move B to A(override)'
        PXMemoryCopy(copyBufferRow, scanLineWidthSize, bufferA, scanLineWidthSize); // Buffer -> B 'Move SaveCopy (A) to B'
    }

    PXMemoryHeapFree(PXNull, copyBufferRow);

    return PXActionInvalid;
}

PXResult PXAPI PXTextureRemoveColor(PXTexture* const pxTexture, const PXByte red, const PXByte green, const PXByte blue)
{
    // DO we have alpha? If not, make one

    const PXSize currentOffset = pxTexture->PixelDataSize;
    PXSize dataOffset = 0;

    PXImageResize(pxTexture, PXColorFormatRGBAI8, pxTexture->Width, pxTexture->Height);

    dataOffset = pxTexture->PixelDataSize;

    const PXSize length = pxTexture->Width * pxTexture->Height;

    for(PXSize pixelIndex = 0; pixelIndex < length; ++pixelIndex)
    {
        PXByte* const oldData = (PXByte*)pxTexture->PixelData + currentOffset - ((pixelIndex + 1) * 3u);
        PXByte* const newData = (PXByte*)pxTexture->PixelData + dataOffset - ((pixelIndex + 1) * 4u);

        newData[0] = oldData[0];
        newData[1] = oldData[1];
        newData[2] = oldData[2];
        newData[3] = !(oldData[0] == red) && (oldData[1] == green) && (oldData[2] == blue);
    }

    return PXActionInvalid;
}

PXResult PXAPI PXTextureFillColorRGBA8(PXTexture* const pxTexture, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha)
{
    for(PXSize i = 0; i < pxTexture->PixelDataSize; i += 4u)
    {
        PXByte* const data = (PXByte*)pxTexture->PixelData + i;

        data[0] = red;
        data[1] = green;
        data[2] = blue;
        data[3] = alpha;
    }

    return PXActionInvalid;
}

void* PXAPI PXTextureDataPoint(const PXTexture* const pxTexture, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize index = x * bytesPerPixel + y * pxTexture->Width;

    return (PXByte*)pxTexture->PixelData + index;
}

PXSize PXAPI PXTexturePixelPosition(const PXTexture* const pxTexture, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize index = x * bytesPerPixel + y * bytesPerPixel * pxTexture->Width;

    return index;
}

void PXAPI PXTexturePixelSetRGB8(PXTexture* const pxTexture, const PXSize x, const PXSize y, const PXByte red, const PXByte green, const PXByte blue)
{
    const PXSize index = PXTexturePixelPosition(pxTexture, x, y);
    PXByte* pixelData = (PXByte*)pxTexture->PixelData + index;

    pixelData[0] = red;
    pixelData[1] = green;
    pixelData[2] = blue;
}
