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
		PXSize Width;
		PXSize Height;

		PXSize PixelDataSize;
		void* PixelData;

		PXColorFormat Format;
	}
	PXImage;

	typedef PXActionResult(*PXImageTranslateFunction)(PXImage* const pxImage, PXFile* const imageDataStream);

	PXPublic void PXImageConstruct(PXImage* const image);
	PXPublic void PXImageDestruct(PXImage* const image);

	PXPublic PXActionResult PXImageLoadA(PXImage* const image, const PXTextASCII filePath);
	PXPublic PXActionResult PXImageLoadW(PXImage* const image, const PXTextUNICODE filePath);
	PXPublic PXActionResult PXImageLoadU(PXImage* const image, const PXTextUTF8 filePath);
	PXPublic PXActionResult PXImageLoadD(PXImage* const image, PXFile* const dataStream, const FileFormatExtension guessedFormat);

	PXPublic PXActionResult PXImageSaveA(PXImage* const image, const PXTextASCII filePath, const FileFormatExtension fileFormat, const PXColorFormat pxColorFormat);
	PXPublic PXActionResult PXImageSaveW(PXImage* const image, const PXTextUNICODE filePath, const FileFormatExtension fileFormat, const PXColorFormat pxColorFormat);
	PXPublic PXActionResult PXImageSaveU(PXImage* const image, const PXTextUTF8 filePath, const FileFormatExtension fileFormat, const PXColorFormat pxColorFormat);
	PXPublic PXActionResult PXImageSaveD
	(
		PXImage* image,
		PXFile* const PXFile,
		const FileFormatExtension fileFormat,
		const PXColorFormat dataFormat
	);

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