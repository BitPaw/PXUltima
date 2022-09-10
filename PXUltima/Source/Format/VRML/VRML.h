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

	CPublic ActionResult VRMLParse(const void* data, const size_t dataSize);

#ifdef __cplusplus
}
#endif

#endif