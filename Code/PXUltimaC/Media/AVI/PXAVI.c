#include "PXAVI.h"

#include <Media/RIFF/PXRIFF.h>
#include <OS/File/PXFile.h>

PXActionResult PXAPI PXAVILoadFromFile(PXVideo* const pxVideo, PXFile* const pxFile)
{
    PXRIFF riff;

    // RIFF
    {
        const PXActionResult riffResult = PXRIFFLoadFromFile(&riff, pxFile);

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

PXActionResult PXAPI PXAVISaveToFile(PXVideo* const pxVideo, PXFile* const pxFile)
{
    return PXActionRefusedNotImplemented;
}