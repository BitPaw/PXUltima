#ifndef STEPInclude
#define STEPInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// (St)andard (E)xchange (P)roduct Data (ISO 10303-2xx)
	typedef struct STEP_
	{
		unsigned int __dummy__;
	}
	STEP;

	CPublic ActionResult STEPParse(STEP* step, const void* data, const size_t dataSize, size_t* dataRead);	

#ifdef __cplusplus
}
#endif

#endif