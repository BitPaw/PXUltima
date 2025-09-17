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

PXPublic PXResult PXAPI PXSalsa20Encrypt(PXSalsa20* const pxSalsa20, PXFile* const input, PXFile* const output);
PXPublic PXResult PXAPI PXSalsa20Decrypt(PXSalsa20* const pxSalsa20, PXFile* const input, PXFile* const output);

#endif
