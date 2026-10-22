#pragma once

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Container/Buffer/PXBuffer.h>
#include <PX/Type/PXColor.h>

typedef struct PXTexture2D_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;

        struct ID3D11Texture2D* DX_11;
        struct IDirect3DTexture9* DX_9;
    };

    PXBuffer Data;

    PXSize Width;
    PXSize Height;

    PXColorFormat Format;
}
PXTexture2D;

typedef struct PXTexture2DCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize Width;
    PXSize Height;
    PXSize Minmaps;

    // [Note] FilePath is in info
}
PXTexture2DCreateInfo;


PXPublic void PXAPI PXTexture2DRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXTexture2DCreate(PXTexture2D** pxTexture2DREF, PXTexture2DCreateInfo PXREF pxTexture2DCreateInfo);

PXPublic PXResult PXAPI PXTexture2DResize
(
    PXTexture2D PXREF pxTexture2D,
    const PXSize width, 
    const PXSize height,
    const PXColorFormat pxColorFormat
);

PXPublic PXResult PXAPI PXTexture2DGenerateCheckerboard
(
    PXTexture2D PXREF pxTexture2D, 
    const PXColorRGB_U8 PXREF colorA,
    const PXColorRGB_U8 PXREF colorB,
    const PXSize checkSize
);

PXPublic PXResult PXAPI PXTexture2DGenerateFractalXOR
(
    PXTexture2D PXREF pxTexture2D
);


PXPublic void* PXAPI PXTexture2DPixelPosition
(
    const PXTexture2D PXREF pxTexture2D, 
    const PXSize x,
    const PXSize y
);


PXPublic PXResult PXAPI PXTexture2DBGRI8ToRGBI8(PXTexture2D PXREF pxTexture2D);


PXPublic PXResult PXAPI PXTexture2DFlipHorizontal(PXTexture2D PXREF pxTexture2D);
PXPublic PXResult PXAPI PXTexture2DFlipVertical(PXTexture2D PXREF pxTexture2D);
PXPublic PXResult PXAPI PXTexture2DColorRemove(PXTexture2D PXREF pxTexture2D, const PXByte red, const PXByte green, const PXByte blue);


#if OSWindows
PXPublic HBITMAP PXAPI PXTexture2DToBitMap(PXTexture2D PXREF pxTexture2D);
PXPublic HBITMAP PXAPI PXTexture2DToBitMapFromData(const PXI32S width, const PXI32S height, const PXI16U amountOfChannels, const void* data);
#endif