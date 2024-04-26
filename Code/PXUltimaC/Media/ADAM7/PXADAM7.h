#ifndef PXADAM7Include
#define PXADAM7Include

#include "../PXResource.h"

#include <Media/PNG/PXPNG.h>

typedef struct PXADAM7_
{
	void* DataInput;
	PXSize InputSize;

	void* DataOutput;
	PXSize OutputSize;

	PXSize Width;
	PXSize Height;
	PXSize BitsPerPixel;

	PXPNGInterlaceMethod InterlaceMethod;
}
PXADAM7;

PXPublic unsigned char PXAPI PXADAM7paethPredictor(short a, short b, short c);
PXPublic void PXAPI PXADAM7removePaddingBits(void* out, const void* in, PXSize olinebits, PXSize ilinebits, PXSize h);
PXPublic unsigned char PXAPI PXADAM7readBitFromReversedStream(PXSize* bitpointer, const unsigned char* bitstream);
PXPublic void PXAPI PXADAM7setBitOfReversedStream(PXSize* bitpointer, void* bitstream, unsigned char bit);
PXPublic void PXAPI PXADAM7_getpassvalues(const PXADAM7* const pxADAM7, unsigned passw[7], unsigned passh[7], PXSize filter_passstart[8], PXSize padded_passstart[8], PXSize passstart[8]);


PXPublic PXActionResult PXAPI PXADAM7ScanlinesDecode(PXADAM7* const pxADAM7);
PXPublic PXActionResult PXAPI PXADAM7ScanlinesEncode(PXADAM7* const pxADAM7);

PXPublic PXSize PXAPI PXADAM7CaluclateExpectedSize(PXADAM7* const pxADAM7);
PXPublic PXSize PXAPI PXADAM7lodepng_get_raw_size_idat(PXADAM7* const pxADAM7);

PXPublic void PXAPI PXADAM7Deinterlace(PXADAM7* const pxADAM7);

PXPublic PXActionResult PXAPI PXADAM7unfilter(PXADAM7* const pxADAM7);
PXPublic PXActionResult PXAPI PXADAM7unfilterScanline(void* reconXX, void* scanlineXX, void* preconXX, PXSize bytewidth, PXInt8U filterType, PXSize length);

#endif