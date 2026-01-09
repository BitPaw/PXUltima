#include "PXSound.h"

#include <PX/OS/Console/PXConsole.h>

const char PXSoundText[] = "Sound";

PXResult PXAPI PXSoundCreate(PXSound PXREF pxSound, PXSoundCreateInfo PXREF pxSoundCreateInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSoundText,
        "Create",
        "PXID:%i, %s",
        pxSound->Info.ID,
        pxSoundCreateInfo->Info.FilePath.A
    );
#endif

    PXResourceMoveInfo pxResourceLoadInfo;
    PXClear(PXResourceMoveInfo, &pxResourceLoadInfo);
    pxResourceLoadInfo.ResourceTarget = pxSound;
    pxResourceLoadInfo.ResourceType = PXResourceTypeSound;

    const PXResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxSoundCreateInfo->Info.FilePath);

    if(PXResultOK != loadResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXSoundText,
            "Load",
            "failed <%s>!",
            pxSoundCreateInfo->Info.FilePath.A
        );
#endif

        return loadResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSoundText,
        "Load",
        "successful <%s>.",
        pxSoundCreateInfo->Info.FilePath.A
    );
#endif

    return PXResultOK;
}