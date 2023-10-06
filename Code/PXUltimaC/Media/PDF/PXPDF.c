#include "PXPDF.h"

const static char PXPDFSignature[5] = {'%', 'P', 'D', 'F', '-'};

PXActionResult PXAPI PXPDFLoadFromFile(PXPDF* const pxPDF, PXFile* const pxFile)
{
    // Check for header
    {
        const PXBool validSignature = PXFileReadAndCompare(pxFile, PXPDFSignature, sizeof(PXPDFSignature));

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

PXActionResult PXAPI PXPDFSaveToFile(PXPDF* const pxPDF, PXFile* const pxFile)
{
    PXFileWriteB(pxFile, PXPDFSignature, sizeof(PXPDFSignature));

    return PXActionRefusedNotImplemented;
}