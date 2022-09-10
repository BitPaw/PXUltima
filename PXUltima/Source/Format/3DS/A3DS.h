#ifndef A3DSInclude
#define A3DSInclude

#include <Format/Type.h>
#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct A3DS_
	{
		unsigned int __dummy__;
	}
	A3DS;

	CPublic ActionResult A3DSParse(const void* data, const size_t dataSize);

#ifdef __cplusplus
}
#endif

#endif