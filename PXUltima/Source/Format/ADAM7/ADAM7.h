#ifndef ADAM7Include
#define ADAM7Include

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <Format/PNG/PNG.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic unsigned char ADAM7paethPredictor(short a, short b, short c);
	PXPublic unsigned ADAM7unfilterScanline(unsigned char* recon, const unsigned char* scanline, const unsigned char* precon, size_t bytewidth, unsigned char filterType, size_t length);
	PXPublic size_t ADAM7lodepng_get_raw_size_idat(size_t w, size_t h, size_t bpp);
	PXPublic unsigned ADAM7unfilter(unsigned char* out, const unsigned char* in, size_t w, size_t h, size_t bpp);
	PXPublic void ADAM7removePaddingBits(unsigned char* out, const unsigned char* in, size_t olinebits, size_t ilinebits, size_t h);
	PXPublic unsigned char ADAM7readBitFromReversedStream(size_t* bitpointer, const unsigned char* bitstream);
	PXPublic void ADAM7setBitOfReversedStream(size_t* bitpointer, unsigned char* bitstream, unsigned char bit);
	PXPublic void ADAM7_getpassvalues(unsigned passw[7], unsigned passh[7], size_t filter_passstart[8], size_t padded_passstart[8], size_t passstart[8], size_t w, size_t h, size_t bpp);
	PXPublic void ADAM7_deinterlace(unsigned char* out, const unsigned char* in, size_t w, unsigned h, unsigned bpp);




	PXPublic unsigned int ADAM7ScanlinesDecode
	(
		unsigned char* out,
		unsigned char* in,
		size_t width,
		size_t height,
		size_t bbp,
		PNGInterlaceMethod interlaceMethod
	);

	PXPublic unsigned int ADAM7ScanlinesEncode
	(
		unsigned char* out,
		unsigned char* in,
		size_t width,
		size_t height,
		size_t bbp,
		PNGInterlaceMethod interlaceMethod
	);


	PXPublic size_t ADAM7CaluclateExpectedSize(size_t width, size_t height, size_t bpp);


#ifdef __cplusplus
}
#endif

#endif