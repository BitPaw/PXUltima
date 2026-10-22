#include "PXMod.h"


const char PXModText[] = "Mod-PX";
const PXI8U PXModTextLength = sizeof(PXModText);
const PXECSRegisterInfoStatic PXModInfoStatic =
{
    {sizeof(PXModText), sizeof(PXModText), PXModText, TextFormatASCII},
    sizeof(PXMod),
    __alignof(PXMod),
    PXECSTypeComponent,
    PXModCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXModInfoDynamic;


PXResult PXAPI PXModCreate(PXMod** pxModREF, PXModCreateInfo PXREF pxModCreateInfo)
{
    PXMod* pxMod = PXNull;

    if(!(pxModREF && pxModCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxModCreateInfo->Info.Static = &PXModInfoStatic;
    pxModCreateInfo->Info.Dynamic = &PXModInfoDynamic;
    PXResult pxResult = PXECSCreate(pxModREF, pxModCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxMod = *pxModREF;

    return pxResult;
}

PXResult PXAPI PXModRelease(PXMod PXREF pxMod)
{
    return PXResultOK;
}