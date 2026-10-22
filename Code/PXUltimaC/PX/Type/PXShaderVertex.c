#include "PXShaderVertex.h"


const char PXShaderVertexText[] = "ShaderVertex";
const PXI8U PXShaderVertexTextLength = sizeof(PXShaderVertexText);
const PXECSRegisterInfoStatic PXShaderVertexRegisterInfoStatic =
{
    {sizeof(PXShaderVertexText), sizeof(PXShaderVertexText), PXShaderVertexText, TextFormatASCII},
    sizeof(PXShaderVertex),
    __alignof(PXShaderVertex),
    PXECSTypeSystem,
    PXShaderVertexCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXShaderVertexRegisterInfoDynamic;

PXResult PXAPI PXShaderVertexCreate(PXShaderVertex** pxShaderVertexREF, PXShaderVertexCreateInfo PXREF pxShaderVertexCreateInfo)
{
    PXShaderVertex* pxShaderVertex = PXNull;

    if(!(pxShaderVertexREF && pxShaderVertexCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxShaderVertexCreateInfo->Info.Static = &PXShaderVertexRegisterInfoStatic;
    pxShaderVertexCreateInfo->Info.Dynamic = &PXShaderVertexRegisterInfoDynamic;

    PXResult pxResult = PXECSCreate(pxShaderVertexREF, pxShaderVertexCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxShaderVertex = *pxShaderVertexREF;

    PXShaderCreate(&pxShaderVertex->ShaderBase, &pxShaderVertexCreateInfo->Shader);

    return PXResultOK;
}