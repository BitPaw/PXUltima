#include "PXMaterial.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>
#include "PXMaterial.hpp"

const char PXMaterialText[] = "Material";
const PXI8U PXMaterialTextLength = sizeof(PXMaterialText);
const PXECSRegisterInfoStatic PXMaterialRegisterInfoStatic =
{
    {sizeof(PXMaterialText), sizeof(PXMaterialText), PXMaterialText, TextFormatASCII},
    sizeof(PXMaterial),
    __alignof(PXMaterial),
    PXECSTypeEntity,
    PXMaterialCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXMaterialRegisterInfoDynamic;

void PXAPI PXMaterialRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXMaterialRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXMaterialRegisterInfoDynamic;
}

PXResult PXAPI PXMaterialCreate(PXMaterial** pxMaterialREF, PXMaterialCreateInfo PXREF pxMaterialCreateInfo)
{
    PXMaterial* pxMaterial = PXNull;

    if(!(pxMaterialREF && pxMaterialCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxMaterialCreateInfo->Info.Static = &PXMaterialRegisterInfoStatic;
    pxMaterialCreateInfo->Info.Dynamic = &PXMaterialRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxMaterialREF, pxMaterialCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxMaterial = *pxMaterialREF;
    PXCopy(PXMaterialData, &pxMaterialCreateInfo->Data, &pxMaterial->Data);

#if 0
    PXSize objectAmount = pxMaterialCreateInfo->Info.ObjectAmount;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXMaterialText,
        "Create",
        "Allocating x%i",
        objectAmount
    );
#endif

    void* keyList[64];

    for(PXSize materialIndex = 0; materialIndex < objectAmount; ++materialIndex)
    {
        PXMaterial PXREF pxMaterialCurrent = &pxMaterial[materialIndex];

        PXECSInfoIDGenerate(&pxMaterialCurrent->Info);   
        PXECSInfoFlagStateAdd(&pxMaterialCurrent->Info, PXECSFlagBehaviourCanRender);

        keyList[materialIndex] = &pxMaterialCurrent->Info.ID;
    }
#endif

    return PXResultOK;
}