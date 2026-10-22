#include "PXTextureCube.h"

const char PXTextureCubeText[] = "TextureCube";
const PXI8U PXTextureCubeTextLength = sizeof(PXTextureCubeText);
const PXECSRegisterInfoStatic PXTextureCubeRegisterInfoStatic =
{
    {sizeof(PXTextureCubeText), sizeof(PXTextureCubeText), PXTextureCubeText, TextFormatASCII},
    sizeof(PXTextureCube),
    __alignof(PXTextureCube),
    PXECSTypeSystem,
    PXTextureCubeCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXTextureCubeRegisterInfoDynamic;

void PXAPI PXTextureCubeRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXTextureCubeRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXTextureCubeRegisterInfoDynamic;
}

PXResult PXAPI PXTextureCubeCreate(PXTextureCube** pxTextureCubeREF, PXTextureCubeCreateInfo PXREF pxTextureCubeCreateInfo)
{
    PXTextureCube* pxTextureCube = PXNull;

    if(!(pxTextureCubeREF && pxTextureCubeCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxTextureCubeCreateInfo->Info.Static = &PXTextureCubeRegisterInfoStatic;
    pxTextureCubeCreateInfo->Info.Dynamic = &PXTextureCubeRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxTextureCubeREF, pxTextureCubeCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTextureCube = *pxTextureCubeREF;
    pxTextureCube->Top = pxTextureCubeCreateInfo->Top;
    pxTextureCube->Left = pxTextureCubeCreateInfo->Left;
    pxTextureCube->Right = pxTextureCubeCreateInfo->Right;
    pxTextureCube->Back = pxTextureCubeCreateInfo->Back;
    pxTextureCube->Bottom = pxTextureCubeCreateInfo->Bottom;
    pxTextureCube->Front = pxTextureCubeCreateInfo->Front;


    return pxResult;
}