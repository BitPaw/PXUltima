#ifndef PXMSIINCLUDE
#define PXMSIINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXMSI_
	{
		int x;
	}
	PXMSI;

	PXPublic PXActionResult PXAPI PXMSILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXMSISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif