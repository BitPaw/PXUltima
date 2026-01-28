#include "PXRAR.h"

#include <PX/OS/File/PXFile.h>

const char PXRARSignature[] = { 'R', 'a', 'r', '!', 0x1A,0x07 };

PXResult PXAPI PXRARLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    PXRAR pxRAR;
    PXClear(PXRAR, &pxRAR);

    // Read Signature
    {
        const PXBool isSignature = PXFileReadAndCompare(pxFile, PXRARSignature, sizeof(PXRARSignature));

        if (!isSignature)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        char checkA = 0;

        PXFileReadB(pxFile, &checkA, 1u);

        switch (checkA)
        {
        case 0: // OK
        {
            pxRAR.Version = PXRARVersion1x5;
            break;
        }
        case 1:
        {
            const char checkB[2] = { 1,0 };

            const PXBool isSignatureB = PXFileReadAndCompare(pxFile, checkB, 2u);

            if (!isSignatureB)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }

            pxRAR.Version = PXRARVersion5x0;

            break;
        }

        default:
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXRARSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
