#ifndef FLACInclude
#define FLACInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct FLAC_
	{
		unsigned int __dummy__;
	}
	FLAC;

	PXPublic ActionResult FLACParse(const void* data, const size_t dataSize);

#ifdef __cplusplus
}
#endif

#endif