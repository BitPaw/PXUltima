#include "PXAVI.h"

#include <PX/Media/RIFF/PXRIFF.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

PXResult PXAPI PXAVILoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXRIFF riff;

    // RIFF
    {
        const PXResult riffResult = PXRIFFLoadFromFile(&riff, pxResourceLoadInfo->FileReference);

        if(PXActionSuccessful != riffResult)
            return riffResult;

        // Valid RIFF
        {
            const PXBool isAVI = PXRIFFAudioVideoInterleave == riff.Format;

            if (!isAVI)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXAVISaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
