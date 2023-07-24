#ifndef PXPLYINCLUDE
#define PXPLYINCLUDE

#include <Media/PXModel.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPLY_
	{
		unsigned int __dummy__;
	}
	PXPLY;

	PXPublic PXActionResult PXPLYParseFromFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif