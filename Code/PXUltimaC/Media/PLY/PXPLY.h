#ifndef PXPLYINCLUDE
#define PXPLYINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPLY_
	{
		unsigned int __dummy__;
	}
	PXPLY;

	PXPublic PXActionResult PXAPI PXPLYLoadFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXPLYSaveToFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
