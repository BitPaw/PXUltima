#include "PXHitBox.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>
#include <PX/OS/PXOS.h>

const char PXHitBoxText[] = "HitBox";

PXResult PXAPI PXHitBoxCreate(PXHitBox** pxHitBoxREF, PXHitboxCreateInfo PXREF pxHitboxCreateInfo)
{
    PXHitBox* pxHitBox = PXMemoryHeapCallocT(PXHitBox, 1);

    *pxHitBoxREF = pxHitBox;

    pxHitBox->Info.Behaviour |= PXECSInfoActive;
    pxHitBox->Info.Behaviour = pxHitboxCreateInfo->Behaviour;
   // pxHitBox->Model = pxHitboxCreateInfo->HitBox.Model;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXHitBoxText,
        "Create",
        "PXID:%i",
        pxHitBox->Info.ID
    );
#endif

    return PXResultOK;
}
