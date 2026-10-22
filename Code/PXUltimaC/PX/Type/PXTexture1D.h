#pragma once
#include <PX/Engine/ECS/PXECS.h>

typedef struct PXTexture1D_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;

        // DX11
        struct ID3D11Texture1D* DX_11;
    };

    PXSize Width;
}
PXTexture1D;

typedef struct PXTexture1DCreateInfo_
{
    PXECSCreateInfo Info;
}
PXTexture1DCreateInfo;

PXPublic void PXAPI PXTexture1DRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXTexture1DCreate(PXTexture1D** pxTexture2DREF, PXTexture1DCreateInfo PXREF pxTexture1DCreateInfo);