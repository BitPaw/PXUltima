#ifndef PXGIFINCLUDE
#define PXGIFINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXGIFImageDescriptor_
	{
		PXInt16U LeftPosition;
		PXInt16U TopPosition;
		PXInt16U Width;
		PXInt16U Height;
		PXInt8U Separator;
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

		PXInt8U BackgroundColorIndex;
		PXInt8U PixelAspectRatio;

		PXInt8U GlobalColorTableSize;
		PXInt8U ColorResolution;
	
		PXBool IsGlobalColorTablePresent;
		PXBool IsSorted;	
	}
	PXGIF;

	PXPublic PXSize PXAPI PXGIFFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

	PXPublic PXActionResult PXAPI PXGIFLoadFromFile(PXVideo* const pxVideo, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXGIFSaveToFile(const PXVideo* const pxVideo, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif