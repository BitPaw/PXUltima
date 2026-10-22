#include "PXSprite.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Type/PXFieldEffect.h>

const char PXSpriteName[] = "Sprite";
const PXI8U PXSpriteNameLength = sizeof(PXSpriteName);
const PXECSRegisterInfoStatic PXSpriteRegisterInfoStatic =
{
    {sizeof(PXSpriteName), sizeof(PXSpriteName), PXSpriteName, TextFormatASCII},
    sizeof(PXSprite),
    __alignof(PXSprite),
    PXECSTypeEntity,
    PXModelCreate,
    PXModelRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXSpriteRegisterInfoDynamic;


void PXAPI PXSpriteRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXSpriteRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXSpriteRegisterInfoDynamic;
}

PXResult PXAPI PXSpriteCreate(PXSprite** pxSpriteREF, PXSpriteCreateInfo PXREF pxSpriteCreateInfo)
{
    PXSprite* pxSprite = PXNull;

    if(!(pxSpriteREF && pxSpriteCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxSpriteCreateInfo->Info.Static = &PXSpriteRegisterInfoStatic;
    pxSpriteCreateInfo->Info.Dynamic = &PXSpriteRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxSpriteREF, pxSpriteCreateInfo);
     
    if(pxResult != PXResultOK)
    {
        return pxResult;
    }

    pxSprite = *pxSpriteREF;

    PXQuadCreate(&pxSprite->Quad);


    PXMaterial* pxMaterial;

    {
        PXMaterialCreateInfo pxMaterialCreateInfo;
        PXClear(PXMaterialCreateInfo, &pxMaterialCreateInfo);
        pxMaterialCreateInfo.Data.ShaderProgram = pxSpriteCreateInfo->ShaderProgramCurrent;
        pxMaterialCreateInfo.Data.TextureBase = pxSpriteCreateInfo->TextureCurrent;

        PXMaterialCreate(&pxMaterial, &pxMaterialCreateInfo);
    }



    PXModelMaterialSet(pxSprite->Quad->Model, pxMaterial, 0, 0);

    return PXResultOK;
}

PXResult PXAPI PXSpriteRelease(PXSprite PXREF pxSprite)
{
    return PXResultRefusedNotImplemented;
}