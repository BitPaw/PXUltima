#ifndef PXImageINCLUDE
#define PXImageINCLUDE

#include <Media/PXColor.h>

typedef struct PXImage_
{
	void* PixelData;
	PXSize PixelDataSize;

	PXSize Width;
	PXSize Height;
	PXSize Depth;

	PXColorFormat Format;
}
PXImage;

PXPublic void PXAPI PXImageConstruct(PXImage* const image);
PXPublic void PXAPI PXImageDestruct(PXImage* const image);

PXPublic void PXAPI PXImageCopyAsIs(PXImage* const pxImageTarget, const PXImage* const pxImageSource);
PXPublic void PXAPI PXImageCopyAsNew(PXImage* const pxImageTarget, const PXImage* const pxImageSource);


PXPublic PXBool PXAPI PXImageResize(PXImage* const image, const PXColorFormat format, const PXSize width, const PXSize height);
PXPublic void PXAPI PXImageFlipHorizontal(PXImage* image);
PXPublic void PXAPI PXImageFlipVertical(PXImage* image);
PXPublic void PXAPI PXImageRemoveColor(PXImage* image, unsigned char red, unsigned char green, unsigned char blue);
PXPublic void PXAPI PXImageFillColorRGBA8(PXImage* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);

PXPublic void* PXAPI PXImageDataPoint(const PXImage* const image, const PXSize x, const PXSize y);

PXPublic PXSize PXAPI PXImagePixelPosition
(
	const PXImage* const image,
	const PXSize x,
	const PXSize y
);
PXPublic void PXAPI PXImagePixelSetRGB8
(
	PXImage* const image,
	const PXSize x,
	const PXSize y,
	const unsigned char red,
	const unsigned char green,
	const unsigned char blue
);

#endif