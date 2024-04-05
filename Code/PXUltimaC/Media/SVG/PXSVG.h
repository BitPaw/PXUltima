#ifndef PXSVGINCLUDE
#define PXSVGINCLUDE

#include "../PXResource.h"

typedef struct PXSVG_
{
	unsigned int __dummy__;
}
PXSVG;

PXPublic PXActionResult PXAPI PXSVGLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXSVGSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif