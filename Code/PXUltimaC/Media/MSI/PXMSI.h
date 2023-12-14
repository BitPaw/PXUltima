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

	PXPublic PXActionResult PXAPI PXMSILoadFromFile(void* const data, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXMSISaveToFile(void* const data, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif