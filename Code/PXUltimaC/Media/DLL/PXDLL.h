#ifndef DLLINCLUDE
#define DLLINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXDLL
	{
		unsigned int __dummy__;
	}
	PXDLL;

	PXPublic PXActionResult PXDLLLoadFromFile(PXDLL* const pxDLL, PXFile* const pxFile);
	PXPublic PXActionResult PXDLLSaveToFile(PXDLL* const pxDLL, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif