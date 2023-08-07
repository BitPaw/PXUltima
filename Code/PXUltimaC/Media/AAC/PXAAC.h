#ifndef PXAACINCLUDE
#define PXAACINCLUDE

#include <OS/Graphic/PXGraphicGeneral.h>

#ifdef __cplusplus
extern "C"
{
#endif


	typedef struct PXAAC_
	{
		unsigned int __dummy__;
	}
	PXAAC;

	PXPublic PXActionResult PXAACParseFromFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif