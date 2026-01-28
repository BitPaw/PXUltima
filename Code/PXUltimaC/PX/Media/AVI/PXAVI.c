#include "PXAVI.h"

#include <PX/Media/RIFF/PXRIFF.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

PXResult PXAPI PXAVILoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    if(!pxResourceLoadInfo)
    {
        return PXResultRefusedParameterNull;
    }

    PXRIFF riff;

    // RIFF
    {
        const PXResult riffResult = PXRIFFLoadFromFile(&riff, pxResourceLoadInfo->FileCurrent);

        if(PXResultOK != riffResult)
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

    return PXResultOK;
}

PXResult PXAPI PXAVISaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
