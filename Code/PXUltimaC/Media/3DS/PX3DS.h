#ifndef PX3DSINCLUDE
#define PX3DSINCLUDE

#include <Media/PXModel.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXAutodesk3DS
	{
		unsigned int __dummy__;
	}
	PXAutodesk3DS;

	PXPublic PXActionResult PXAutodesk3DSParseFromFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif