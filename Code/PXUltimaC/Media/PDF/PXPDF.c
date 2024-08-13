#include "PXPDF.h"

#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

const static char PXPDFSignature[5] = {'%', 'P', 'D', 'F', '-'};

PXActionResult PXAPI PXPDFLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXPDF pxPDF;

    // Check for header
    {
        const PXBool validSignature = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXPDFSignature, sizeof(PXPDFSignature));

        if (!validSignature)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    // Fetch Versionc
    {
        PXFileReadTextIU8(pxResourceLoadInfo->FileReference, &pxPDF.VersionMajor);
        PXFileCursorAdvance(pxResourceLoadInfo->FileReference, 1u); // Move over the dot
        PXFileReadTextIU8(pxResourceLoadInfo->FileReference, &pxPDF.VersionMinor);
        PXFileSkipEndOfLineCharacters(pxResourceLoadInfo->FileReference);
    }


    return PXActionSuccessful;
}

PXActionResult PXAPI PXPDFSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXFileWriteB(pxResourceSaveInfo->FileReference, PXPDFSignature, sizeof(PXPDFSignature));

    return PXActionRefusedNotImplemented;
}