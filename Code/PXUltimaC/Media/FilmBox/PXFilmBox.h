#ifndef PXFBXINCLUDE
#define PXFBXINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXFilmBox_
	{
		unsigned int __dummy__;
	}
	PXFilmBox;

	PXPublic PXActionResult PXFilmBoxParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif