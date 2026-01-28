#include "PXLightSpot.h"

const char PXLightSpotName[] = "Light-Point";
const PXI8U PXLightSpotNameLength = sizeof(PXLightSpotName);
const PXECSRegisterInfoStatic PXLightSpotRegisterInfoStatic =
{
    {sizeof(PXLightSpotName), sizeof(PXLightSpotName), PXLightSpotName, TextFormatASCII},
    sizeof(PXLightSpot),
    __alignof(PXLightSpot),
    PXECSTypeEntity,
    PXLightSpotCreate,
    PXLightSpotRelease,
    PXLightSpotDraw
};
PXECSRegisterInfoDynamic PXLightSpotRegisterInfoDynamic;

PXResult PXAPI PXLightSpotRegisterToECS(void)
{
    PXECSRegister(&PXLightSpotRegisterInfoStatic, &PXLightSpotRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXLightSpotCreate(PXLightSpot** pxLightSpotREF, PXLightSpotCreateInfo PXREF pxLightSpotCreateInfo)
{
    if(!(pxLightSpotREF && pxLightSpotCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXLightSpot* pxLightSpot = PXNull;

    pxLightSpotCreateInfo->Info.Static = &PXLightSpotRegisterInfoStatic;
    pxLightSpotCreateInfo->Info.Dynamic = &PXLightSpotRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxLightSpotREF, pxLightSpotCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxLightSpot = *pxLightSpotREF;

    return PXResultOK;
}

PXResult PXAPI PXLightSpotRelease(const PXLightSpot PXREF pxLightSpot)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXLightSpotDraw(const PXLightSpot PXREF pxLightSpot, struct PXWindowDrawInfo_ PXREF pxDrawInfo)
{
    return PXActionRefusedNotImplemented;
}