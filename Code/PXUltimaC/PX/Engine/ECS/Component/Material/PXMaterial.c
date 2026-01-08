#include "PXMaterial.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>

const char PXMaterialText[] = "Material";

PXMaterial* PXAPI PXMaterialContainerFind(const PXMaterialContainer PXREF pxMaterialContainer, PXText PXREF pxMaterialName)
{
    if(!pxMaterialContainer)
    {
        return PXNull;
    }

    //for (PXSize materialContainerID = 0; materialContainerID < pxMaterialContainer->MaterialContaierListSize; ++materialContainerID)
    //{
    // const PXMaterialContainer PXREF pxMaterialContaier = &pxModel->MaterialContaierList[materialContainerID];

    for(PXSize materialID = 0; materialID < pxMaterialContainer->MaterialListAmount; ++materialID)
    {
        PXMaterial PXREF pxMaterial = &pxMaterialContainer->MaterialList[materialID];

        PXECSProperty pxECSProperty;
        PXClear(PXECSProperty, &pxECSProperty);

        PXResourcePropertyIO(&pxMaterial->Info, &pxECSProperty, PXResourcePropertyName, PXResourcePropertyFetch);

        const PXBool isMatch = PXTextCompare
        (
            pxMaterialName,
            &pxECSProperty.Text
            //PXTextCompareRequireSameLength
        );

        if(isMatch)
        {
            return pxMaterial;
        }
    }
    // }

    return PXNull;
}

PXResult PXAPI PXMaterialCreate(PXMaterial PXREF pxMaterial, PXMaterialCreateInfo PXREF pxMaterialCreateInfo)
{
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

        pxMaterialCurrent->Info.ID = PXIDGenerate();
        pxMaterialCurrent->Info.Behaviour |= PXECSInfoRender;

        keyList[materialIndex] = &pxMaterialCurrent->Info.ID;
    }

    return PXActionSuccessful;
}