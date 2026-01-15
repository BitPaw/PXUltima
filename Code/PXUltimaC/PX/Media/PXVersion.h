#pragma once
#include "PXType.h"
#include <PX/OS/Error/PXResult.h>

// Version holder
typedef struct PXVersion_
{
    PXI8U Major;
    PXI8U Minor;
    PXI8U Build;
    PXI8U Patch;
}
PXVersion;

PXPublic PXResult PXAPI PXVersionFromString(PXVersion PXREF pxVersion, const char* versioNString);
PXPublic PXResult PXAPI PXVersionToString(PXVersion PXREF pxVersion, char* versioNString);



