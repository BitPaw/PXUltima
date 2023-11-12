#ifndef PXPLYINCLUDE
#define PXPLYINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPLY_
	{
		unsigned int __dummy__;
	}
	PXPLY;

	PXPublic PXActionResult PXAPI PXPLYLoadFromFile(PXModel* const pxModel, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXPLYSaveToFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
