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

	PXPublic PXActionResult PXVRMLParseFromFile(PXModel* const pxModel, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif