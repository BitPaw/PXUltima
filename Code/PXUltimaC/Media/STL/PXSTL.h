#ifndef PXSTLINCLUDE
#define PXSTLINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXSTL_
	{
		unsigned int __dummy__;
	}
	PXSTL;

	PXPublic PXActionResult PXAPI PXSTLLoadFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXSTLSaveToFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
