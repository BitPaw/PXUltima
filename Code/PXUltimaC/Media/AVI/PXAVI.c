#include "PXAVI.h"

#include <Media/RIFF/PXRIFF.h>
#include <OS/File/PXFile.h>

PXActionResult PXAPI PXAVILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
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

PXActionResult PXAPI PXAVISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}