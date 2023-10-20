#ifndef PXVRMLINCLUDE
#define PXVRMLINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXVRML_
	{
		unsigned int __dummy__;
	}
	PXVRML;

	PXPublic PXActionResult PXAPI PXVRMLLoadFromFile(PXModel* const pxModel, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXVRMLSaveToFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
