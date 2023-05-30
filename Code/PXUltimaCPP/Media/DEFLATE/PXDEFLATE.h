#ifndef PXDeflateINCLUDE
#define PXDeflateINCLUDE

#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
		PXBool IsLastBlock;
		PXDeflateEncodingMethod EncodingMethod;
	}
	PXDeflateBlock;

	PXPublic PXDeflateEncodingMethod PXDeflateEncodingMethodFromID(const PXInt8U deflateEncodingMethod);
	PXPublic PXInt8U PXDeflateEncodingMethodToID(const PXDeflateEncodingMethod deflateEncodingMethod);

	PXPublic PXActionResult PXDEFLATEParse(PXFile* const pxInputStream, PXFile* const pxOutputStream);
	PXPublic PXActionResult PXDEFLATESerialize(PXFile* const pxInputStream, PXFile* const pxOutputStream);

#ifdef __cplusplus
}
#endif

#endif