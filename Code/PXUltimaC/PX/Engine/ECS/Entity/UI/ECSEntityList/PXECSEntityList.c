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

PXResult PXAPI PXECSEntityListRegisterToECS()
{
    PXECSRegister(&PXECSEntityListInfoStatic, &PXECSEntityListInfoDynamic);

    return PXResultOK;
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

    char buffer[128];

    PXText pxText;
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));

    PXTextDrawInfo pxTextDrawInfo;
    PXClear(PXTextDrawInfo, &pxTextDrawInfo);
    pxTextDrawInfo.WindowDrawInfo = pxDrawInfo;
    pxTextDrawInfo.Text = &pxText;

    PXSize amount = PXECSEntryAmount();    

    amount = PXMathMaximum(amount, 10);

    for(size_t i = 0; i < amount; i++)
    {
        pxTextDrawInfo.Y -= 0.8;

        PXECSTypeInfo pxECSTypeInfo;

        PXResult pxResult = PXECSTypeGet(&pxECSTypeInfo, i);

        if(PXResultOK == pxResult)
        {
            const char* typeName = PXECSTypeToString(pxECSTypeInfo.Static->Type);

            PXTextPrint
            (
                &pxText, 
                "[%4i] %-15s (%s)",
                pxECSTypeInfo.Dynamic->ID,
                pxECSTypeInfo.Static->NameOfType.A,
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
            &pxTextDrawInfo
        );
    }
    

   

    return PXResultOK;
}
