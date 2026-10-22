#include "PXShaderPixelFragment.h"

const char PXShaderPixelFragmentText[] = "ShaderVertex";
const PXI8U PXShaderPixelFragmentTextLength = sizeof(PXShaderPixelFragmentText);
const PXECSRegisterInfoStatic PXShaderPixelFragmentRegisterInfoStatic =
{
    {sizeof(PXShaderPixelFragmentText), sizeof(PXShaderPixelFragmentText), PXShaderPixelFragmentText, TextFormatASCII},
    sizeof(PXShaderPixelFragment),
    __alignof(PXShaderPixelFragment),
    PXECSTypeSystem,
    PXShaderPixelFragmentCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXShaderPixelFragmentRegisterInfoDynamic;

PXResult PXAPI PXShaderPixelFragmentCreate(PXShaderPixelFragment** pxShaderPixelFragmentREF, PXShaderPixelFragmentCreateInfo PXREF pxShaderPixelFragmentCreateInfo)
{
    PXShaderPixelFragment* pxShaderPixelFragment = PXNull;

    if(!(pxShaderPixelFragmentREF && pxShaderPixelFragmentCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxShaderPixelFragmentCreateInfo->Info.Static = &PXShaderPixelFragmentRegisterInfoStatic;
    pxShaderPixelFragmentCreateInfo->Info.Dynamic = &PXShaderPixelFragmentRegisterInfoDynamic;

    PXResult pxResult = PXECSCreate(pxShaderPixelFragmentREF, pxShaderPixelFragmentCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxShaderPixelFragment = *pxShaderPixelFragmentREF;

    PXShaderCreate(&pxShaderPixelFragment->ShaderBase, &pxShaderPixelFragmentCreateInfo->Shader);

    return PXResultOK;
}