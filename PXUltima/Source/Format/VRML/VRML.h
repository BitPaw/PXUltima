#ifndef VRMLInclude
#define VRMLInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct VRML_
	{
		unsigned int __dummy__;
	}
	VRML;

	PXPublic ActionResult VRMLParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif