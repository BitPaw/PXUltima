#include "PXWEBP.h"

#include <PX/Media/RIFF/PXRIFF.h>
#include <PX/OS/File/PXFile.h>

const char PXWEBPVP[4] = "VP8 ";

PXResult PXAPI PXWEBPLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo)
{
    PXTexture PXREF pxTexture = (PXTexture*)pxResourceLoadInfo->ResourceTarget;

    // Parse RIFF header
    {
        PXRIFF pxRiff;

        const PXResult riffResult = PXRIFFLoadFromFile(&pxRiff, pxResourceLoadInfo->FileReference);

        if(PXActionSuccessful != riffResult) 
            return riffResult;

        if (PXRIFFWebPicture != pxRiff.Format)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        pxResourceLoadInfo->FileReference->EndiannessOfData = pxRiff.EndianFormat;
    }

    // Expect "VP8 " Chunk

    while (!PXFileIsAtEnd(pxResourceLoadInfo->FileReference))
    {
        PXI32UCluster chunkID;

        PXFileReadB(pxResourceLoadInfo->FileReference, chunkID.Data, 4u);

        switch (chunkID.Value)
        {
        case PXI32Make('V', 'P', '8', 'L'):
        {
            PXI32U streamSize = 0;
            PXFileReadI32UE(pxResourceLoadInfo->FileReference, &streamSize, PXEndianLittle);

            const char signature = 0x2f;
            const PXBool isValid = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, &signature, sizeof(char));

            // Images can only be 16384x16384
            pxTexture->Width = PXFileReadBits(pxResourceLoadInfo->FileReference, 14u) + 1u;
            pxTexture->Height = PXFileReadBits(pxResourceLoadInfo->FileReference, 14u) + 1u;

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

PXResult PXAPI PXWEBPSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo)
{
    // Write RIFF
    {
        PXRIFF pxRiff;
        pxRiff.ChunkSize = 0;
        pxRiff.Format = PXRIFFWebPicture;
        pxRiff.EndianFormat = PXEndianLittle;

        const PXResult riffResult = PXRIFFSaveToFile(&pxRiff, pxResourceSaveInfo->FileReference);
    }

    return PXActionRefusedNotImplemented;
}
