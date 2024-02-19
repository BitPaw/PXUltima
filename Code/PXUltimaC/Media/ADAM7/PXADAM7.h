#ifndef PXADAM7Include
#define PXADAM7Include

#include "../PXResource.h"

#include <Media/PNG/PXPNG.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic unsigned char PXADAM7paethPredictor(short a, short b, short c);
	PXPublic PXActionResult PXAPI PXADAM7unfilterScanline(void* reconXX, const void* scanlineXX, const void* preconXX, PXSize bytewidth, PXInt8U filterType, PXSize length);
	PXPublic PXSize PXADAM7lodepng_get_raw_size_idat(PXSize w, PXSize h, PXSize bpp);
	PXPublic PXActionResult PXAPI PXADAM7unfilter(void* out, const void* in, PXSize w, PXSize h, PXSize bpp);
	PXPublic void PXADAM7removePaddingBits(void* out, const void* in, PXSize olinebits, PXSize ilinebits, PXSize h);
	PXPublic unsigned char PXADAM7readBitFromReversedStream(PXSize* bitpointer, const unsigned char* bitstream);
	PXPublic void PXADAM7setBitOfReversedStream(PXSize* bitpointer, void* bitstream, unsigned char bit);
	PXPublic void PXADAM7_getpassvalues(unsigned passw[7], unsigned passh[7], PXSize filter_passstart[8], PXSize padded_passstart[8], PXSize passstart[8], PXSize w, PXSize h, PXSize bpp);
	PXPublic void PXADAM7_deinterlace(void* out, const void* in, PXSize w, unsigned h, unsigned bpp);




	PXPublic PXActionResult PXAPI PXADAM7ScanlinesDecode
	(
		void* out,
		void* in,
		PXSize width,
		PXSize height,
		PXSize bbp,
		PXPNGInterlaceMethod interlaceMethod
	);

	PXPublic PXActionResult PXAPI PXADAM7ScanlinesEncode
	(
		void* out,
		void* in,
		PXSize width,
		PXSize height,
		PXSize bbp,
		PXPNGInterlaceMethod interlaceMethod
	);


	PXPublic PXSize PXADAM7CaluclateExpectedSize(PXSize width, PXSize height, PXSize bpp);


#ifdef __cplusplus
}
#endif

#endif