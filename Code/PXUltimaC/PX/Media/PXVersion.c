#include "PXVersion.h"

#include <PX/Media/PXText.h>

PXResult PXAPI PXVersionFromString(PXVersion PXREF pxVersion, char* versioNString)
{
    int versionMajor = 0;
    int versionMinor = 0;
    int versionBuild = 0;
    int versionPatch = 0;

    PXText pxTextVersion;
    PXTextFromAdressA(&pxTextVersion, versioNString, 0, PXTextUnkownLength);

    PXSize offset = 0;

    offset = PXTextToInt(&pxTextVersion, &versionMajor);

    if(offset != -1)
    {
        PXAdvance(&pxTextVersion, offset + 1u); // dot
        offset = PXTextToInt(&pxTextVersion, &versionMinor);

        if(offset != -1)
        {
            PXAdvance(&pxTextVersion, offset + 1u); // dot
            PXTextToInt(&pxTextVersion, &versionBuild);
        }
    }

    pxVersion->Major = versionMajor;
    pxVersion->Minor = versionMinor;
    pxVersion->Build = versionBuild;
    pxVersion->Patch = versionPatch;

    return PXResultOK;
}

PXResult PXAPI PXVersionToString(PXVersion PXREF pxVersion, char* versioNString)
{
    PXTextPrintA(versioNString, 64, "%i.%i.%i", pxVersion->Major, pxVersion->Minor, pxVersion->Build);

    return PXResultOK;
}
