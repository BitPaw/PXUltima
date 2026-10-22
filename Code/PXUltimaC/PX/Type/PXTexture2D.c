#include "PXTexture2D.h"

#include <PX/OS/PXOS.h>

const char PXTexture2DText[] = "Texture2D";
const PXI8U PXTexture2DTextLength = sizeof(PXTexture2DText);
const PXECSRegisterInfoStatic PXTexture2DRegisterInfoStatic =
{
    {sizeof(PXTexture2DText), sizeof(PXTexture2DText), PXTexture2DText, TextFormatASCII},
    sizeof(PXTexture2D),
    __alignof(PXTexture2D),
    PXECSTypeSystem,
    PXTexture2DCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXTexture2DRegisterInfoDynamic;

void PXAPI PXTexture2DRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXTexture2DRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXTexture2DRegisterInfoDynamic;    
}

PXResult PXAPI PXTexture2DCreate(PXTexture2D** pxTexture2DREF, PXTexture2DCreateInfo PXREF pxTexture2DCreateInfo)
{
    PXTexture2D* pxTexture2D = PXNull;

    if(!(pxTexture2DREF && pxTexture2DCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxTexture2DCreateInfo->Info.Static = &PXTexture2DRegisterInfoStatic;
    pxTexture2DCreateInfo->Info.Dynamic = &PXTexture2DRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxTexture2DREF, pxTexture2DCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTexture2D = *pxTexture2DREF;


    // Load texture
    PXBool hasData = PXTextHasData(&pxTexture2DCreateInfo->Info.FilePath);

    if(hasData)
    {
        const PXResult loadResult = PXECSLoad(pxTexture2D, pxTexture2DCreateInfo);

        if(PXResultOK != loadResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXTexture2DText,
                "Create",
                "Failed <%s>!",
                pxTexture2DCreateInfo->Info.FilePath.A
            );
#endif

            return loadResult;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXTexture2DText,
            "Create",
            "Successful <%s>.",
            pxTexture2DCreateInfo->Info.FilePath.A
        );
#endif
    }
    else
    {
        if(pxTexture2DCreateInfo->Width == 0 || pxTexture2DCreateInfo->Height == 0)
        {
            pxTexture2DCreateInfo->Width = 256;
            pxTexture2DCreateInfo->Height = 256;
        }

        PXTexture2DResize
        (
            pxTexture2D,
            pxTexture2DCreateInfo->Width,
            pxTexture2DCreateInfo->Height,
            PXColorFormatRGBI8
        );


        PXColorRGB_U8 pink = { 150, 0, 150};
        PXColorRGB_U8 black = { 20, 0, 20 };

        PXTexture2DGenerateCheckerboard(pxTexture2D, &pink, &black, 5);
        //PXTexture2DGenerateFractalXOR(pxTexture2D);
    }

    return PXResultOK;
}

PXResult PXAPI PXTexture2DResize
(
    PXTexture2D PXREF pxTexture2D, 
    const PXSize width, 
    const PXSize height,
    const PXColorFormat pxColorFormat
)
{ 
    const PXSize bytePerPixel = PXColorFormatBytePerPixel(pxColorFormat);
    const PXSize pixelDataSize = width * height * bytePerPixel;

    PXBufferResize(&pxTexture2D->Data, pixelDataSize);

    pxTexture2D->Width = width;
    pxTexture2D->Height = height;
    pxTexture2D->Format = pxColorFormat;

    return PXResultOK;
}

PXResult PXAPI PXTexture2DGenerateCheckerboard
(
    PXTexture2D PXREF pxTexture2D, 
    const PXColorRGB_U8 PXREF colorA, 
    const PXColorRGB_U8 PXREF colorB, 
    const PXSize checkSize
)
{
    for(PXSize y = 0; y < pxTexture2D->Height; y++)
    {
        for(PXSize x = 0; x < pxTexture2D->Width; x++)
        {
            PXSize idx = y * pxTexture2D->Width + x;
            // XOR the grid coordinates to create a alternating grid

            PXBool colorSelect = ((x / checkSize) ^ (y / checkSize)) & 1;

            PXColorRGB_U8* colorData = &pxTexture2D->Data.ColorRGBI8[idx];

            if(colorSelect)
            {
                *colorData = *colorA;
            }
            else 
            {
                *colorData = *colorB;
            }
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXTexture2DGenerateFractalXOR(PXTexture2D PXREF pxTexture2D)
{
    for(PXSize y = 0; y < pxTexture2D->Height; y++)
    {
        for(PXSize x = 0; x < pxTexture2D->Width; x++)
        {
            PXSize idx = y * pxTexture2D->Width + x;
            PXColorRGB_U8* colorData = &pxTexture2D->Data.ColorRGBI8[idx];
           
            PXI8U value = (PXI8U)(x ^ y) * 0.8;
            PXColorRGB_U8 color = { (PXI8U)(255 - value), value,  (PXI8U)(255 - value) };

            // Pure bitwise operation creates a fractal pattern

            // Map the single value out to an electric purple/cyan spectrum
            *colorData = color;
        }
    }

    return PXResultOK;
}

void* PXAPI PXTexture2DPixelPosition(const PXTexture2D PXREF pxTexture2D, const PXSize x, const PXSize y)
{
    const PXSize bytePerPixel = PXColorFormatBytePerPixel(pxTexture2D->Format);

    const PXSize index = (y + x * pxTexture2D->Width) * bytePerPixel;

    void* data = PXBufferDataWithOffset(&pxTexture2D->Data, index);

    return data;
}

PXResult PXAPI PXTexture2DBGRI8ToRGBI8(PXTexture2D PXREF pxTexture2D)
{
    if(pxTexture2D->Format != PXColorFormatBGRI8)
    {
        return PXResultRefuedImageFormatInvalid;
    }

    for(PXSize i = 0; i < pxTexture2D->Data.SizeAllowedToUse; i += 3)
    {
        PXByte* data = &pxTexture2D->Data.Data[i];

        PXByte temp = data[0]; // Store Blue
        data[0] = data[2]; // Move Red to Blue position
        data[2] = temp;    // Move Blue to Red position
    }

    pxTexture2D->Format = PXColorFormatRGBI8;

    return PXResultOK;
}

PXResult PXAPI PXTexture2DFlipHorizontal(PXTexture2D PXREF pxTexture2D)
{
    const PXSize bbp = PXColorFormatBytePerPixel(pxTexture2D->Format);
    const PXSize rowSize = (pxTexture2D->Width * bbp);
    const PXSize length = (pxTexture2D->Width * bbp) / 2;

    for(PXSize x = 0; x < length; x += bbp) //
    {
        const PXSize xB = rowSize - x - bbp;

        for(PXSize y = 0; y < pxTexture2D->Height; y++)
        {
            const PXSize indexA = x + (y * rowSize);
            const PXSize indexB = xB + (y * rowSize);
            PXByte tempByte[4] = { 0,0,0,0 };
            PXByte* pixelA = pxTexture2D->Data.Data + indexA;
            PXByte* pixelB = pxTexture2D->Data.Data + indexB;

            PXMemoryCopyX(pixelA, 4, tempByte, bbp);
            PXMemoryCopyX(pixelB, 4, pixelA, bbp);
            PXMemoryCopyX(tempByte, 4, pixelB, bbp);
        }
    }

    return PXResultInvalid;
}

PXResult PXAPI PXTexture2DFlipVertical(PXTexture2D PXREF pxTexture2D)
{
    if(!pxTexture2D)
    {
        return PXResultRefusedParameterNull;
    }

    const PXSize bbp = PXColorFormatBytePerPixel(pxTexture2D->Format);
    const PXSize scanLineWidthSize = pxTexture2D->Width * bbp;
    const PXSize scanLinesToSwap = pxTexture2D->Height / 2u;
    PXByte* copyBufferRow = PXMemoryHeapCallocT(PXByte, scanLineWidthSize);

    if(!copyBufferRow)
    {
        return PXResultFailedMemoryAllocation;
    }

    for(PXSize scanlineIndex = 0; scanlineIndex < scanLinesToSwap; scanlineIndex++)
    {
        PXByte* bufferA = pxTexture2D->Data.Data + (scanlineIndex * scanLineWidthSize);
        PXByte* bufferB = pxTexture2D->Data.Data + ((pxTexture2D->Height - scanlineIndex) * scanLineWidthSize) - scanLineWidthSize;

        PXMemoryCopy(bufferB, copyBufferRow, scanLineWidthSize); // A -> Buffer 'Save A'
        PXMemoryCopy(bufferA, bufferB, scanLineWidthSize); // B -> A 'Move B to A(override)'
        PXMemoryCopy(copyBufferRow, bufferA, scanLineWidthSize); // Buffer -> B 'Move SaveCopy (A) to B'
    }

    PXMemoryHeapFree(PXNull, copyBufferRow);

    return PXResultInvalid;
}

PXResult PXAPI PXTexture2DColorRemove(PXTexture2D PXREF pxTexture2D, const PXByte red, const PXByte green, const PXByte blue)
{
    // DO we have alpha? If not, make one

    const PXSize currentOffset = pxTexture2D->Data.CursorOffsetByte;
    PXSize dataOffset = 0;

    dataOffset = pxTexture2D->Data.CursorOffsetByte;

    const PXSize length = pxTexture2D->Width * pxTexture2D->Height;

    for(PXSize pixelIndex = 0; pixelIndex < length; ++pixelIndex)
    {
        PXByte PXREF oldData = pxTexture2D->Data.Data + currentOffset - ((pixelIndex + 1) * 3u);
        PXByte PXREF newData = pxTexture2D->Data.Data + dataOffset - ((pixelIndex + 1) * 4u);

        newData[0] = oldData[0];
        newData[1] = oldData[1];
        newData[2] = oldData[2];
        newData[3] = !(oldData[0] == red) && (oldData[1] == green) && (oldData[2] == blue);
    }

    return PXResultOK;
}

#if OSWindows
HBITMAP PXAPI PXTexture2DToBitMap(PXTexture2D PXREF pxTexture2D)
{
    HBITMAP bitMapHandle = PXTexture2DToBitMapFromData
    (
        pxTexture2D->Width, 
        pxTexture2D->Height,
        1, 
        pxTexture2D->Data.Address
    );

    return bitMapHandle;
}

HBITMAP PXAPI PXTexture2DToBitMapFromData(const PXI32S width, const PXI32S height, const PXI16U amountOfChannels, const void* data)
{
    BITMAPINFO bmi;
    PXClear(BITMAPINFO, &bmi);
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 8 * amountOfChannels;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* bitmapData;
    HBITMAP hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bitmapData, NULL, 0);

    if(!hBitmap)
    {
        return PXNull;
    }

    PXSize size = amountOfChannels * width * height;

    PXMemoryCopy(data, bitmapData, size);

    return hBitmap;
}
#endif