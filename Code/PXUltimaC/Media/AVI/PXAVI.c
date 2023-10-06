#include "PXAVI.h"

#include <Media/RIFF/PXRIFF.h>

PXActionResult PXAPI PXAVILoadFromFile(PXVideo* const pxVideo, PXFile* const pxFile)
{
    PXRIFF riff;

    // PXRIFF
    {
        const PXActionResult riffResult = PXRIFFParse(&riff, pxFile);

        PXActionReturnOnError(riffResult);

        // Valid PXRIFF
        {
            const PXBool isAVI = riff.Format == PXRIFFAudioVideoInterleave;

            if (!isAVI)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXAVISaveToFile(PXVideo* const pxVideo, PXFile* const pxFile)
{
    return PXActionRefusedNotImplemented;
}