#include "PXAVI.h"

#include <Media/RIFF/RIFF.h>

PXActionResult PXAVIParse(PXAVI* const avi, PXFile* const PXFile)
{
    RIFF riff;

    // RIFF
    {
        const PXActionResult riffResult = RIFFParse(&riff, PXFile);

        PXActionExitOnError(riffResult);

        // Valid RIFF
        {
            const PXBool isAVI = riff.Format == RIFFAudioVideoInterleave;

            if (!isAVI)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }
    }

    return PXActionSuccessful;
}