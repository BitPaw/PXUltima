#include "PXLightDirectional.h"

const char PXLightDirectionalName[] = "Light-Directional";
const PXI8U PXLightDirectionalNameLength = sizeof(PXLightDirectionalName);
const PXECSRegisterInfoStatic PXLightDirectionalRegisterInfoStatic =
{
    {sizeof(PXLightDirectionalName), sizeof(PXLightDirectionalName), PXLightDirectionalName, TextFormatASCII},
    sizeof(PXLightDirectional),
    __alignof(PXLightDirectional),
    PXECSTypeEntity,
    PXLightDirectionalCreate,
    PXLightDirectionalRelease,
    PXLightDirectionalDraw
};
PXECSRegisterInfoDynamic PXLightDirectionalRegisterInfoDynamic;

PXResult PXAPI PXLightDirectionalRegisterToECS(void)
{
    PXECSRegister(&PXLightDirectionalRegisterInfoStatic, &PXLightDirectionalRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXLightDirectionalCreate(PXLightDirectional** pxLightDirectionalREF, PXLightDirectionalCreateInfo PXREF pxLightDirectionalCreateInfo)
{
    PXLightDirectional* pxLightDirectional = PXNull;

    pxLightDirectionalCreateInfo->Info.Static = &PXLightDirectionalRegisterInfoStatic;
    pxLightDirectionalCreateInfo->Info.Dynamic = &PXLightDirectionalRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxLightDirectionalREF, pxLightDirectionalCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxLightDirectional = *pxLightDirectionalREF;

    return PXResultOK;
}

PXResult PXAPI PXLightDirectionalRelease(const PXLightDirectional PXREF pxLightDirectional)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXLightDirectionalDraw(const PXLightDirectional PXREF pxLightDirectional, struct PXDrawInfo_ PXREF pxDrawInfo)
{
    return PXActionRefusedNotImplemented;
}
