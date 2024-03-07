#include "PXWEBP.h"

#include <Media/RIFF/PXRIFF.h>
#include <OS/File/PXFile.h>

const char PXWEBPVP[4] = { 'V', 'P', '8', ' ' };

PXActionResult PXAPI PXWEBPLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceLoadInfo->Target;

    // Parse RIFF header
    {
        PXRIFF pxRiff;

        const PXActionResult riffResult = PXRIFFLoadFromFile(&pxRiff, pxResourceLoadInfo->FileReference);

        PXActionReturnOnError(riffResult);

        if (PXRIFFWebPicture != pxRiff.Format)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        pxResourceLoadInfo->FileReference->EndiannessOfData = pxRiff.EndianFormat;
    }

    // Expect "VP8 " Chunk

    while (!PXFileIsAtEnd(pxResourceLoadInfo->FileReference))
    {
        PXInt32UCluster chunkID;

        PXFileReadB(pxResourceLoadInfo->FileReference, chunkID.Data, 4u);

        switch (chunkID.Value)
        {
            case PXInt32Make('V', 'P', '8', 'L'):
            {
                PXInt32U streamSize = 0;
                PXFileReadI32UE(pxResourceLoadInfo->FileReference, &streamSize, PXEndianLittle);

                const char signature = 0x2f;
                const PXBool isValid = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, &signature, sizeof(char));

                // Images can only be 16384x16384 
                pxImage->Width = PXFileReadBits(pxResourceLoadInfo->FileReference, 14u) + 1u;
                pxImage->Height = PXFileReadBits(pxResourceLoadInfo->FileReference, 14u) + 1u;

                PXBool hasAlpha = PXFileReadBits(pxResourceLoadInfo->FileReference, 1u);
                PXBool version = PXFileReadBits(pxResourceLoadInfo->FileReference, 3u);

                break;
            }
            default:
                break;
        }

    }


    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXWEBPSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
    // Write RIFF
    {
        PXRIFF pxRiff;
        pxRiff.ChunkSize = 0;
        pxRiff.Format = PXRIFFWebPicture;
        pxRiff.EndianFormat = 0;

        const PXActionResult riffResult = PXRIFFSaveToFile(&pxRiff, pxResourceSaveInfo->FileReference);
    }

    return PXActionRefusedNotImplemented;
}