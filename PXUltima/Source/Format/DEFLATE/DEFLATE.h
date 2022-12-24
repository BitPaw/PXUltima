#ifndef DeflateInclude
#define DeflateInclude

#include <Format/Type.h>
#include <Error/PXActionResult.h>
#include <File/PXDataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum DeflateEncodingMethod_
	{
		DeflateEncodingInvalid,
		DeflateEncodingLiteralRaw,
		DeflateEncodingHuffmanStatic,
		DeflateEncodingHuffmanDynamic,
		DeflateEncodingReserverd
	}
	DeflateEncodingMethod;

	typedef struct DeflateBlock_
	{
		unsigned char IsLastBlock;
		DeflateEncodingMethod EncodingMethod;
	}
	DeflateBlock;


	PXPublic DeflateEncodingMethod ConvertToDeflateEncodingMethod(const unsigned char deflateEncodingMethod);
	PXPublic unsigned char ConvertFromDeflateEncodingMethod(const DeflateEncodingMethod deflateEncodingMethod);

	PXPublic PXActionResult DEFLATEParse(PXDataStream* const pxInputStream, PXDataStream* const pxOutputStream);
	PXPublic PXActionResult DEFLATESerialize(PXDataStream* const pxInputStream, PXDataStream* const pxOutputStream);

#ifdef __cplusplus
}
#endif

#endif