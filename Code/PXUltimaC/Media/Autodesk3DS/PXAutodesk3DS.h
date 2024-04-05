#ifndef PX3DSINCLUDE
#define PX3DSINCLUDE

#include "../PXResource.h"

typedef struct PXAutodesk3DS
{
	unsigned int __dummy__;
}
PXAutodesk3DS;

PXPublic PXActionResult PXAPI PXAutodesk3DSLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXAutodesk3DSSafeFromFile(PXResourceSaveInfo* const pxResourceSafeInfo);

#endif