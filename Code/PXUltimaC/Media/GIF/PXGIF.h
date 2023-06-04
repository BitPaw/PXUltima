#ifndef PXGIFINCLUDE
#define PXGIFINCLUDE

#include <Media/PXImage.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXGIFImageDescriptor_
	{
		PXInt8U Separator;
		PXInt16U LeftPosition;
		PXInt16U TopPosition;
		PXInt16U Width;
		PXInt16U Height;
		PXInt8U LocalColorTableSize;
		PXInt8U Reserved;
		PXInt8U SortFlag;
		PXInt8U InterlaceFlag;
		PXInt8U LocalColorTableFlag;
	}
	PXGIFImageDescriptor;

	typedef struct PXGIF_
	{
		PXInt16U Width;
		PXInt16U Height;

		PXInt8U GlobalColorTableSize;
		PXBool IsSorted;
		PXInt8U ColorResolution;
		PXBool IsGlobalColorTablePresent;

		PXInt8U BackgroundColorIndex;
		PXInt8U PixelAspectRatio;
	}
	PXGIF;

	PXPublic PXSize PXGIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

	PXPublic PXActionResult PXGIFParseToImage(PXImage* const image, PXFile* const dataStream);

	PXPublic PXActionResult PXGIFSerializeFromImage(const PXImage* const image, PXFile* const dataStream);

#ifdef __cplusplus
}
#endif

#endif