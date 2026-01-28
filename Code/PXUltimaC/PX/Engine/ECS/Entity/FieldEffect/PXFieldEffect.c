#include "PXFieldEffect.h"
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>
#include <PX/OS/PXOS.h>


const char PXFieldEffectName[] = "FieldEffect";
const PXI8U PXFieldEffectNameLength = sizeof(PXFieldEffectName);
const PXECSRegisterInfoStatic PXFieldEffectNameRegisterInfoStatic =
{
    {sizeof(PXFieldEffectName), sizeof(PXFieldEffectName), PXFieldEffectName, TextFormatASCII},
    sizeof(PXFieldEffect),
    __alignof(PXFieldEffect),
    PXECSTypeEntity,
    PXFieldEffectCreate,
    PXFieldEffectRelease,
    PXFieldEffectDraw
};
PXECSRegisterInfoDynamic PXFieldEffectNameRegisterInfoDynamic;

PXResult PXAPI PXFieldEffectRegisterToECS(void)
{
    PXECSRegister(&PXFieldEffectNameRegisterInfoStatic, &PXFieldEffectNameRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXFieldEffectCreate(PXFieldEffect** pxFieldEffectREF, PXFieldEffectCreateInfo PXREF pxFieldEffectCreateInfo)
{
    PXFieldEffect* pxFieldEffect = PXNull;

    pxFieldEffectCreateInfo->Info.Static = &PXFieldEffectNameRegisterInfoStatic;
    pxFieldEffectCreateInfo->Info.Dynamic = &PXFieldEffectNameRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxFieldEffectREF, pxFieldEffectCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxFieldEffect = *pxFieldEffectREF;

    pxFieldEffect->Info.Behaviour |= PXECSInfoActive;
    pxFieldEffect->Info.Behaviour = pxFieldEffectCreateInfo->Behaviour;
    // pxHitBox->Model = pxHitboxCreateInfo->HitBox.Model;

    return PXResultOK;
}

PXResult PXAPI PXFieldEffectRelease(PXFieldEffect PXREF pxFieldEffect)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXFieldEffectDraw(PXFieldEffect PXREF pxFieldEffect, PXDrawInfo PXREF pxDrawInfo)
{
    return PXActionRefusedNotImplemented;
}