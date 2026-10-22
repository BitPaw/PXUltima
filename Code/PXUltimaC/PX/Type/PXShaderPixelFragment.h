#pragma once
#include <PX/Type/PXShader.h>

typedef struct PXShaderPixelFragment_
{
    PXShader ShaderBase;

    union
    {
        PXI32U OpenGLID;

        struct IDirect3DPixelShader9* PixelShader_9;
        struct ID3D11PixelShader* PixelShader_11;
    };
}
PXShaderPixelFragment;

typedef struct PXShaderPixelFragmentCreateInfo_
{
    PXECSCreateInfo Info;

    PXShaderCreateInfo Shader;
}
PXShaderPixelFragmentCreateInfo;

PXPublic PXResult PXAPI PXShaderPixelFragmentCreate(PXShaderPixelFragment** pxShaderPixelFragmentREF, PXShaderPixelFragmentCreateInfo PXREF pxShaderPixelFragmentCreateInfo);