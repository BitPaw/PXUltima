#include "PXPDF.h"

ActionResult PXPDFCompile(PXPDF* const pxPDF, PXDataStream* const fileStream)
{
    // Check for header
    {
        const char signature[5] = { '%', 'P', 'D', 'F', '-' };
        const PXBool validSignature = PXDataStreamReadAndCompare(fileStream, signature, sizeof(signature));

        if (!validSignature)
        {
            return ActionInvalidHeaderSignature;
        }
    }

    // Fetch Versionc
    {
        PXDataStreamReadTextIU8(fileStream, &pxPDF->VersionMajor);
        PXDataStreamCursorAdvance(fileStream, 1u); // Move over the dot
        PXDataStreamReadTextIU8(fileStream, &pxPDF->VersionMinor);
        PXDataStreamSkipEndOfLineCharacters(fileStream);
    }


    return ActionSuccessful;
}
