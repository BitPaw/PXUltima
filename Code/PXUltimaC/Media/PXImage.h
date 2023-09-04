#ifndef PXImageINCLUDE
#define PXImageINCLUDE

#include <OS/File/PXFile.h>
#include <Media/PXColor.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXImage_
	{
		void* PixelData;
		PXSize PixelDataSize;

		PXSize Width;
		PXSize Height;	

		PXColorFormat Format;
	}
	PXImage;

	typedef PXActionResult(*PXImageTranslateFunction)(const PXImage* const pxImage, PXFile* const imageDataStream);

	PXPublic void PXImageConstruct(PXImage* const image);
	PXPublic void PXImageDestruct(PXImage* const image);

	PXPublic void PXImageCopyAsIs(PXImage* const pxImageTarget, const PXImage* const pxImageSource);
	PXPublic void PXImageCopyAsNew(PXImage* const pxImageTarget, const PXImage* const pxImageSource);


	PXPublic PXBool PXImageResize(PXImage* const image, const PXColorFormat format, const PXSize width, const PXSize height);
	PXPublic void PXImageFlipHorizontal(PXImage* image);
	PXPublic void PXImageFlipVertical(PXImage* image);
	PXPublic void PXImageRemoveColor(PXImage* image, unsigned char red, unsigned char green, unsigned char blue);
	PXPublic void PXImageFillColorRGBA8(PXImage* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);

	PXPublic void* PXImageDataPoint(const PXImage* const image, const PXSize x, const PXSize y);

	PXPublic PXSize PXImagePixelPosition
	(
		const PXImage* const image,
		const PXSize x,
		const PXSize y
	);
	PXPublic void PXImagePixelSetRGB8
	(
		PXImage* const image,
		const PXSize x,
		const PXSize y,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue
	);

#ifdef __cplusplus
}
#endif

#endif