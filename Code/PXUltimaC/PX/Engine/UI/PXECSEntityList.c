#include "PXECSEntityList.h"
#include <gl/GL.h>
#include <PX/Math/PXMath.h>

const char PXECSEntityListText[] = "ECS-EntityList";
const PXI8U PXECSEntityListTextLength = sizeof(PXECSEntityListText);
const PXECSRegisterInfoStatic PXECSEntityListInfoStatic =
{
    {sizeof(PXECSEntityListText), sizeof(PXECSEntityListText), PXECSEntityListText, TextFormatASCII},
    sizeof(PXECSEntityList),
    __alignof(PXECSEntityList),
    PXECSTypeEntity,
    PXECSEntityListCreate,
    PXNull,
    PXECSEntityListDraw
};
PXECSRegisterInfoDynamic PXECSEntityListInfoDynamic;

void PXAPI PXECSEntityListRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXECSEntityListInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXECSEntityListInfoDynamic;
}

PXResult PXAPI PXECSEntityListCreate(PXECSEntityList** pxECSEntityListREF, PXECSEntityListCreateInfo PXREF pxECSEntityListCreateInfo)
{
    PXECSEntityList* pxECSEntityList = PXNull;

    if(!(pxECSEntityListREF && pxECSEntityListCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxECSEntityListCreateInfo->Info.Static = &PXECSEntityListInfoStatic;
    pxECSEntityListCreateInfo->Info.Dynamic = &PXECSEntityListInfoDynamic;
    PXResult pxResult = PXECSCreate(pxECSEntityListREF, pxECSEntityListCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxECSEntityList = *pxECSEntityListREF;


    pxECSEntityListCreateInfo->Window.EventList.CallBackOwner = pxECSEntityList;
    pxECSEntityListCreateInfo->Window.EventList.CallBackDraw = PXECSEntityListDraw;

    PXWindowCreate(&pxECSEntityList->WindowBase, &pxECSEntityListCreateInfo->Window);

    return PXResultOK;
}

PXResult PXAPI PXECSEntityListDraw(PXECSEntityList PXREF pxECSEntityList, PXDrawInfo PXREF pxDrawInfo)
{
    if(!(pxECSEntityList && pxDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXText pxText;
    char buffer[128];
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));

    pxDrawInfo->Text = &pxText;
    //pxDrawInfo->RectangleXYWH.Width = 150;
    //pxDrawInfo->RectangleXYWH.Height = 20;

    PXTextPrint(&pxText, "Scene");
    PXWindowDrawText(pxECSEntityList->WindowBase, pxDrawInfo);

    PXTextDrawInfoRowNext(pxDrawInfo);

    PXSize amount = PXECSEntryAmount();    

    amount = PXMathMaximum(amount, 10);

    for(PXSize i = 0; i < amount; i++)
    {
        PXECSRegisterInfo pxECSTypeInfo;

        PXResult pxResult = PXECSTypeGet(&pxECSTypeInfo, i);

        if(PXResultOK == pxResult)
        {
            const char* typeName = PXECSTypeToString(pxECSTypeInfo.InfoStatic->Type);

            PXTextPrint
            (
                &pxText, 
                "[%4i] %-15s (%s)",
                pxECSTypeInfo.InfoDynamic->ID,
                pxECSTypeInfo.InfoStatic->NameOfType.A,
                typeName          
            );
        }
        else
        {
            PXTextPrint(&pxText, "TEXT:%i", -1);
        }  

        PXWindowDrawText
        (
            pxECSEntityList->WindowBase,
            pxDrawInfo
        );

        PXTextDrawInfoRowNext(pxDrawInfo);
    }

    return PXResultOK;
}
