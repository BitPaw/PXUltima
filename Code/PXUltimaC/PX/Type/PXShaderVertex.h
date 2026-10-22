#pragma once

#include <PX/Type/PXShader.h>

typedef struct PXShaderVertex_
{
    PXShader ShaderBase;

    union
    {
        PXI32U OpenGLID;

        struct IDirect3DVertexShader9* VertexShader_9;
        struct ID3D11VertexShader* VertexShader_11; // ID3D11VertexShader
    };
}
PXShaderVertex;

typedef struct PXShaderVertexCreateInfo_
{
    PXECSCreateInfo Info;

    PXShaderCreateInfo Shader;
}
PXShaderVertexCreateInfo;

PXPublic PXResult PXAPI PXShaderVertexCreate(PXShaderVertex** pxShaderVertexREF, PXShaderVertexCreateInfo PXREF pxShaderVertexCreateInfo);