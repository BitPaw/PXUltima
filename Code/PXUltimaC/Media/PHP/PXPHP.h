#ifndef PXPHPINCLUDE
#define PXPHPINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPHP_
	{
		int x;
	}
	PXPHP;

	PXPublic PXActionResult PXAPI PXPHPLoadFromFile(void* const data, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXPHPSaveToFile(void* const data, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif