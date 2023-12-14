#include "PXWEBP.h"

#include <Media/RIFF/PXRIFF.h>

const char PXWEBPVP[4] = { 'V', 'P', '8', ' ' };

PXActionResult PXAPI PXWEBPLoadFromFile(PXImage* const pxImage, PXFile* const pxFile)
{
    // Parse RIFF header
    {
        PXRIFF pxRiff;

        const PXActionResult riffResult = PXRIFFLoadFromFile(&pxRiff, pxFile);

        PXActionReturnOnError(riffResult);

        if (PXRIFFWebPicture != pxRiff.Format)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        pxFile->EndiannessOfData = pxRiff.EndianFormat;
    }

    // Expect "VP8 " Chunk

    while (!PXFileIsAtEnd(pxFile))
    {
        PXInt32UCluster chunkID;

        PXFileReadB(pxFile, chunkID.Data, 4u);

        switch (chunkID.Value)
        {
            case PXInt32Make('V', 'P', '8', 'L'):
            {
                PXInt32U streamSize = 0;
                PXFileReadI32UE(pxFile, &streamSize, PXEndianLittle);

                const char signature = 0x2f;
                const PXBool isValid = PXFileReadAndCompare(pxFile, &signature, sizeof(char));

                // Images can only be 16384x16384 
                pxImage->Width = PXFileReadBits(pxFile, 14u) + 1u;
                pxImage->Height = PXFileReadBits(pxFile, 14u) + 1u;

                PXBool hasAlpha = PXFileReadBits(pxFile, 1u);
                PXBool version = PXFileReadBits(pxFile, 3u);

                break;
            }
            default:
                break;
        }

    }


    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXWEBPSaveToFile(PXImage* const pxImage, PXFile* const pxFile)
{
    // Write RIFF
    {
        PXRIFF pxRiff;
        pxRiff.ChunkSize = 0;
        pxRiff.Format = PXRIFFWebPicture;
        pxRiff.EndianFormat = 0;

        const PXActionResult riffResult = PXRIFFSaveToFile(&pxRiff, pxFile);
    }

    return PXActionRefusedNotImplemented;
}