#ifndef ImageInclude
#define ImageInclude

#include <Format/Type.h>
#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef PXFont_
	typedef struct PXFont_ PXFont; // Avoid inclusion loop
#endif

	typedef enum ImageFileFormat_
	{
		ImageFileFormatUnkown,
		ImageFileFormatBitMap,
		ImageFileFormatPNG,
		ImageFileFormatTGA,
		ImageFileFormatJPEG,
		ImageFileFormatTIFF,
		ImageFileFormatGIF
	}
	ImageFileFormat;

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

	CPublic void ImageConstruct(Image* const image);
	CPublic void ImageDestruct(Image* const image);

	CPublic ImageFileFormat ImageGuessFormat(const wchar_t* const filePath);

	CPublic ActionResult ImageLoadA(Image* const image, const char* const filePath);
	CPublic ActionResult ImageLoadW(Image* const image, const wchar_t* const filePath);
	CPublic ActionResult ImageLoadD(Image* const image, const void* const data, const size_t dataSize, const ImageFileFormat guessedFormat);

	CPublic ActionResult ImageSaveA(Image* const image, const char* const filePath, const ImageFileFormat fileFormat, const ImageDataFormat dataFormat);
	CPublic ActionResult ImageSaveW(Image* const image, const wchar_t* const filePath, const ImageFileFormat fileFormat, const ImageDataFormat dataFormat);
	CPublic ActionResult ImageSaveD
	(
		Image* image,
		void* const data,
		const size_t dataSize,
		const ImageFileFormat fileFormat,
		const ImageDataFormat dataFormat
	);

	CPublic size_t ImageBitDepth(const ImageDataFormat imageDataFormat);
	CPublic size_t ImageBytePerPixel(const ImageDataFormat imageDataFormat);
	CPublic size_t ImageBitsPerPixel(const ImageDataFormat imageDataFormat);

	CPublic void ImageResize(Image* image, const ImageDataFormat format, const size_t width, const size_t height);
	CPublic void ImageFlipHorizontal(Image* image);
	CPublic void ImageFlipVertical(Image* image);
	CPublic void ImageRemoveColor(Image* image, unsigned char red, unsigned char green, unsigned char blue);
	CPublic void ImageFillColorRGBA8(Image* const image, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);

	CPublic void* ImageDataPoint(const Image* const image, const size_t x, const size_t y);

	CPublic size_t ImagePixelPosition
	(
		const Image* const image,
		const size_t x,
		const size_t y
	);
	CPublic void ImagePixelSetRGB8
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue
	);

	CPublic void ImageDrawRectangle
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
	CPublic void ImageDrawTextA
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const size_t width,
		const size_t height,
		const PXFont* const font,
		const char* text
	);
	CPublic void ImageDrawTextW
	(
		Image* const image,
		const size_t x,
		const size_t y,
		const size_t width,
		const size_t height,
		const PXFont* const font,
		const wchar_t* text
	);
	CPublic void ImageMerge
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