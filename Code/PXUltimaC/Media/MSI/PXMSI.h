#ifndef PXMSIINCLUDE
#define PXMSIINCLUDE

#include "../PXResource.h"

typedef struct PXMSI_
{
	int x;
}
PXMSI;

PXPublic PXActionResult PXAPI PXMSILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXMSISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif