#ifndef PXRARInclude
#define PXRARInclude

#include "../PXResource.h"

typedef enum PXRARVersion_
{
	PXRARVersion1x3,
	PXRARVersion1x5,
	PXRARVersion2x0,
	PXRARVersion2x9,
	PXRARVersion5x0
}
PXRARVersion;

// Roshal Archive
typedef struct PXRAR_
{
	PXRARVersion Version;
}
PXRAR;

PXPublic PXActionResult PXAPI PXRARLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXRARSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif