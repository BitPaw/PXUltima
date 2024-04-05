#ifndef PXDeflateINCLUDE
#define PXDeflateINCLUDE

#include "../PXResource.h"

typedef enum PXDeflateEncodingMethod_
{
	PXDeflateEncodingInvalid,
	PXDeflateEncodingLiteralRaw,
	PXDeflateEncodingHuffmanStatic,
	PXDeflateEncodingHuffmanDynamic,
	PXDeflateEncodingReserverd
}
PXDeflateEncodingMethod;

typedef struct PXDeflateBlock_
{
	PXDeflateEncodingMethod EncodingMethod;
	PXBool IsLastBlock;
}
PXDeflateBlock;

PXPublic PXDeflateEncodingMethod PXAPI PXDeflateEncodingMethodFromID(const PXInt8U deflateEncodingMethod);
PXPublic PXInt8U PXAPI PXDeflateEncodingMethodToID(const PXDeflateEncodingMethod deflateEncodingMethod);

PXPublic PXActionResult PXAPI PXDEFLATEParse(PXFile* const pxInputStream, PXFile* const pxOutputStream);
PXPublic PXActionResult PXAPI PXDEFLATESerialize(PXFile* const pxInputStream, PXFile* const pxOutputStream);

#endif