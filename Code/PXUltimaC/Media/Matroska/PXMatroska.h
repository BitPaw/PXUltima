#ifndef PXMatroskaInclude
#define PXMatroskaInclude

#include "../PXResource.h"

typedef struct PXMatroska_
{
	unsigned int __dummy__;
}
PXMatroska;

PXPublic PXActionResult PXAPI PXMatroskaLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXMatroskaSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif