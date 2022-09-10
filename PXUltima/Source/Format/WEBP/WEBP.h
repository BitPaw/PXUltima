#ifndef WEBPInclude
#define WEBPInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct WEBP_
	{
		unsigned int __dummy__;
	}
	WEBP;

	CPublic ActionResult WEBPParse(WEBP* webp, const void* data, const size_t dataSize, size_t* dataRead);

#ifdef __cplusplus
}
#endif

#endif