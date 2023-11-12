#ifndef PXCanonRaw3Include
#define PXCanonRaw3Include

#include "../PXResource.h"

#define PXCanonRaw3UUIDSize 16u

#ifdef __cplusplus
extern "C"
{
#endif


	typedef struct PXCanonRaw3_
	{


		// ISO chunk
		PXInt32U ISOChunkSize;

		char UUID[PXCanonRaw3UUIDSize];
	}
	PXCanonRaw3;

	PXPublic PXActionResult PXAPI PXCanonRaw3LoadFromFile(PXImage* const pxImage, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXCanonRaw3SaveToFile(PXImage* const pxImage, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif