#include "PXECSEntityList.h"

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

    pxECSEntityListCreateInfo->Info.Static = &PXECSEntityListInfoStatic;
    pxECSEntityListCreateInfo->Info.Dynamic = &PXECSEntityListInfoDynamic;
    PXECSCreate(pxECSEntityListREF, pxECSEntityListCreateInfo);

    pxECSEntityList = *pxECSEntityListREF;

    return PXResultOK;
}

PXResult PXAPI PXECSEntityListDraw(PXECSEntityList PXREF pxECSEntityList, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    return PXResultOK;
}
