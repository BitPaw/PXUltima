#ifndef PXWEBPINCLUDE
#define PXWEBPINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXWEBPChunk_
	{
		PXWEBPChunkInvalid,
		PXWEBPChunkUnkown,
		PXWEBPChunkVP8L,
		PXWEBPChunkVP8X,
		PXWEBPChunkICCP,
		PXWEBPChunkANIM,
		PXWEBPChunkEXIF,
		PXWEBPChunkXMP
	}
	PXWEBPChunk;

	typedef struct PXWEBP_
	{
		unsigned int __dummy__;
	}
	PXWEBP;

	PXPublic PXActionResult PXAPI PXWEBPLoadFromFile(PXImage* const pxImage, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXWEBPSaveToFile(PXImage* const pxImage, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif