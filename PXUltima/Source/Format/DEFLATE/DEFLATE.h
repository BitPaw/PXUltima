#ifndef DeflateInclude
#define DeflateInclude

#include <Format/Type.h>

#include <Error/PXActionResult.h>

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

	PXPublic int DEFLATEParse(const void* const inputBuffer, const PXSize inputBufferSize, void* const outputBuffer, const PXSize outputBufferSize, PXSize* const outputBufferSizeRead);
	PXPublic int DEFLATESerialize(const void* const inputBuffer, const PXSize inputBufferSize, void* const outputBuffer, const PXSize outputBufferSize, PXSize* const outputBufferSizeWritten);

#ifdef __cplusplus
}
#endif

#endif