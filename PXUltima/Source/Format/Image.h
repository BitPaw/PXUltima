#ifndef ImageInclude
#define ImageInclude

#include <Format/Type.h>
#include <Error/ActionResult.h>
#include <File/File.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef PXFont_
	typedef struct PXFont_ PXFont; // Avoid inclusion loop
#endif

	typedef  enum ImageDataFormat_
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
		size_t Width;
		size_t Height;

		ImageDataFormat Format;

		size_t PixelDataSize;
		void* PixelData;
	}
	Image;

	typedef  ActionResult(*ParseToImage)(Image* const image, void* data, const size_t dataSize, size_t* dataWritten);
	typedef  ActionResult(*SerializeFromImage)(const Image* const image, void* data, const size_t dataSize, size_t* dataWritten);

	PXPublic void ImageConstruct(Image* const image);
	PXPublic void ImageDestruct(Image* const image);

	PXPublic ActionResult ImageLoadA(Image* const image, const char* const filePath);
	PXPublic ActionResult ImageLoadW(Image* const image, const wchar_t* const filePath);
	PXPublic ActionResult ImageLoadD(Image* const image, const void* const data, const size_t dataSize, const FileFormatExtension guessedFormat);

	PXPublic ActionResult ImageSaveA(Image* const image, const char* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat);
	PXPublic ActionResult ImageSaveW(Image* const image, const wchar_t* const filePath, const FileFormatExtension fileFormat, const ImageDataFormat dataFormat);
	PXPublic ActionResult ImageSaveD
	(
		Image* image,
		void* const data,
		const size_t dataSize,
		const FileFormatExtension fileFormat,
		const ImageDataFormat dataFormat
	);

	PXPublic size_t ImageBitDepth(const ImageDataFormat imageDataFormat);
	PXPublic size_t ImageBytePerPixel(const ImageDataFormat imageDataFormat);
	PXPublic size_t ImageBitsPerPixel(const ImageDataFormat imageDataFormat);

	PXPublic void ImageResize(Image* image, const ImageDataFormat format, const size_t width, const size_t height);
	PXPublic void ImageFlipHorizontal(Image* image);
	PXPublic void ImageFlipVertical(Image* image);
	PXPublic void ImageRemoveColor(Image* image, unsigned char red, unsigned char green, unsigned char blue);
	PXPublic void ImageFillColorRGBA8(Image* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);

	PXPublic void* ImageDataPoint(const Image* const image, const size_t x, const size_t y);

	PXPublic size_t ImagePixelPosition
	(
		const Image* const image,
		const size_t x,
		const size_t y
	);
	PXPublic void ImagePixelSetRGB8
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue
	);

	PXPublic void ImageDrawRectangle
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const size_t width,
		const size_t height,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue,
		const unsigned char alpha
	);
	PXPublic void ImageDrawTextA
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const size_t width,
		const size_t height,
		const PXFont* const font,
		const char* text
	);
	PXPublic void ImageDrawTextW
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const size_t width,
		const size_t height,
		const PXFont* const font,
		const wchar_t* text
	);
	PXPublic void ImageMerge
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const size_t InsertX,
		const size_t InsertY,
		const size_t InsertWidth,
		const size_t InsertHeight,
		const Image* const imageInsert
	);

	//CPublic void ImageFillRandome();
	//CPublic void ImageFormatChange(ImageDataFormat imageFormat);


#ifdef __cplusplus
}
#endif

#endif