#pragma once

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXColor.h>

typedef struct PXTexture3D_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;

        struct ID3D11Texture3D* DX_11;     
        struct IDirect3DVolumeTexture9* DX_9;
    };

    PXSize Width;
    PXSize Height;
    PXSize Depth;

    PXColorFormat Format;
}
PXTexture3D;

typedef struct PXTexture3DCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize Width;
    PXSize Height;
    PXSize Depth;
}
PXTexture3DCreateInfo;


PXPublic void PXAPI PXTexture3DRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXTexture3DCreate(PXTexture3D** pxTexture3DREF, PXTexture3DCreateInfo PXREF pxTexture3DCreateInfo);