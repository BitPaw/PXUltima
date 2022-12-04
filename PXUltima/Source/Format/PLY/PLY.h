#ifndef PLYInclude
#define PLYInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PLY_
	{
		unsigned int __dummy__;
	}
	PLY;

	PXPublic ActionResult PLYParse(PLY* ply, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif