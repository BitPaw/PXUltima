#pragma once

#ifndef PXSalsa20Included
#define PXSalsa20Included

#include <PX/Engine/PXResource.h>

typedef struct PXSalsa20_
{
    char* KeyData;
    PXSize KeyLength;

    int DataSectionIndex;
    PXI32U DataSize;

    char IV[8];
}
PXSalsa20;

PXPublic PXResult PXAPI PXSalsa20Encrypt(PXSalsa20 PXREF pxSalsa20, PXFile PXREF input, PXFile PXREF output);
PXPublic PXResult PXAPI PXSalsa20Decrypt(PXSalsa20 PXREF pxSalsa20, PXFile PXREF input, PXFile PXREF output);

#endif
