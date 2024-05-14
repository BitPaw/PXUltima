#ifndef PXSalsa20INCLUDE
#define PXSalsa20INCLUDE

#include "../PXResource.h"

typedef struct PXSalsa20_
{
	char* KeyData;
	PXSize KeyLength;

	int DataSectionIndex;
	PXInt32U DataSize;

	char IV[8];
}
PXSalsa20;

PXPublic PXActionResult PXAPI PXSalsa20Encrypt(PXSalsa20* const pxSalsa20, PXFile* const input, PXFile* const output);
PXPublic PXActionResult PXAPI PXSalsa20Decrypt(PXSalsa20* const pxSalsa20, PXFile* const input, PXFile* const output);

#endif