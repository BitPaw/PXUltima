#include "PXAVI.h"

#include <Media/RIFF/PXRIFF.h>

PXActionResult PXAVIParse(PXAVI* const avi, PXFile* const PXFile)
{
    PXRIFF riff;

    // PXRIFF
    {
        const PXActionResult riffResult = PXRIFFParse(&riff, PXFile);

        PXActionExitOnError(riffResult);

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