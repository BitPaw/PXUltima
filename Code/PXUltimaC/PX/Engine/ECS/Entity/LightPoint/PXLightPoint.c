#include "PXLightPoint.h"

const char PXLightPointName[] = "Light-Point";
const PXI8U PXLightPointNameLength = sizeof(PXLightPointName);
const PXECSRegisterInfoStatic PXLightPointRegisterInfoStatic =
{
    {sizeof(PXLightPointName), sizeof(PXLightPointName), PXLightPointName, TextFormatASCII},
    sizeof(PXLightPoint),
    __alignof(PXLightPoint),
    PXECSTypeEntity,
    PXLightPointCreate,
    PXLightPointRelease,
    PXLightPointDraw
};
PXECSRegisterInfoDynamic PXLightPointRegisterInfoDynamic;

PXResult PXAPI PXLightPointRegisterToECS(void)
{
    PXECSRegister(&PXLightPointRegisterInfoStatic, &PXLightPointRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXLightPointCreate(PXLightPoint** pxLightPointREF, PXLightPointCreateInfo PXREF pxLightPointCreateInfo)
{
    if(!(pxLightPointREF && pxLightPointCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXLightPoint* pxLightPoint = PXNull;

    pxLightPointCreateInfo->Info.Static = &PXLightPointRegisterInfoStatic;
    pxLightPointCreateInfo->Info.Dynamic = &PXLightPointRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxLightPointREF, pxLightPointCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxLightPoint = *pxLightPointREF;

    return PXResultOK;
}

PXResult PXAPI PXLightPointRelease(const PXLightPoint PXREF pxLightPoint)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXLightPointDraw(const PXLightPoint PXREF pxLightPoint, struct PXWindowDrawInfo_ PXREF pxDrawInfo)
{
    return PXActionRefusedNotImplemented;
}