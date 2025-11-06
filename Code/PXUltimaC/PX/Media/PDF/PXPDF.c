#include "PXPDF.h"

#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/File/PXFile.h>

const char PXPDFSignature[5] = "%PDF-";

PXResult PXAPI PXPDFLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo)
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

PXResult PXAPI PXPDFSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo)
{
    PXFileWriteB(pxResourceSaveInfo->FileReference, PXPDFSignature, sizeof(PXPDFSignature));

    return PXActionRefusedNotImplemented;
}
