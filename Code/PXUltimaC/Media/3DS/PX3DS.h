#ifndef PX3DSINCLUDE
#define PX3DSINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXAutodesk3DS
	{
		unsigned int __dummy__;
	}
	PXAutodesk3DS;

	PXPublic PXActionResult PXAPI PXAutodesk3DSLoadFromFile(PXVertexStructure * const pxVertexStructure, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXAutodesk3DSSafeFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
