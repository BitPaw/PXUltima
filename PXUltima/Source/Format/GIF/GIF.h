#ifndef GIFInclude
#define GIFInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <Format/Image.h>
#include <File/PXDataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct GIFImageDescriptor_
	{
		unsigned char Separator;
		unsigned short LeftPosition;
		unsigned short TopPosition;
		unsigned short Width;
		unsigned short Height;
		unsigned char LocalColorTableSize;
		unsigned char Reserved;
		unsigned char SortFlag;
		unsigned char InterlaceFlag;
		unsigned char LocalColorTableFlag;
	}
	GIFImageDescriptor;

	typedef struct GIF_
	{
		unsigned short Width;
		unsigned short Height;

		unsigned char GlobalColorTableSize;
		unsigned char IsSorted;
		unsigned char ColorResolution;
		unsigned char IsGlobalColorTablePresent;

		unsigned char BackgroundColorIndex;
		unsigned char PixelAspectRatio;
	}
	GIF;

	PXPublic PXSize GIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

	PXPublic ActionResult GIFLoad(GIF* gif, const void* data, const PXSize dataSize, PXSize* dataRead);

	PXPublic ActionResult GIFParseToImage(Image* const image, PXDataStream* const dataStream);

	PXPublic ActionResult GIFSerializeFromImage(const Image* const image, void* data, const PXSize dataSize, PXSize* dataWritten);

#ifdef __cplusplus
}
#endif

#endif