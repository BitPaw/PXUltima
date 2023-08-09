#ifndef PXVRMLINCLUDE
#define PXVRMLINCLUDE

#include <Media/PXModel.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXVRML_
	{
		unsigned int __dummy__;
	}
	PXVRML;

	PXPublic PXActionResult PXVRMLLoadFromFile(PXModel* const pxModel, PXFile* const pxFile);
	PXPublic PXActionResult PXVRMLSaveToFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif