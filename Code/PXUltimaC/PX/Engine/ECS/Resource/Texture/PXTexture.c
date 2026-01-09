#include "PXTexture.h"
#include <PX/Engine/PXResource.h>
#include <PX/OS/PXOS.h>

#include <PX/OS/Console/PXConsole.h>

const char PXTextureText[] = "Texture";

PXResult PXAPI PXTextureProperty(PXTexture PXREF pxTexture, PXECSProperty PXREF pxECSProperty)
{
    switch(pxECSProperty->PropertyID)
    {
        case PXTexturePropertyWidth:
        {
            if(pxECSProperty->DoWrite)
            {
                pxTexture->Width = pxECSProperty->Size;
            }
            else
            {
                pxECSProperty->Size = pxTexture->Width;
            }

            break;
        }
        case PXTexturePropertyHeight:
        {
            if(pxECSProperty->DoWrite)
            {
                pxTexture->Height = pxECSProperty->Size;
            }
            else
            {
                pxECSProperty->Size = pxTexture->Height;
            }

            break;
        }
        case PXTexturePropertyDepth:
        {
            if(pxECSProperty->DoWrite)
            {
                pxTexture->Height = pxECSProperty->Size;
            }
            else
            {
                pxECSProperty->Size = pxTexture->Depth;
            }

            break;
        }
        case PXTexturePropertyFormat:
        {
            if(pxECSProperty->DoWrite)
            {
                pxTexture->Format = pxECSProperty->Size;
            }
            else
            {
                pxECSProperty->Size = pxTexture->Depth;
            }

            break;
        }
        case PXTexturePropertyBBP:
        {
            if(pxECSProperty->DoWrite)
            {
               
            }
            else
            {
                pxECSProperty->Size = PXColorFormatBytePerPixel(pxTexture->Format);
            }

            break;
        }

        default:
            break;
    }

    return PXResultOK;
}

PXSize PXAPI PXTextureHeight(PXTexture PXREF pxTexture)
{
    return pxTexture->Height;
}

PXSize PXAPI PXTextureWidth(PXTexture PXREF pxTexture)
{
    return pxTexture->Width;
}

PXSize PXAPI PXTextureDepth(PXTexture PXREF pxTexture)
{
    return pxTexture->Depth;
}

PXColorFormat PXAPI PXTextureColorFormat(PXTexture PXREF pxTexture)
{
    return pxTexture->Format;
}

void* PXAPI PXTextureDXInterface(PXTexture PXREF pxTexture)
{
    return pxTexture->DirectXHandle;
}

PXBuffer* PXAPI PXTexturePixelData(PXTexture PXREF pxTexture)
{
    return &pxTexture->PixelData;
}

PXResult PXAPI PXTextureRelease(PXTexture PXREF pxTexture)
{
    PXBufferRelese(&pxTexture->PixelData);

    return PXResultInvalid;
}

PXResult PXAPI PXTextureCopyAsIs(PXTexture PXREF pxTexture, const PXTexture PXREF pxTextureSource)
{
    //  PXCopy(PXTexture, pxTexture, pxTextureSource);

    return PXResultInvalid;
}

PXResult PXAPI PXTextureCopyAsNew(PXTexture PXREF pxTexture, const PXTexture PXREF pxTextureSource)
{
    PXBufferAllocate(&pxTexture->PixelData, pxTextureSource->PixelData.CursorOffsetByte);
    PXBufferCopy(&pxTexture->PixelData, pxTextureSource->PixelData.Adress, pxTextureSource->PixelData.CursorOffsetByte);

    pxTexture->Width = pxTextureSource->Width;
    pxTexture->Height = pxTextureSource->Height;
    pxTexture->Depth = pxTextureSource->Depth;
    pxTexture->Format = pxTextureSource->Format;

    return PXResultInvalid;
}

PXResult PXAPI PXTextureResize(PXTexture PXREF pxTexture, const PXColorFormat format, const PXSize width, const PXSize height)
{
    const PXSize bbp = PXColorFormatBytePerPixel(format);
    const PXSize newSize = width * height * bbp;
    const PXSize oldSize = 0;// pxTexture->PixelDataSize;

    // Do we need to realloc?
    {
        const PXBool isSizeAlreadyOK = newSize == oldSize;

        if(isSizeAlreadyOK)
        {
            return PXResultOK;
        }
    }

    // reallocate
    {
        PXBufferAllocate(&pxTexture->PixelData, newSize);
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

    return PXResultOK;
}

PXResult PXAPI PXTextureFlipHorizontal(PXTexture PXREF pxTexture)
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
            PXByte* pixelA = pxTexture->PixelData.Data + indexA;
            PXByte* pixelB = pxTexture->PixelData.Data + indexB;

            PXMemoryCopyX(pixelA, 4, tempByte, bbp);
            PXMemoryCopyX(pixelB, 4, pixelA, bbp);
            PXMemoryCopyX(tempByte, 4, pixelB, bbp);
        }
    }

    return PXResultInvalid;
}

PXResult PXAPI PXTextureFlipVertical(PXTexture PXREF pxTexture)
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
        PXByte* bufferA = pxTexture->PixelData.Data + (scanlineIndex * scanLineWidthSize);
        PXByte* bufferB = pxTexture->PixelData.Data + ((pxTexture->Height - scanlineIndex) * scanLineWidthSize) - scanLineWidthSize;

        PXMemoryCopy(bufferB, copyBufferRow, scanLineWidthSize); // A -> Buffer 'Save A'
        PXMemoryCopy(bufferA, bufferB, scanLineWidthSize); // B -> A 'Move B to A(override)'
        PXMemoryCopy(copyBufferRow, bufferA, scanLineWidthSize); // Buffer -> B 'Move SaveCopy (A) to B'
    }

    PXMemoryHeapFree(PXNull, copyBufferRow);

    return PXResultInvalid;
}

PXResult PXAPI PXTextureRemoveColor(PXTexture PXREF pxTexture, const PXByte red, const PXByte green, const PXByte blue)
{
    // DO we have alpha? If not, make one

    const PXSize currentOffset = pxTexture->PixelData.CursorOffsetByte;
    PXSize dataOffset = 0;

    PXTextureResize(pxTexture, PXColorFormatRGBAI8, pxTexture->Width, pxTexture->Height);

    dataOffset = pxTexture->PixelData.CursorOffsetByte;

    const PXSize length = pxTexture->Width * pxTexture->Height;

    for(PXSize pixelIndex = 0; pixelIndex < length; ++pixelIndex)
    {
        PXByte PXREF oldData = pxTexture->PixelData.Data + currentOffset - ((pixelIndex + 1) * 3u);
        PXByte PXREF newData = pxTexture->PixelData.Data + dataOffset - ((pixelIndex + 1) * 4u);

        newData[0] = oldData[0];
        newData[1] = oldData[1];
        newData[2] = oldData[2];
        newData[3] = !(oldData[0] == red) && (oldData[1] == green) && (oldData[2] == blue);
    }

    return PXResultInvalid;
}

PXResult PXAPI PXTextureFillColorRGBA8(PXTexture PXREF pxTexture, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha)
{
    for(PXSize i = 0; i < pxTexture->PixelData.CursorOffsetByte; i += 4u)
    {
        PXByte PXREF data = pxTexture->PixelData.Data + i;

        data[0] = red;
        data[1] = green;
        data[2] = blue;
        data[3] = alpha;
    }

    return PXResultInvalid;
}

void* PXAPI PXTextureDataPoint(const PXTexture PXREF pxTexture, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize index = x * bytesPerPixel + y * pxTexture->Width;

    return (PXByte*)pxTexture->PixelData.Data + index;
}

PXSize PXAPI PXTexturePixelPosition(const PXTexture PXREF pxTexture, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize index = x * bytesPerPixel + y * bytesPerPixel * pxTexture->Width;

    return index;
}

void PXAPI PXTexturePixelSetRGB8(PXTexture PXREF pxTexture, const PXSize x, const PXSize y, const PXByte red, const PXByte green, const PXByte blue)
{
    const PXSize index = PXTexturePixelPosition(pxTexture, x, y);
    PXByte* pixelData = (PXByte*)pxTexture->PixelData.Data + index;

    pixelData[0] = red;
    pixelData[1] = green;
    pixelData[2] = blue;
}

PXResult PXAPI PXTextureCreate(PXTexture PXREF pxTexture, PXTextureCreateInfo PXREF pxTextureCreateInfo)
{
    if(!(pxTexture && pxTextureCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxTexture->Filter = PXRenderFilterNoFilter;
    pxTexture->LayoutNear = PXTextureLayoutNearest;
    pxTexture->LayoutFar = PXTextureLayoutNearest;
    pxTexture->WrapHeight = PXTextureWrapRepeat;
    pxTexture->WrapWidth = PXTextureWrapRepeat;


    switch(pxTextureCreateInfo->Type)
    {
        case PXTextureType2D:
        {
            // Load texture
            if(pxTextureCreateInfo->Info.FilePath.Data)
            {
                PXResourceMoveInfo pxResourceLoadInfo;
                PXClear(PXResourceMoveInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.ResourceTarget = pxTexture;
                pxResourceLoadInfo.ResourceType = PXResourceTypeTexture;

                const PXResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxTextureCreateInfo->Info.FilePath);


                if(PXResultOK != loadResult)
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingError,
                        PXTextureText,
                        "Create",
                        "Failed <%s>!",
                        pxTextureCreateInfo->Info.FilePath.A
                    );
#endif

                    return loadResult;
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXTextureText,
                    "Create",
                    "Successful <%s>.",
                    pxTextureCreateInfo->Info.FilePath.A
                );
#endif
            }
            else
            {
                // PXTextureCopyAsNew(PXTexture, &PXTextureCreateInfo->Image);
            }
            break;
        }
        case PXTextureTypeCube:
        {
            // pxTexture->Format = PXColorFormatRGBI8;

            PXECSCreateInfo pxResourceCreateInfoList[6];
            PXClearList(PXECSCreateInfo, &pxResourceCreateInfoList, 6);

            pxResourceCreateInfoList[0].Type = PXResourceTypeTexture;
            //pxResourceCreateInfoList[0].ObjectReference = (void**)&pxTexture->ImageA;
            pxResourceCreateInfoList[0].FilePath = pxTextureCreateInfo->Cube.Top;

            pxResourceCreateInfoList[1].Type = PXResourceTypeTexture;
            // pxResourceCreateInfoList[1].ObjectReference = (void**)&pxTexture->ImageB;
            pxResourceCreateInfoList[1].FilePath = pxTextureCreateInfo->Cube.Left;

            pxResourceCreateInfoList[2].Type = PXResourceTypeTexture;
            // pxResourceCreateInfoList[2].ObjectReference = (void**)&pxTexture->ImageC;
            pxResourceCreateInfoList[2].FilePath = pxTextureCreateInfo->Cube.Right;

            pxResourceCreateInfoList[3].Type = PXResourceTypeTexture;
            // pxResourceCreateInfoList[3].ObjectReference = (void**)&pxTexture->ImageD;
            pxResourceCreateInfoList[3].FilePath = pxTextureCreateInfo->Cube.Back;

            pxResourceCreateInfoList[4].Type = PXResourceTypeTexture;
            // pxResourceCreateInfoList[4].ObjectReference = (void**)&pxTexture->ImageE;
            pxResourceCreateInfoList[4].FilePath = pxTextureCreateInfo->Cube.Bottom;

            pxResourceCreateInfoList[5].Type = PXResourceTypeTexture;
            // pxResourceCreateInfoList[5].ObjectReference = (void**)&pxTexture->ImageF;
            pxResourceCreateInfoList[5].FilePath = pxTextureCreateInfo->Cube.Front;


            //const PXResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxTextureCreateInfo->Info.FilePath);

          //  //PXResourceManagerAddV(pxResourceCreateInfoList, 6);

            break;
        }
        case 2:
        {
            break;
        }

        default:
            break;
    }



    //const PXBool hasImageData = pxResourceCreateInfo->Texture.PixelData && pxResourceCreateInfo->Texture2D.Image.Image.PixelDataSize;

    //PXEngineTexture2DCreateData PXREF pxEngineTexture2DCreateData = &pxEngineResourceCreateInfo->Texture2D;

    // If we dont have a texture file path, instead of loading nothing, we
    // can pass the fail-back texture back. This prevents redundant materials and missing texture material
#if 0
    {
        const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePath;

        if(!hasFilePath)
        {
            // Do we have any other data to load?
            if(!hasImageData)
            {
                // Load failback texture
                *pxResourceCreateInfo->ObjectReference = _GLOBALResourceManager.Texture2DFailBack;

                return PXResultOK;
            }
        }
    }
#endif

    pxTexture->Filter = PXRenderFilterNoFilter;
    pxTexture->LayoutNear = PXTextureLayoutNearest;
    pxTexture->LayoutFar = PXTextureLayoutNearest;
    pxTexture->WrapHeight = PXTextureWrapRepeat;
    pxTexture->WrapWidth = PXTextureWrapRepeat;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTextureText,
        "Create",
        "Texture2D PXID:%i <%s>.",
        pxTexture->Info.ID,
        pxTextureCreateInfo->Info.FilePath.A
    );
#endif

    {
        PXECSCreateInfo pxResourceCreateInfoSub;
        PXClear(PXECSCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.Type = PXResourceTypeTexture;
        // pxResourceCreateInfoSub.ObjectReference = (void**)&pxTexture->Image;
        pxResourceCreateInfoSub.FilePath.A = pxTextureCreateInfo->Info.FilePath.A;
        // pxResourceCreateInfoSub.Image = pxResourceCreateInfo->Texture2D.Image; // What is this?

     //   //PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

    return PXResultOK;
}


#if OSWindows
HBITMAP PXAPI PXBitMapFromImage(const int width, const int height, const int amountofchannels, void* data)
{
    BITMAPINFO bmi;
    PXClear(BITMAPINFO, &bmi);
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Top-down DIB 
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 8 * amountofchannels;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* bitmapData;
    HBITMAP hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bitmapData, NULL, 0);

    if(!hBitmap)
    {
        return PXNull;
    }

    PXSize size = amountofchannels * width * height;

    PXMemoryCopy(data, bitmapData, size);

    return hBitmap;
}
#endif