#include "PXWEBP.h"

#include <PX/Media/RIFF/PXRIFF.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

const char PXWEBPVP[4] = "VP8 ";

PXResult PXAPI PXWEBPLoadFromFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    // Parse RIFF header
    {
        PXRIFF pxRiff;

        const PXResult riffResult = PXRIFFLoadFromFile(&pxRiff, pxFile);

        if(PXResultOK != riffResult) 
            return riffResult;

        if (PXRIFFWebPicture != pxRiff.Format)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        PXFileEndianessSet(pxFile, pxRiff.EndianFormat);
    }

    // Expect "VP8 " Chunk

    while (!PXFileIsAtEnd(pxFile))
    {
        PXI32UCluster chunkID;

        PXFileReadB(pxFile, chunkID.Data, 4u);

        switch (chunkID.Value)
        {
        case PXI32Make('V', 'P', '8', 'L'):
        {
            PXI32U streamSize = 0;
            PXFileReadI32UE(pxFile, &streamSize, PXEndianLittle);

            const char signature = 0x2f;
            const PXBool isValid = PXFileReadAndCompare(pxFile, &signature, sizeof(char));

            // Images can only be 16384x16384
            pxTexture->Width = PXFileReadBits(pxFile, 14u) + 1u;
            pxTexture->Height = PXFileReadBits(pxFile, 14u) + 1u;

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

PXResult PXAPI PXWEBPSaveToFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    // Write RIFF
    {
        PXRIFF pxRiff;
        pxRiff.ChunkSize = 0;
        pxRiff.Format = PXRIFFWebPicture;
        pxRiff.EndianFormat = PXEndianLittle;

        const PXResult riffResult = PXRIFFSaveToFile(&pxRiff, pxResourceSaveInfo->FileCurrent);
    }

    return PXActionRefusedNotImplemented;
}
