#include "PXPDF.h"

PXActionResult PXPDFLoadFromFile(PXPDF* const pxPDF, PXFile* const pxFile)
{
    // Check for header
    {
        const char signature[5] = { '%', 'P', 'D', 'F', '-' };
        const PXBool validSignature = PXFileReadAndCompare(pxFile, signature, sizeof(signature));

        if (!validSignature)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    // Fetch Versionc
    {
        PXFileReadTextIU8(pxFile, &pxPDF->VersionMajor);
        PXFileCursorAdvance(pxFile, 1u); // Move over the dot
        PXFileReadTextIU8(pxFile, &pxPDF->VersionMinor);
        PXFileSkipEndOfLineCharacters(pxFile);
    }


    return PXActionSuccessful;
}

PXActionResult PXPDFSaveToFile(PXPDF* const pxPDF, PXFile* const pxFile)
{
    return PXActionRefusedNotImplemented;
}