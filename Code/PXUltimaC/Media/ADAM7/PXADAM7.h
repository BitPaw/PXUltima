#ifndef PXADAM7Include
#define PXADAM7Include

#include "../PXResource.h"

#include <Media/PNG/PXPNG.h>

typedef struct PXADAM7_
{
	char* DataInput;
	PXSize InputSize;

	char* DataOutput;
	PXSize OutputSize;

	PXSize Width;
	PXSize Height;
	PXSize BitsPerPixel;

	PXPNGInterlaceMethod InterlaceMethod;
}
PXADAM7;

PXPublic unsigned char PXAPI PXADAM7paethPredictor(short a, short b, short c);
PXPublic void PXAPI PXADAM7removePaddingBits(unsigned char* out, const unsigned char* in, PXSize olinebits, PXSize ilinebits, PXSize h);
PXPublic unsigned char PXAPI PXADAM7readBitFromReversedStream(PXSize* bitpointer, const unsigned char* bitstream);
PXPublic void PXAPI PXADAM7setBitOfReversedStream(PXSize* bitpointer, unsigned char* bitstream, unsigned char bit);
PXPublic void PXAPI PXADAM7_getpassvalues(unsigned passw[7], unsigned passh[7], PXSize filter_passstart[8], PXSize padded_passstart[8], PXSize passstart[8], PXSize w, PXSize h, PXSize bpp);


PXPublic PXActionResult PXAPI PXADAM7ScanlinesDecode(PXADAM7* const pxADAM7);
PXPublic PXActionResult PXAPI PXADAM7ScanlinesEncode(PXADAM7* const pxADAM7);

PXPublic PXSize PXAPI PXADAM7CaluclateExpectedSize(PXADAM7* const pxADAM7);
PXPublic PXSize PXAPI PXADAM7lodepng_get_raw_size_idat(PXSize w, PXSize h, PXSize bpp);

PXPublic void PXAPI PXADAM7Deinterlace(PXADAM7* const pxADAM7);

PXPublic PXActionResult PXAPI PXADAM7unfilter(PXADAM7* const pxADAM7);
PXPublic PXActionResult PXAPI PXADAM7unfilterScanline(unsigned char* recon, const unsigned char* scanline, const unsigned char* precon, PXSize bytewidth, unsigned char filterType, PXSize length);

#endif