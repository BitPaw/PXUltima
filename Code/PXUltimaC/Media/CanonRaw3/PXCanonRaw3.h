#ifndef PXCanonRaw3Include
#define PXCanonRaw3Include

#include "../PXResource.h"

#define PXCanonRaw3UUIDSize 16u

typedef struct PXCanonRaw3_
{


	// ISO chunk
	PXInt32U ISOChunkSize;

	char UUID[PXCanonRaw3UUIDSize];
}
PXCanonRaw3;

PXPublic PXActionResult PXAPI PXCanonRaw3LoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXCanonRaw3SaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif