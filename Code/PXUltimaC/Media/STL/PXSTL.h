#ifndef PXSTLINCLUDE
#define PXSTLINCLUDE

#include <Media/PXModel.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXSTL_
	{
		unsigned int __dummy__;
	}
	PXSTL;

	PXPublic PXActionResult PXSTLLoadFromFile(PXModel* const pxModel, PXFile* const pxFile);
	PXPublic PXActionResult PXSTLSaveToFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif