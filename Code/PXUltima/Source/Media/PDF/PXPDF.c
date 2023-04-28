#include "PXPDF.h"

PXActionResult PXPDFCompile(PXPDF* const pxPDF, PXFile* const fileStream)
{
    // Check for header
    {
        const char signature[5] = { '%', 'P', 'D', 'F', '-' };
        const PXBool validSignature = PXFileReadAndCompare(fileStream, signature, sizeof(signature));

        if (!validSignature)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    // Fetch Versionc
    {
        PXFileReadTextIU8(fileStream, &pxPDF->VersionMajor);
        PXFileCursorAdvance(fileStream, 1u); // Move over the dot
        PXFileReadTextIU8(fileStream, &pxPDF->VersionMinor);
        PXFileSkipEndOfLineCharacters(fileStream);
    }


    return PXActionSuccessful;
}
