#include "AVI.h"

#include <Media/RIFF/RIFF.h>

PXActionResult AVIParse(AVI* const avi, PXDataStream* const pxDataStream)
{
    RIFF riff;

    // RIFF
    {
        const PXActionResult riffResult = RIFFParse(&riff, pxDataStream);

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