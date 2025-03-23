#include "PXAVI.h"

#include <PX/Media/RIFF/PXRIFF.h>
#include <PX/OS/File/PXFile.h>

PXActionResult PXAPI PXAVILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXRIFF riff;

    // RIFF
    {
        const PXActionResult riffResult = PXRIFFLoadFromFile(&riff, pxResourceLoadInfo->FileReference);

        PXActionReturnOnError(riffResult);

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

PXActionResult PXAPI PXAVISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
