#pragma once
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXTexture2D.h>

typedef struct PXTextureCube_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;

        struct IDirect3DCubeTexture9* DX9;
    };

    union
    {
        PXTexture2D* Textures[6];

        struct
        {
            PXTexture2D* Top;
            PXTexture2D* Left;
            PXTexture2D* Right;
            PXTexture2D* Back;
            PXTexture2D* Bottom;
            PXTexture2D* Front;
        };
    };

    PXColorFormat Format;
}
PXTextureCube;

typedef struct PXTextureCubeCreateInfo_
{
    PXECSCreateInfo Info;

    PXTexture2D* Top;
    PXTexture2D* Left;
    PXTexture2D* Right;
    PXTexture2D* Back;
    PXTexture2D* Bottom;
    PXTexture2D* Front;
}
PXTextureCubeCreateInfo;


PXPublic void PXAPI PXTextureCubeRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXTextureCubeCreate(PXTextureCube** pxTextureCubeREF, PXTextureCubeCreateInfo PXREF pxTextureCubeCreateInfo);