#include "PXPDF.h"

ActionResult PXPDFCompile(PXPDF* const pxPDF, DataStream* const fileStream)
{
    // Check for header
    {
        const char signature[5] = { '%', 'P', 'D', 'F', '-' };
        const PXBool validSignature = DataStreamReadAndCompare(fileStream, signature, sizeof(signature));

        if (!validSignature)
        {
            return ActionInvalidHeaderSignature;
        }
    }

    // Fetch Versionc
    {
        DataStreamReadTextIU8(fileStream, &pxPDF->VersionMajor);
        DataStreamCursorAdvance(fileStream, 1u); // Move over the dot
        DataStreamReadTextIU8(fileStream, &pxPDF->VersionMinor);
        DataStreamSkipEndOfLineCharacters(fileStream);
    }


    return ActionSuccessful;
}
