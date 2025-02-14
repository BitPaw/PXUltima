#ifndef PXDeflateINCLUDE
#define PXDeflateINCLUDE

#include "../PXResource.h"

#define PXDeflateEncodingLiteralRaw     0b00
#define PXDeflateEncodingHuffmanStatic  0b01
#define PXDeflateEncodingHuffmanDynamic 0b10
#define PXDeflateEncodingReserverd      0b11

typedef struct PXDeflateBlock_
{
    PXInt8U IsLastBlock;
    PXInt8U EncodingMethod;
}
PXDeflateBlock;

PXPublic PXActionResult PXAPI PXDEFLATEParse(PXFile* const pxInputStream, PXFile* const pxOutputStream);
PXPublic PXActionResult PXAPI PXDEFLATESerialize(PXFile* const pxInputStream, PXFile* const pxOutputStream);

#endif
