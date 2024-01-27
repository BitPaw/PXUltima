#ifndef PXZLIBINCLUDE
#define PXZLIBINCLUDE

#include "../PXResource.h"
#include <Media/PNG/PXPNG.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXPXZLIBCompressionLevel_
	{
		PXZLIBCompressionLevelInvalid,
		PXZLIBCompressionLevelDefault,
		PXZLIBCompressionLevelSlowest,
		PXZLIBCompressionLevelFast,
		PXZLIBCompressionLevelFastest
	}
	PXZLIBCompressionLevel;

	typedef enum PXZLIBCompressionMethod_
	{
		PXZLIBCompressionMethodInvalid,
		PXZLIBCompressionMethodDeflate,
		PXZLIBCompressionMethodReserved
	}
	PXZLIBCompressionMethod;

	typedef struct PXZLIBHeader_
	{
		PXZLIBCompressionMethod CompressionMethod; // 4 Bits


		/*
		   4 Bits

		For CM = 8, CINFO is the base-2 logarithm of the LZ77 window size, minus eight
		CINFO = 7 indicates a 32K window size).
		Values of CINFO above 7 are not allowed in this version of the specification.
		CINFO is not defined in this specification for CM not equal to 8.
		*/
		unsigned char CompressionInfo;

		PXSize WindowSize;
		PXInt8U CheckFlag; // 5 Bits
		PXBool DictionaryPresent; // 1 Bit
		PXZLIBCompressionLevel CompressionLevel; // 2 Bits
	}
	PXZLIBHeader;

	typedef struct PXZLIB_
	{
		PXZLIBHeader Header;

		PXSize CompressedDataSize;
		void* CompressedData;

		PXInt32U AdlerChecksum;
	}
	PXZLIB;

	PXPrivate inline PXZLIBCompressionLevel PXZLIBCompressionLevelFromID(const PXInt8U compressionLevel);
	PXPrivate inline PXInt8U PXZLIBCompressionLevelToID(const PXZLIBCompressionLevel compressionLevel);

	PXPrivate inline PXZLIBCompressionMethod PXZLIBCompressionMethodFromID(const PXInt8U compressionMethod);
	PXPrivate inline PXInt8U PXZLIBCompressionMethodToID(const PXZLIBCompressionMethod compressionMethod);

	PXPublic PXActionResult PXZLIBDecompress(PXFile* const pxInputSteam, PXFile* const pxOutputSteam);
	PXPublic PXActionResult PXZLIBCompress(PXFile* const pxInputSteam, PXFile* const pxOutputSteam);


	PXPublic PXSize PXZLIBCalculateExpectedSize(const PXSize width, const PXSize height, const PXSize bpp, const PXPNGInterlaceMethod interlaceMethod);

	/*in an idat chunk, each scanline is a multiple of 8 bits, unlike the lodepng output buffer,
and in addition has one extra byte per line: the filter byte. So this gives a larger
result than lodepng_get_raw_size. Set h to 1 to get the size of 1 row including filter byte. */
	PXPublic PXSize PXZLIBCalculateRawSizeIDAT(const PXSize w, const PXSize h, const PXSize bpp);

#ifdef __cplusplus
}
#endif

#endif