#ifndef ImageInclude
#define ImageInclude

#include <Format/Type.h>
#include <Error/ActionResult.h>
#include <File/File.h>
#include <File/PXDataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef PXFont_
	typedef struct PXFont_ PXFont; // Avoid inclusion loop
#endif

	typedef enum ImageDataFormat_
	{
		ImageDataFormatInvalid,
		ImageDataFormatAlphaMaskBinary,
		ImageDataFormatAlphaMask,

		// 8-Bit
		ImageDataFormatRGB8,
		ImageDataFormatRGBA8,
		ImageDataFormatBGR8,
		ImageDataFormatBGRA8,

		// 16-Bit
		ImageDataFormatRGB16,
		ImageDataFormatRGBA16,

		ImageDataFormatRGBFloat,
		ImageDataFormatRGBAFloat
	}
	ImageDataFormat;

	typedef struct Image_
	{
		PXSize Width;
		PXSize Height;

		ImageDataFormat Format;

		PXSize PixelDataSize;
		void* PixelData;
	}
	Image;

	typedef  ActionResult(*ParseToImage)(Image* const image, PXDataStream* dataStream);
	typedef  ActionResult(*SerializeFromImage)(const Image* const image, PXDataStream* dataStream);

	PXPublic void ImageConstruct(Image* const image);
	PXPublic void ImageDestruct(Image* const image);

	PXPublic ActionResult ImageLoadA(Image* const image, const PXTextASCII filePath);
	PXPublic ActionResult ImageLoadW(Image* const image, const PXTextUNICODE filePath);
	PXPublic ActionResult ImageLoadU(Image* const image, const PXTextUTF8 filePath);
	PXPublic ActionResult ImageLoadD(Image* const image, PXDataStream* const dataStream, const FileFormatExtension guessedFormat);

	PXPublic ActionResult ImageSaveA(Image* const image, const char* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat);
	PXPublic ActionResult ImageSaveW(Image* const image, const wchar_t* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat);
	PXPublic ActionResult ImageSaveD
	(
		Image* image,
		void* const data,
		const PXSize dataSize,
		const FileFormatExtension fileFormat,
		const ImageDataFormat dataFormat
	);

	PXPublic PXSize ImageBitDepth(const ImageDataFormat imageDataFormat);
	PXPublic PXSize ImageBytePerPixel(const ImageDataFormat imageDataFormat);
	PXPublic PXSize ImageBitsPerPixel(const ImageDataFormat imageDataFormat);

	PXPublic PXBool ImageResize(Image* const image, const ImageDataFormat format, const PXSize width, const PXSize height);
	PXPublic void ImageFlipHorizontal(Image* image);
	PXPublic void ImageFlipVertical(Image* image);
	PXPublic void ImageRemoveColor(Image* image, unsigned char red, unsigned char green, unsigned char blue);
	PXPublic void ImageFillColorRGBA8(Image* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);

	PXPublic void* ImageDataPoint(const Image* const image, const PXSize x, const PXSize y);

	PXPublic PXSize ImagePixelPosition
	(
		const Image* const image,
		const PXSize x,
		const PXSize y
	);
	PXPublic void ImagePixelSetRGB8
	(
		Image* const image,
		const PXSize x,
		const PXSize y,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue
	);

	PXPublic void ImageDrawRectangle
	(
		Image* const image,
		const PXSize x,
		const PXSize y,
		const PXSize width,
		const PXSize height,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue,
		const unsigned char alpha
	);
	PXPublic void ImageDrawTextA
	(
		Image* const image,
		const PXSize x,
		const PXSize y,
		const PXSize width,
		const PXSize height,
		const PXFont* const font,
		const char* text
	);
	PXPublic void ImageDrawTextW
	(
		Image* const image,
		const PXSize x,
		const PXSize y,
		const PXSize width,
		const PXSize height,
		const PXFont* const font,
		const wchar_t* text
	);
	PXPublic void ImageMerge
	(
		Image* const image,
		const PXSize x,
		const PXSize y,
		const PXSize InsertX,
		const PXSize InsertY,
		const PXSize InsertWidth,
		const PXSize InsertHeight,
		const Image* const imageInsert
	);

	//CPublic void ImageFillRandome();
	//CPublic void ImageFormatChange(ImageDataFormat imageFormat);


#ifdef __cplusplus
}
#endif

#endif