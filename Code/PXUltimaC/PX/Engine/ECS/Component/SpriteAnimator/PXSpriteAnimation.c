#include "PXSpriteAnimation.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

const char PXSpriteAnimatorText[] = "SpriteAnimator";

PXResult PXAPI PXSpriteAnimatorCreate(PXSpriteAnimator PXREF pxSpriteAnimator, PXSpriteAnimatorInfo PXREF pxSpriteAnimatorInfo)
{
    pxSpriteAnimator->Info.Behaviour |= PXECSInfoActive;

    pxSpriteAnimator->Info.Behaviour = pxSpriteAnimatorInfo->Behaviour;
    pxSpriteAnimator->SpriteTarget = pxSpriteAnimatorInfo->SpriteTarget;
    pxSpriteAnimator->RateUpdate = pxSpriteAnimatorInfo->UpdateRate;
    pxSpriteAnimator->TimeStampAmount = pxSpriteAnimatorInfo->TimeStampAmount;

    pxSpriteAnimator->TimeStampList = PXMemoryHeapCallocT(PXSpriteAnimatorTimeStamp, pxSpriteAnimatorInfo->TimeStampAmount);
    PXCopyList(PXSpriteAnimatorTimeStamp, pxSpriteAnimatorInfo->TimeStampAmount, pxSpriteAnimatorInfo->TimeStampList, pxSpriteAnimator->TimeStampList);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSpriteAnimatorText,
        "Create",
        "SpriteAnimator"
    );
#endif


    return PXResultOK;
}
