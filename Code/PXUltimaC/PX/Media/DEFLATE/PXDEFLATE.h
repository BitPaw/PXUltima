#ifndef PXDeflateIncluded
#define PXDeflateIncluded

#include <PX/Engine/PXResource.h>

#define PXDeflateEncodingLiteralRaw     0b00
#define PXDeflateEncodingHuffmanStatic  0b01
#define PXDeflateEncodingHuffmanDynamic 0b10
#define PXDeflateEncodingReserverd      0b11

typedef struct PXDeflateBlock_
{
    PXI8U IsLastBlock;
    PXI8U EncodingMethod;
}
PXDeflateBlock;

PXPublic PXResult PXAPI PXDEFLATEParse(PXFile* const pxInputStream, PXFile* const pxOutputStream);
PXPublic PXResult PXAPI PXDEFLATESerialize(PXFile* const pxInputStream, PXFile* const pxOutputStream);

#endif
