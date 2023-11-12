#ifndef PXFLACINCLUDE
#define PXFLACINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXFLAC_
	{
		unsigned int __dummy__;
	}
	PXFLAC;

	PXPublic PXActionResult PXAPI PXFLACLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXFLACSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif