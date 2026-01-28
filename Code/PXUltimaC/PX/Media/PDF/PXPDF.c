#include "PXPDF.h"

#include <PX/OS/Error/PXResult.h>
#include <PX/OS/File/PXFile.h>

const char PXPDFSignature[5] = "%PDF-";

PXResult PXAPI PXPDFLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXPDF pxPDF;
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

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
        PXFileReadTextIU8(pxFile, &pxPDF.VersionMajor);
        PXFileCursorAdvance(pxFile, 1u); // Move over the dot
        PXFileReadTextIU8(pxFile, &pxPDF.VersionMinor);
        PXFileSkipEndOfLineCharacters(pxFile);
    }

    return PXResultOK;
}

PXResult PXAPI PXPDFSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    PXFile* pxFile = pxResourceSaveInfo->FileCurrent;

    PXFileWriteB(pxFile, PXPDFSignature, sizeof(PXPDFSignature));

    return PXActionRefusedNotImplemented;
}
